/*


 Copyright (c) 2002-2017 Microsemi Corporation "Microsemi". All Rights Reserved.

 Unpublished rights reserved under the copyright laws of the United States of
 America, other countries and international treaties. Permission to use, copy,
 store and modify, the software and its source code is granted but only in
 connection with products utilizing the Microsemi switch and PHY products.
 Permission is also granted for you to integrate into other products, disclose,
 transmit and distribute the software only in an absolute machine readable format
 (e.g. HEX file) and only in or with products utilizing the Microsemi switch and
 PHY products.  The source code of the software may not be disclosed, transmitted
 or distributed without the prior written permission of Microsemi.

 This copyright notice must appear in any copy, modification, disclosure,
 transmission or distribution of the software.  Microsemi retains all ownership,
 copyright, trade secret and proprietary rights in the software and its source code,
 including all modifications thereto.

 THIS SOFTWARE HAS BEEN PROVIDED "AS IS". MICROSEMI HEREBY DISCLAIMS ALL WARRANTIES
 OF ANY KIND WITH RESPECT TO THE SOFTWARE, WHETHER SUCH WARRANTIES ARE EXPRESS,
 IMPLIED, STATUTORY OR OTHERWISE INCLUDING, WITHOUT LIMITATION, WARRANTIES OF
 MERCHANTABILITY, FITNESS FOR A PARTICULAR USE OR PURPOSE AND NON-INFRINGEMENT.

*/
//***************************************************************************
//* This file contains board specific functions needed for running the PHY  *
//* API at a Tesla evaluation board. The evaluation board is equipped with *
//* a Rabbit CPU board, which do the communication with the PHY using a     *
//* socket connection. The actual API is running on the host computer. The  *
//* API has been tested with both Linux (Red Hat) and Cygwin.               *
//***************************************************************************
#include <netdb.h>  // For socket
#include <stdarg.h> // For va_list
#include "vtss_api.h"   // For BOOL and friends
#include "vtss_appl.h"  // For board types
#include <unistd.h>     // For TCP read/write

#ifdef _INCLUDE_DEBUG_FILE_PRINT_
extern  FILE   *fp;
#endif


// Fixed socket port for the CPU board used
#define CPU_BOARD_PORT "26"

// Define which trace group to use for VTSS printout in this file
#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_PHY

/* ================================================================= *
 *  Misc. functions
 * ================================================================= */

// Function defining the port interface.
static vtss_port_interface_t port_interface(vtss_port_no_t port_no)
{
    return VTSS_PORT_INTERFACE_SGMII;
}

/* ================================================================= *
 *  Board specific functions
 * ================================================================= */

int sockfd;

// Function for do read access from the board CPU board via socket
// In :  Buffer - Pointer for the data read via the socket.
static void socket_read (char *buffer) {
    int n;

    n = read(sockfd, buffer, 255);
    if (n < 0)
        T_E("ERROR reading from socket");
}

// Function for doing write access to the board CPU board via socket
// In :  Buffer - Pointer to the text to send over the socket
static void socket_write (char *buffer) {
    int n;
    n = write(sockfd, buffer, strlen(buffer));
    if (n < 0) {
        T_E("ERROR writing to socket");
    } else {
        T_N("TX: %s\n", buffer);
    }
}

// Function for initializing the socket connection to the CPU board.
// In : server_addr - IP address for the CPU board
//      port        - Port used for the socket connection
static void socket_init (const char *server_addr, const char *port) {
    struct hostent *server;
    struct sockaddr_in serv_addr;

    int portno = atoi(port);

    server = gethostbyname(server_addr);
    sockfd = socket(AF_INET, SOCK_STREAM, 0);

    if (sockfd < 0)
        T_E("ERROR opening socket");
 
    if (server == NULL) {
        fprintf(stderr,"ERROR, no such host\n");
        exit(0);
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char *)server->h_addr,
         (char *)&serv_addr.sin_addr.s_addr,
         server->h_length);
    serv_addr.sin_port = htons(portno);

    if (connect(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0)
        T_E("ERROR connecting");
}

// Each board can have it own way of communicating with the chip. The miim read and write function are called by the API 
// when the API needs to do register access.


// Miim read access specific for this board.
// In : port_no - The port to access.
//      addr    - The address to access
//
// In/Out:  value   - Pointer to the value to be returned
vtss_rc miim_read(const vtss_inst_t    inst,
                  const vtss_port_no_t port_no,
                  const u8             addr,
                  u16                  *const value)
{
    char buffer[255];
    int v;
 
    sprintf(buffer, "miimread %d %d \n", port_no, addr);
#ifdef _INCLUDE_DEBUG_TERM_PRINT_
//    printf ("miim_read: %s", buffer);
#endif
    socket_write(&buffer[0]);

    memset(buffer, 0, sizeof(buffer));
    socket_read(&buffer[0]);
    if (sscanf(buffer, "[%d]", &v) == 1) {
        *value = v;
    } else {
        T_E("missing value -buffer = %s, port %u, addr =%d", buffer, port_no, addr);
    }

    T_N("miim read port_no = %d, addr = %d, value = 0x%X", port_no, addr, *value);
#ifdef _INCLUDE_DEBUG_FILE_PRINT_
    fprintf(fp,"miim__read_port_no: %d; phy_addr: 0x%02X; value: 0x%04X\n", port_no, addr, *value);
#endif

    return VTSS_RC_OK;
}

// Miim write access specific for this board.
// In : port_no - The port to access.
//      addr    - The address to access
//      value   - The value to written
//
vtss_rc miim_write(const vtss_inst_t    inst,
                   const vtss_port_no_t port_no,
                   const u8             addr,
                   const u16            value)
{
    char buffer[255];
    sprintf(buffer, " miimwrite %d %d 0x%X\n", port_no, addr, value);
#ifdef _INCLUDE_DEBUG_TERM_PRINT_
//    printf ("%s \n", buffer);
#endif
    T_N("miim_writes port_no = %d, addr = %d, value = 0x%X", port_no, addr ,value);
#ifdef _INCLUDE_DEBUG_FILE_PRINT_
    fprintf(fp,"miim_write_port_no: %d; phy_addr: 0x%02X; value: 0x%04X\n", port_no, addr, value);
#endif
    socket_write(&buffer[0]);
    socket_read(&buffer[0]);
    return VTSS_RC_OK;
}

#if defined(VTSS_CHIP_10G_PHY)
vtss_rc mmd_read(const vtss_port_no_t  port_no,
                        const u8              mmd,
                        u16                   addr,
                        u16                   *const value)
{
    /* Must be filled out by the user */
    T_N("mmd_read port_no = %d, mmd = %d addr = %d, value = 0x%X", port_no, mmd, addr);
    return VTSS_RC_OK;
}

vtss_rc mmd_write(const vtss_port_no_t  port_no,
                         const u8              mmd,
                         u16                   addr,
                         u16                   data)
{
    /* Must be filled out by the user */
    T_N("mmd_write port_no = %d, mmd = %d addr = %d, value = 0x%X", port_no, addr, data);
    return VTSS_RC_OK;
}
#endif /* VTSS_CHIP_10G_PHY */

// Function for initializing the hardware board.
int tesla_board_init(int argc, const char **argv, vtss_appl_board_t *board)
{
    board->port_count = VTSS_PORTS; //Setup the number of port used 

    board->port_interface = port_interface; // Define the port interface

    board->init.init_conf->miim_read =  miim_read; // Set pointer to the MIIM read function for this board.
    board->init.init_conf->miim_write = miim_write; // Set pointer to the MIIM write function for this board.
#if defined(VTSS_CHIP_10G_PHY)
    board->init.init_conf->mmd_read =  mmd_read; // Set pointer to the MIIM read function for this board.
    board->init.init_conf->mmd_write = mmd_write; // Set pointer to the MIIM write function for this board.
#endif /* VTSS_CHIP_10G_PHY */


        if (argc != 2) {
            printf("Usage  : %s <Rabbit IP Address> \n"  , argv[0]);
            printf("Example: %s 10.10.132.59 \n"  , argv[0]);
            exit(EXIT_SUCCESS);
        }

        socket_init(argv[1], CPU_BOARD_PORT); // Connect to the CPU board
	if (board->init.init_conf->warm_start_enable != TRUE) {
		// Set signal detect polarity (for SFPs for the board)
		miim_write(NULL, 0, 31, 1);
		miim_write(NULL, 0, 19, 1);
		miim_write(NULL, 0, 31, 0);

		miim_write(NULL, 3, 31, 1);
		miim_write(NULL, 3, 19, 1);
		miim_write(NULL, 3, 31, 0);
	}
    return 0;
}




