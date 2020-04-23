#ifndef _VTSS_VENICE_REGS_SFP_I2C_H_
#define _VTSS_VENICE_REGS_SFP_I2C_H_

/*


 Copyright (c) 2004-2018 Microsemi Corporation "Microsemi".

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in all
 copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 SOFTWARE.

*/

#include "vtss_venice_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a SFP_I2C
 *
 * 
 *
 ***********************************************************************/

/**
 * Register Group: \a SFP_I2C:SFP_I2C_CFG
 *
 * Configuration Registers for I2C Master Interface for SFP Module
 */

/**
 * Register Group: \a SFP_I2C:I2C_WRITE
 *
 * I2C Write Control Register
 */


/** 
 * \brief I2C Write Control Register
 *
 * \details
 * I2C Write Control Register
 *
 * Register: \a SFP_I2C:I2C_WRITE:I2C_WRITE_CTRL
 */
#define VTSS_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL  VTSS_IOREG(0x01, 0, 0xc002)

/** 
 * \brief
 * I2C Write Data. A write to I2C_WRITE_CTRL register will trigger I2C
 * master to write the value in WRITE_DATA register to address specified in
 * WRITE_ADDR register of slave ID specified in SLAVE_ID register
 *
 * \details 
 * Field: VTSS_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL . WRITE_DATA
 */
#define  VTSS_F_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_DATA(x)  VTSS_ENCODE_BITFIELD(x,8,8)
#define  VTSS_M_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_DATA     VTSS_ENCODE_BITMASK(8,8)
#define  VTSS_X_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_DATA(x)  VTSS_EXTRACT_BITFIELD(x,8,8)

/** 
 * \brief
 * I2C Write Address. A write to I2C_WRITE_CTRL register will trigger I2C
 * master to write the value in WRITE_DATA register to address specified in
 * WRITE_ADDR register of slave ID specified in SLAVE_ID register
 *
 * \details 
 * Field: VTSS_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL . WRITE_ADDR
 */
#define  VTSS_F_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_ADDR     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_SFP_I2C_I2C_WRITE_I2C_WRITE_CTRL_WRITE_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a SFP_I2C:I2C_BUS_STAT
 *
 * I2C Bus Status
 */


/** 
 * \brief I2C Bus Status
 *
 * \details
 * I2C Bus Status
 *
 * Register: \a SFP_I2C:I2C_BUS_STAT:I2C_BUS_STATUS
 */
#define VTSS_SFP_I2C_I2C_BUS_STAT_I2C_BUS_STATUS  VTSS_IOREG(0x01, 0, 0xc003)

/** 
 * \brief
 * I2C Write Acknowledge
 *
 * \details 
 * 0: Idle
 * 1: Write Acknowledge
 *
 * Field: VTSS_SFP_I2C_I2C_BUS_STAT_I2C_BUS_STATUS . I2C_WRITE_ACK
 */
#define  VTSS_F_SFP_I2C_I2C_BUS_STAT_I2C_BUS_STATUS_I2C_WRITE_ACK  VTSS_BIT(1)

/** 
 * \brief
 * I2C Bus Busy
 *
 * \details 
 * 0: I2C bus is not busy
 * 1: I2C bus is busy
 *
 * Field: VTSS_SFP_I2C_I2C_BUS_STAT_I2C_BUS_STATUS . I2C_BUS_BUSY
 */
#define  VTSS_F_SFP_I2C_I2C_BUS_STAT_I2C_BUS_STATUS_I2C_BUS_BUSY  VTSS_BIT(0)

/**
 * Register Group: \a SFP_I2C:I2C_READ_ADDRESS
 *
 * I2C Read Address Register
 */


/** 
 * \brief I2C Read Address Register
 *
 * \details
 * I2C Read Address Register
 *
 * Register: \a SFP_I2C:I2C_READ_ADDRESS:I2C_READ_ADDR
 */
#define VTSS_SFP_I2C_I2C_READ_ADDRESS_I2C_READ_ADDR  VTSS_IOREG(0x01, 0, 0xc004)

/** 
 * \brief
 * I2C Read Address. A write to READ_ADDR register will trigger I2C master
 * to read the value from the address specified  in READ_ADDR register of
 * slave ID specified in SLAVE_ID register, and stores the value at
 * READ_DATA register

 *
 * \details 
 * Field: VTSS_SFP_I2C_I2C_READ_ADDRESS_I2C_READ_ADDR . READ_ADDR
 */
#define  VTSS_F_SFP_I2C_I2C_READ_ADDRESS_I2C_READ_ADDR_READ_ADDR(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_SFP_I2C_I2C_READ_ADDRESS_I2C_READ_ADDR_READ_ADDR     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_SFP_I2C_I2C_READ_ADDRESS_I2C_READ_ADDR_READ_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a SFP_I2C:I2C_READ_STATUS_AND_DATA
 *
 * I2C Read Status And Data
 */


/** 
 * \brief I2C Read Status And Data
 *
 * \details
 * I2C Read Status And Data
 *
 * Register: \a SFP_I2C:I2C_READ_STATUS_AND_DATA:I2C_READ_STATUS_DATA
 */
#define VTSS_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA  VTSS_IOREG(0x01, 0, 0xc005)

/** 
 * \brief
 * I2C Bus Busy
 *
 * \details 
 * 0: I2C bus is not busy, data updated
 * 1: I2C bus is busy, data not updated
 *
 * Field: VTSS_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA . I2C_BUS_BUSY
 */
#define  VTSS_F_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA_I2C_BUS_BUSY  VTSS_BIT(15)

/** 
 * \brief
 * I2C Read Data. A write to READ_ADDR register will trigger I2C master to
 * read the value from the address specified  in READ_ADDR register of
 * slave ID specified in SLAVE_ID register, and stores the value at
 * READ_DATA register
 *
 * \details 
 * Field: VTSS_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA . READ_DATA
 */
#define  VTSS_F_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA_READ_DATA(x)  VTSS_ENCODE_BITFIELD(x,0,8)
#define  VTSS_M_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA_READ_DATA     VTSS_ENCODE_BITMASK(0,8)
#define  VTSS_X_SFP_I2C_I2C_READ_STATUS_AND_DATA_I2C_READ_STATUS_DATA_READ_DATA(x)  VTSS_EXTRACT_BITFIELD(x,0,8)

/**
 * Register Group: \a SFP_I2C:I2C_RESET_SEQUENCE
 *
 * I2C Reset Sequence
 */


/** 
 * \brief I2C Reset Sequence
 *
 * \details
 * I2C Reset Sequence
 *
 * Register: \a SFP_I2C:I2C_RESET_SEQUENCE:I2C_RESET_SEQ
 */
#define VTSS_SFP_I2C_I2C_RESET_SEQUENCE_I2C_RESET_SEQ  VTSS_IOREG(0x01, 0, 0xc006)

/** 
 * \brief
 * I2C Reset Sequence. A write to RESET_SEQ register (any value) will
 * trigger I2C master to issue a Reset Sequence.
 *
 * \details 
 * Field: VTSS_SFP_I2C_I2C_RESET_SEQUENCE_I2C_RESET_SEQ . RESET_SEQ
 */
#define  VTSS_F_SFP_I2C_I2C_RESET_SEQUENCE_I2C_RESET_SEQ_RESET_SEQ  VTSS_BIT(0)


#endif /* _VTSS_VENICE_REGS_SFP_I2C_H_ */
