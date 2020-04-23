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

/**
 * \file
 * \brief Generic types API
 * \details This header file describes generic types used in the API
 */

#ifndef _VTSS_API_TYPES_H_
#define _VTSS_API_TYPES_H_

#include <vtss/api/options.h>

/*
 * This determines whether to use integer standard types as defined by
 * <stdint.h>. If a particular compiler has this, but the check below
 * fails, you can either define VTSS_USE_STDINT_H or instruct the
 * compiler to turn on C99 mode or similar. (GCC: -std=gnu99).
 */
#if !defined(__KERNEL__) && !defined(VTSS_CHIP_DAYTONA)
#if defined(__STDC_VERSION__) && (__STDC_VERSION__ >= 199901L) ||		\
    defined(__linux__) || defined(__unix__) || defined(VTSS_USE_STDINT_H)

#include <stdint.h>
#include <inttypes.h>

/** \brief C99 Integer types */
typedef char               i8;   /**<  8-bit signed */
typedef int16_t            i16;  /**< 16-bit signed */
typedef int32_t            i32;  /**< 32-bit signed */
typedef int64_t            i64;  /**< 64-bit signed */

typedef uint8_t            u8;   /**<  8-bit unsigned */
typedef uint16_t           u16;  /**< 16-bit unsigned */
typedef uint32_t           u32;  /**< 32-bit unsigned */
typedef uint64_t           u64;  /**< 64-bit unsigned */

typedef uint8_t            BOOL; /**< Boolean implemented as 8-bit unsigned */

#else  /* Apparently cannot use stdint.h, fallback */

#include <sys/bsdtypes.h>
/** \brief Fallback Integer types */
typedef        char        i8;   /**<  8-bit signed */
typedef signed short       i16;  /**< 16-bit signed */
typedef signed int         i32;  /**< 32-bit signed */
typedef signed long long   i64;  /**< 64-bit signed */

typedef unsigned char      u8;   /**<  8-bit unsigned */
typedef unsigned short     u16;  /**< 16-bit unsigned */
typedef unsigned int       u32;  /**< 32-bit unsigned */
typedef unsigned long long u64;  /**< 64-bit unsigned */

typedef unsigned char      BOOL; /**< Boolean implemented as 8-bit unsigned */
typedef unsigned int       uintptr_t; /**< Unsigned integer big enough to hold pointers */

#endif

#else /* __KERNEL__ */

/** \brief Integer types */
#if !defined(VTSS_HAVE_I_TYPES)
typedef signed char        i8;   /**<  8-bit signed */
typedef signed short       i16;  /**< 16-bit signed */
typedef signed int         i32;  /**< 32-bit signed */
typedef signed long long   i64;  /**< 64-bit signed */
#endif

#if !defined(VTSS_HAVE_U_TYPES)
typedef unsigned char      u8;   /**<  8-bit unsigned */
typedef unsigned short     u16;  /**< 16-bit unsigned */
typedef unsigned int       u32;  /**< 32-bit unsigned */
typedef unsigned long long u64;  /**< 64-bit unsigned */
#endif

typedef unsigned char      BOOL; /**< Boolean implemented as 8-bit unsigned */
//typedef unsigned int       uintptr_t; /**< Unsigned integer big enough to hold pointers */
#endif /* __KERNEL__ */

#ifdef __cplusplus
extern "C" {
#endif

/* Formatting defaults if no inttypes available */

#if !defined(PRIu64)
#define PRIu64 "llu"           /**< Fallback un-signed 64-bit formatting string */
#endif

#if !defined(PRIi64)
#define PRIi64 "lli"           /**< Fallback signed 64-bit formatting string */
#endif

#if !defined(PRIx64)
#define PRIx64 "llx"           /**< Fallback hex 64-bit formatting string */
#endif

#define VTSS_BIT64(x)                  (1ULL << (x))                           /**< Set one bit in a 64-bit mask               */
#define VTSS_BITMASK64(x)              ((1ULL << (x)) - 1)                     /**< Get a bitmask consisting of x ones         */
#define VTSS_EXTRACT_BITFIELD64(x,o,w) (((x) >> (o)) & VTSS_BITMASK64(w))      /**< Extract w bits from bit position o in x    */
#define VTSS_ENCODE_BITFIELD64(x,o,w)  (((u64)(x) & VTSS_BITMASK64(w)) << (o)) /**< Place w bits of x at bit position o        */
#define VTSS_ENCODE_BITMASK64(o,w)     (VTSS_BITMASK64(w) << (o))              /**< Create a bitmask of w bits positioned at o */

#if !defined(TRUE)
#define TRUE  1 /**< True boolean value */
#endif
#if !defined(FALSE)
#define FALSE 0 /**< False boolean value */
#endif

/** \brief Error code type */
typedef int vtss_rc;

/** \brief Error codes */
enum 
{
    VTSS_RC_OK                                  =  0,  /**< Success */
    VTSS_RC_ERROR                               = -1,  /**< Unspecified error */
    VTSS_RC_INV_STATE                           = -2,  /**< Invalid state for operation */
    VTSS_RC_INCOMPLETE                          = -3,  /**< Incomplete result */

    VTSS_RC_ERR_CLK_CONF_NOT_SUPPORTED          = -6,  /**< The PHY doesn't support clock configuration (for SynceE) */
    VTSS_RC_ERR_KR_CONF_NOT_SUPPORTED           = -7,  /**< The PHY doesn't support 10GBASE_KR equalization */
    VTSS_RC_ERR_KR_CONF_INVALID_PARAMETER       = -8,  /**< One of the parameters are out of range */
    
    /**** 1G ERRORS ****/
    VTSS_RC_ERR_PHY_BASE_NO_NOT_FOUND           = -50,  /**< Port base number (first port within a chip) is not found */
    VTSS_RC_ERR_PHY_6G_MACRO_SETUP              = -51,  /**< Setup of 6G macro failed */
    VTSS_RC_ERR_PHY_MEDIA_IF_NOT_SUPPORTED      = -52,  /**< PHY does not support the selected media mode */
    VTSS_RC_ERR_PHY_CLK_CONF_NOT_SUPPORTED      = -53,  /**< The PHY doesn't support clock configuration (for SynceE) */
    VTSS_RC_ERR_PHY_GPIO_ALT_MODE_NOT_SUPPORTED = -54,  /**< The PHY doesn't support the alternative mode for the selected GPIO pin*/
    VTSS_RC_ERR_PHY_GPIO_PIN_NOT_SUPPORTED      = -55,  /**< The PHY doesn't support the selected GPIO pin */
    VTSS_RC_ERR_PHY_PORT_OUT_RANGE              = -56,  /**< PHY API called with port number larger than VTSS_PORTS*/
    VTSS_RC_ERR_PHY_PATCH_SETTING_NOT_SUPPORTED = -57,  /**< PHY API micro patch setting not supported for the port in question*/
    VTSS_RC_ERR_PHY_LCPLL_NOT_SUPPORTED         = -58,  /**< PHY API LC-PLL status not supported for the port */
    VTSS_RC_ERR_PHY_RCPLL_NOT_SUPPORTED         = -59,  /**< PHY API RC-PLL status not supported for the port */

    /**** MACSEC ERRORS ****/
    VTSS_RC_ERR_MACSEC_INVALID_SCI_MACADDR      = -60,  /**< From IEEE 802.1AE-2006, section 9.9 - The 64-bit value FF-FF-FF-FF-FF-FF is never used as an SCI and is reserved for use by implementations to indicate the absence of an SC or an SCI in contexts where an SC can be present */
    VTSS_RC_ERR_MACSEC_NOT_ENABLED              = -61,  /**< Trying to access port where MACSEC is not enabled*/
    VTSS_RC_ERR_MACSEC_SECY_ALREADY_IN_USE      = -63,  /**< Trying to use a secy which is already in use*/
    VTSS_RC_ERR_MACSEC_NO_SECY_FOUND            = -64,  /**< No SecY found for the specific port*/
    VTSS_RC_ERR_MACSEC_NO_SECY_VACANCY          = -65,  /**< No secy vacancy*/
    VTSS_RC_ERR_MACSEC_INVALID_VALIDATE_FRM     = -66,  /**< Validate_frames value invalid*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_PRG_SA_MATCH   = -67,  /**< Could not program the SA match*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_PRG_SA_FLOW    = -68,  /**< Could not program the SA flow*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_ENA_SA         = -69,  /**< Could not enable the SA*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_SET_SA         = -70,  /**< Could not set SA to in use*/
    VTSS_RC_ERR_MACSEC_INVALID_BYPASS_HDR_LEN   = -71,  /**< Invalid header bypass length*/
    VTSS_RC_ERR_MACSEC_SC_NOT_FOUND             = -72,  /**< Could not find SC (from sci)*/
    VTSS_RC_ERR_MACSEC_NO_CTRL_FRM_MATCH        = -73,  /**< No control frame match*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_SET_PATTERN    = -74,  /**< Could no set bypass pattern for CP rule*/
    VTSS_RC_ERR_MACSEC_TIMEOUT_ISSUE            = -75,  /**< Internal timeout issue, bailing out*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_EMPTY_EGRESS   = -76,  /**< Could not empty the egress pipeline*/
    VTSS_RC_ERR_MACSEC_AN_NOT_CREATED           = -77,  /**< AN not created.*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_EMPTY_INGRESS  = -78,  /**< Could not empty the ingress pipeline*/
    VTSS_RC_ERR_MACSEC_TX_SC_NOT_EXIST          = -80,  /**< No tx SC found*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_DISABLE_SA     = -81,  /**< Could not disable sa*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_DEL_RX_SA      = -82,  /**< Could not delete rx sa*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_DEL_TX_SA      = -83,  /**< Could not delete tx sa*/
    VTSS_RC_ERR_MACSEC_PATTERN_NOT_SET          = -84,  /**< Pattern not set*/
    VTSS_RC_ERR_MACSEC_HW_RESOURCE_EXHUSTED     = -85,  /**< HW resources exhausted*/
    VTSS_RC_ERR_MACSEC_SCI_ALREADY_EXISTS       = -86,  /**< SCI already exists*/
    VTSS_RC_ERR_MACSEC_SC_RESOURCE_NOT_FOUND    = -87,  /**< Could not find SC resources*/
    VTSS_RC_ERR_MACSEC_RX_AN_ALREADY_IN_USE     = -88,  /**< Rx AN is in use*/ 
    VTSS_RC_ERR_MACSEC_EMPTY_RECORD             = -89,  /**< Could not get an empty record*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_PRG_XFORM      = -90,  /**< Could not program the xform record*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_TOGGLE_SA      = -91,  /**< Could not toggle SA*/
    VTSS_RC_ERR_MACSEC_TX_AN_ALREADY_IN_USE     = -92,  /**< Tx AN is in use*/ 
    VTSS_RC_ERR_MACSEC_ALL_AVAILABLE_SA_IN_USE  = -93,  /**< All available SA's are in use*/
    VTSS_RC_ERR_MACSEC_MATCH_DISABLE            = -94,  /**< MACSEC match disabled*/
    VTSS_RC_ERR_MACSEC_ALL_CP_RULES_IN_USE      = -95,  /**< All CP rules of the specific type are in use*/
    VTSS_RC_ERR_MACSEC_PATTERN_PRIO_NOT_VALID   = -96,  /**< The pattern priority is not valid*/
    VTSS_RC_ERR_MACSEC_BUFFER_TOO_SMALL         = -97,  /**< Buffer to small, must be greater than VTSS_MACSEC_FRAME_CAPTURE_SIZE_MAX*/
    VTSS_RC_ERR_MACSEC_FRAME_TOO_LONG           = -98,  /**< Frame length is supposed to be less than the amount of data in the fifo*/
    VTSS_RC_ERR_MACSEC_FRAME_TRUNCATED          = -99,  /**< Frame is Truncated*/
    VTSS_RC_ERR_MACSEC_PHY_POWERED_DOWN         = -100, /**< Phy is powered down, i.e. the MacSec block is not accessible*/
    VTSS_RC_ERR_MACSEC_PHY_NOT_MACSEC_CAPABLE   = -101, /**< Port/Phy is not MacSec capable*/
    VTSS_RC_ERR_MACSEC_AN_NOT_EXIST             = -102, /**< AN does not exist*/
    VTSS_RC_ERR_MACSEC_NO_PATTERN_CFG           = -103, /**< No pattern is configured*/
    VTSS_RC_ERR_MACSEC_MAX_MTU                  = -105, /**< Maximum MTU allowed is 32761 (+ 4 bytes for VLAN)*/
    VTSS_RC_ERR_MACSEC_UNEXPECT_CP_MODE         = -106, /**< Unexpected CP mode*/
    VTSS_RC_ERR_MACSEC_COULD_NOT_DISABLE_AN     = -107, /**< Could not disable AN*/
    VTSS_RC_ERR_MACSEC_RULE_OUT_OF_RANGE        = -108, /**< Rule id is out of range. Must not be larger than VTSS_MACSEC_CP_RULES*/
    VTSS_RC_ERR_MACSEC_RULE_NOT_EXIST           = -109, /**< Rule does not exist*/
    VTSS_RC_ERR_MACSEC_CSR_READ                 = -110, /**< Could not do CSR read*/
    VTSS_RC_ERR_MACSEC_CSR_WRITE                = -111, /**< Could not do CSR write*/
    VTSS_RC_ERR_PHY_6G_RCPLL_ON_BASE_PORT_ONLY  = -112, /**< PHY API 6G RC-PLL status support only on Base port */


    /**** MISC ERRORS ****/
    VTSS_RC_ERR_INVALID_NULL_PTR                = -200, /**< A pointer was unexpected NULL */
}; // Leave it anonymous.

/****************************************************************************
 * Common types
 ****************************************************************************/
/** \brief Chip number used for targets with multiple chips */
typedef u32 vtss_chip_no_t;

/** \brief Instance identifier */
typedef struct vtss_state_s *vtss_inst_t;

/**
 * \brief Description: Event type.
 * When a variable of this type is used as an input parameter, the API will set the variable if the event has occured.
 * The API will never clear the variable. If is up to the application to clear the variable, when the event has been handled.
 **/
typedef BOOL vtss_event_t;

/** \brief Policer packet rate in PPS */
typedef u32 vtss_packet_rate_t;

#define VTSS_PACKET_RATE_DISABLED 0xffffffff /**< Special value for disabling packet policer */

/** \brief Port Number */
typedef u32 vtss_port_no_t;

/** \brief Physical port number */
typedef u32 vtss_phys_port_no_t;

/**
 * \brief Memory allocation flags.
 *
 * The VTSS API asks the application to
 * allocate dynamic memory for its internal structures
 * through calls to VTSS_OS_MALLOC().
 *
 * The application should normally just associate
 * this with a call to malloc() or kmalloc()
 * depending on the OS and the runtime model (API running
 * in Kernel or User space).
 *
 * However, on some OSs, it's required to allocate
 * specially if the memory is going to be associated
 * with DMA, hence the VTSS_MEM_FLAGS_DMA enumeration.
 *
 * Also, to be able to support warm restart, another
 * enumeration, VTSS_MEM_FLAGS_PERSIST, tells
 * the application to allocate the memory in a part
 * of RAM that won't be affected by a subsequent boot.
 *
 * VTSS_OS_MALLOC() must not block or make waiting points
 * if called with flags != VTSS_MEM_FLAGS_NONE.
 *
 * Each of the enumerations are ORed together to form
 * the final flags that are used in a call to VTSS_OS_MALLOC().
 *
 * The same set of flags are used in calls to VTSS_OS_FREE().
 */
typedef enum {
    VTSS_MEM_FLAGS_NONE    = 0x0, /**< Allocate normally according to runtime model (User or Kernel space). */
    VTSS_MEM_FLAGS_DMA     = 0x1, /**< Allocate memory that can be used with a DMA.                         */
    VTSS_MEM_FLAGS_PERSIST = 0x2, /**< Allocate memory that will survive a warm restart.                    */
} vtss_mem_flags_t;

#define VTSS_PORT_COUNT 1 /**< Default number of ports */

#if defined(VTSS_CHIP_SERVAL_LITE) || defined(VTSS_CHIP_CARACAL_LITE)
#if (VTSS_PORT_COUNT < 7)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 7 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 7 */
#endif /* SERVAL_LITE/CARACAL_LITE */

#if defined(VTSS_CHIP_SPARX_III_10) || defined(VTSS_CHIP_SPARX_III_10_UM) || \
    defined(VTSS_CHIP_SPARX_III_10_01) || defined(VTSS_CHIP_SEVILLE)
#if (VTSS_PORT_COUNT < 10)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 10 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 10 */
#endif /* SPARX_III_10 */

#if defined(VTSS_CHIP_CARACAL_1) || defined(VTSS_CHIP_SERVAL) || defined(VTSS_CHIP_SPARX_III_11)
#if (VTSS_PORT_COUNT < 11)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 11 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 11 */
#endif /* CARACAL_1/SERVAL */

#if defined(VTSS_CHIP_SPARX_III_17_UM)
#if (VTSS_PORT_COUNT < 17)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 17 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 17 */
#endif /* SPARX_III_17_UM */

#if defined(VTSS_CHIP_SPARX_III_18)
#if (VTSS_PORT_COUNT < 18)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 18 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 18 */
#endif /* SPARX_III_18 */

#if defined(VTSS_CHIP_SPARX_III_24)
#if (VTSS_PORT_COUNT < 24)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 24 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 24 */
#endif /* SPARX_III_24 */

#if defined(VTSS_CHIP_SPARX_III_25_UM) 
#if (VTSS_PORT_COUNT < 25)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 25 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 25 */
#endif /* SPARX_III_25_UM */

#if defined(VTSS_CHIP_SPARX_III_26) || defined(VTSS_CHIP_CARACAL_2)
#if (VTSS_PORT_COUNT < 26)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 26 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 26 */
#endif /* SPARX_III_26 */

#if defined(VTSS_CHIP_BARRINGTON_II)
#if (VTSS_PORT_COUNT < 24)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 26 /**< Number of ports incl. host port */
#endif /* VTSS_PORT_COUNT < 24 */
#endif /* VTSS_CHIP_BARRINGTON_II */

#if defined(VTSS_CHIP_SCHAUMBURG_II)
#if (VTSS_PORT_COUNT < 13)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 13 /**< Number of ports incl. host port */
#endif /* VTSS_PORT_COUNT < 13 */
#endif /* VTSS_CHIP_SCHAUMBURG_II */

#if defined(VTSS_CHIP_EXEC_1)
#if (VTSS_PORT_COUNT < 3)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 3 /**< Number of ports incl. host port */
#endif /* VTSS_PORT_COUNT < 2 */
#endif /* VTSS_CHIP_EXEC_1 */

#if defined(VTSS_CHIP_DAYTONA) || defined(VTSS_CHIP_TALLADEGA)
#if (VTSS_PORT_COUNT < 4)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 4 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 2 */
#endif /* VTSS_CHIP_DAYTONA */

/* 12x1G + 4x2.5G + 3x10G + NPI (port mux mode 1) */
#if defined(VTSS_CHIP_LYNX_1)
#if (VTSS_PORT_COUNT < 20)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 20 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 20 */
#endif /* VTSS_CHIP_LYNX_1 */

/* 24x1G + 2x10G + NPI (port mux mode 0) */
#if defined(VTSS_CHIP_E_STAX_III_48)
#if (VTSS_PORT_COUNT < 27)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 27 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 27 */
#endif /* VTSS_CHIP_E_STAX_III_48 */

/* 24x1G + 4x10G + NPI (port mux mode 0) */
#if defined(VTSS_CHIP_E_STAX_III_68)
#if (VTSS_PORT_COUNT < 29)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 29 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 29 */
#endif /* VTSS_CHIP_E_STAX_III_68 */

/* 23x1G + 4x2.5G + 3x10G + NPI (port mux mode 1) */
#if defined(VTSS_CHIP_JAGUAR_1)
#if (VTSS_PORT_COUNT < 31)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 31 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 31 */
#endif /* VTSS_CHIP_JAGUAR_1 */

/* 23x1G + 4x2.5G + NPI on each device (port mux mode 1) */
#if defined(VTSS_CHIP_E_STAX_III_24_DUAL)
#if (VTSS_PORT_COUNT < 56)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 56 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 56 */
#endif /* VTSS_CHIP_E_STAX_III_24_DUAL */

/* 24x1G + 2x10G + NPI on each device (port mux mode 0) */
#if defined(VTSS_CHIP_E_STAX_III_68_DUAL)
#if (VTSS_PORT_COUNT < 54)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 54 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 54 */
#endif /* VTSS_CHIP_E_STAX_III_68_DUAL */

#if defined(VTSS_CHIP_SERVAL_2)
/* 12x1G + 2x10G + NPI */
#if (VTSS_PORT_COUNT < 15)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 15 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 15 */
#endif /* VTSS_CHIP_SERVAL_2 */

#if defined(VTSS_CHIP_LYNX_2) || defined(VTSS_CHIP_SPARX_IV_26)
/* 24x1G + 2x10G + NPI */
#if (VTSS_PORT_COUNT < 27)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 27 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 27 */
#endif /* VTSS_CHIP_LYNX_2/SPARX_IV_26 */

#if defined(VTSS_CHIP_JAGUAR_2) || defined(VTSS_CHIP_SPARX_IV_52) || defined(VTSS_CHIP_SPARX_IV_80) || defined(VTSS_CHIP_SPARX_IV_90)
/* 48x1G + 4x1G + NPI */
#if (VTSS_PORT_COUNT < 53)
#undef VTSS_PORT_COUNT
#define VTSS_PORT_COUNT 53 /**< Number of ports */
#endif /* VTSS_PORT_COUNT < 53 */
#endif /* VTSS_CHIP_JAGUAR_2/SPARX_IV_52/SPARX_IV_80/SPARX_IV_90 */

/* Number of ports may optionally be less than number of chip ports */
#if VTSS_OPT_PORT_COUNT && ((VTSS_PORT_COUNT == 1) || (VTSS_OPT_PORT_COUNT < VTSS_PORT_COUNT))
#define VTSS_PORTS VTSS_OPT_PORT_COUNT /**< Number of ports */
#else
#define VTSS_PORTS VTSS_PORT_COUNT     /**< Number of ports */
#endif /* VTSS_OPT_PORT_COUNT */

/* The first logical port number is 0. */
#define VTSS_PORT_NO_NONE    (0xffffffff) /**< Port number none */
#define VTSS_PORT_NO_CPU     (0xfffffffe) /**< Port number for CPU for special purposes */
#define VTSS_PORT_NO_START   (0)          /**< Port start number */
#define VTSS_PORT_NO_END     (VTSS_PORT_NO_START+VTSS_PORTS) /**< Port end number */
#define VTSS_PORT_ARRAY_SIZE VTSS_PORT_NO_END /**< Port number array size */

#define VTSS_PORT_IS_PORT(x) ((x)<VTSS_PORT_NO_END) /**< Valid port number */

/** \brief The different interfaces for connecting MAC and PHY */
typedef enum
{
    VTSS_PORT_INTERFACE_NO_CONNECTION, /**< No connection */
    VTSS_PORT_INTERFACE_LOOPBACK,      /**< Internal loopback in MAC */
    VTSS_PORT_INTERFACE_INTERNAL,      /**< Internal interface */
    VTSS_PORT_INTERFACE_MII,           /**< MII (RMII does not exist) */
    VTSS_PORT_INTERFACE_GMII,          /**< GMII */
    VTSS_PORT_INTERFACE_RGMII,         /**< RGMII */
    VTSS_PORT_INTERFACE_TBI,           /**< TBI */
    VTSS_PORT_INTERFACE_RTBI,          /**< RTBI */
    VTSS_PORT_INTERFACE_SGMII,         /**< SGMII */
    VTSS_PORT_INTERFACE_SGMII_CISCO,   /**< SGMII using Cisco aneg  */
    VTSS_PORT_INTERFACE_SERDES,        /**< SERDES */
    VTSS_PORT_INTERFACE_VAUI,          /**< VAUI */
    VTSS_PORT_INTERFACE_100FX,         /**< 100FX */
    VTSS_PORT_INTERFACE_XAUI,          /**< XAUI */
    VTSS_PORT_INTERFACE_RXAUI,         /**< RXAUI */
    VTSS_PORT_INTERFACE_XGMII,         /**< XGMII */
    VTSS_PORT_INTERFACE_SPI4,          /**< SPI4 */
    VTSS_PORT_INTERFACE_QSGMII,        /**< QSGMII */
    VTSS_PORT_INTERFACE_SFI,           /**< SFI/LAN */
} vtss_port_interface_t;


/** \brief Auto negotiation struct */
typedef struct
{
    BOOL obey_pause;               /**< This port should obey PAUSE frames */
    BOOL generate_pause;           /**< Link partner obeys PAUSE frames */
} vtss_aneg_t;                     /**< Auto negotiation result */

#if defined(VTSS_FEATURE_PORT_CONTROL) || defined(VTSS_CHIP_10G_PHY)
/** \brief Serdes macro mode */
typedef enum
{
    VTSS_SERDES_MODE_DISABLE,   /**< Disable serdes */
    VTSS_SERDES_MODE_XAUI_12G,  /**< XAUI 12G mode  */
    VTSS_SERDES_MODE_XAUI,      /**< XAUI 10G mode  */
    VTSS_SERDES_MODE_RXAUI,     /**< RXAUI 10G mode */
    VTSS_SERDES_MODE_RXAUI_12G, /**< RXAUI 12G mode */
    VTSS_SERDES_MODE_2G5,       /**< 2.5G mode      */
    VTSS_SERDES_MODE_QSGMII,    /**< QSGMII mode    */
    VTSS_SERDES_MODE_SGMII,     /**< SGMII mode     */
    VTSS_SERDES_MODE_100FX,     /**< 100FX mode     */
    VTSS_SERDES_MODE_1000BaseX, /**< 1000BaseX mode */
    VTSS_SERDES_MODE_SFI,       /**< LAN/10G mode   */
    VTSS_SERDES_MODE_SFI_DAC,   /**< LAN/10G DAC(CU)*/
    VTSS_SERDES_MODE_IDLE,      /**< Send idles     */
} vtss_serdes_mode_t;
#endif //VTSS_FEATURE_PORT_CONTROL

/****************************************************************************
 * QoS types
 ****************************************************************************/
#if defined(VTSS_FEATURE_QOS)

/** \brief Priority number */
typedef u32 vtss_prio_t;
#define VTSS_PRIOS           8                              /**< Number of priorities */
#define VTSS_PRIO_NO_NONE    0xffffffff                     /**< Priority number none (= undefined) */
#define VTSS_PRIO_START      0                              /**< Priority start number (lowest) */
#define VTSS_PRIO_END        (VTSS_PRIO_START + VTSS_PRIOS) /**< Priority end number */
#define VTSS_PRIO_ARRAY_SIZE VTSS_PRIO_END                  /**< Priority number array size */

/** \brief Queue number */
typedef u32 vtss_queue_t;
#define VTSS_QUEUES           VTSS_PRIOS                       /**< Number of queues */
#define VTSS_QUEUE_START      0                                /**< Queue start number */
#define VTSS_QUEUE_END        (VTSS_QUEUE_START + VTSS_QUEUES) /**< Queue end number */
#define VTSS_QUEUE_ARRAY_SIZE VTSS_QUEUE_END                   /**< Queue number array size */

/** \brief Tag Priority or Priority Code Point (PCP) */
typedef u32 vtss_tagprio_t;
#define VTSS_PCPS           8                            /**< Number of PCP values */
#define VTSS_PCP_START      0                            /**< PCP start number */
#define VTSS_PCP_END        (VTSS_PCP_START + VTSS_PCPS) /**< PCP end number */
#define VTSS_PCP_ARRAY_SIZE VTSS_PCP_END                 /**< PCP array size */

/** \brief Drop Eligible Indicator (DEI) */
typedef BOOL vtss_dei_t;
#define VTSS_DEIS           2                            /**< Number of DEI values */
#define VTSS_DEI_START      0                            /**< DEI start number */
#define VTSS_DEI_END        (VTSS_DEI_START + VTSS_DEIS) /**< DEI end number */
#define VTSS_DEI_ARRAY_SIZE VTSS_DEI_END                 /**< DEI array size */

/** \brief Drop Precedence Level (DPL) */
typedef u8 vtss_dp_level_t;
#define VTSS_DPLS 2 /**< Default number of drop precedence levels */

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2)
#undef VTSS_DPLS
#define VTSS_DPLS 4 /**< Number of drop precedence levels */
#endif /* defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2) */

#define VTSS_DPL_START      0                            /**< DPL start number */
#define VTSS_DPL_END        (VTSS_DPL_START + VTSS_DPLS) /**< DPL end number */
#define VTSS_DPL_ARRAY_SIZE VTSS_DPL_END                 /**< DPL array size */

/** \brief Percentage, 0-100 */
typedef u8 vtss_pct_t;

/**
 * \brief Policer/Shaper bit rate in kbps (1000 bits per second).
 * The rate will be rounded to the nearest value supported by the chip
 **/
typedef u32 vtss_bitrate_t;
#define VTSS_BITRATE_DISABLED 0xffffffff /**< Bitrate disabled */

/**
 * \brief Policer/shaper burst level in bytes.
 * The level will be rounded to the nearest value supported by the chip
 **/
typedef u32 vtss_burst_level_t;

/** \brief Storm policer mode configuration */
typedef enum
{
    VTSS_STORM_POLICER_MODE_PORTS_AND_CPU, /**< Police both CPU and front port destinations */
    VTSS_STORM_POLICER_MODE_PORTS_ONLY,    /**< Police front port destinations only */
    VTSS_STORM_POLICER_MODE_CPU_ONLY       /**< Police CPU destination only */
} vtss_storm_policer_mode_t;

/** \brief DSCP value (0-63) */
typedef u8 vtss_dscp_t;

/** \brief QoS Control Entry ID */
typedef u32 vtss_qce_id_t;

#if defined(VTSS_FEATURE_QOS_POLICER_DLB)
/** \brief Dual leaky buckets policer configuration */
typedef enum
{
    VTSS_POLICER_TYPE_MEF,    /**< MEF bandwidth profile */
    VTSS_POLICER_TYPE_SINGLE  /**< Single bucket policer (CIR/CBS) */
} vtss_policer_type_t;
#endif /* VTSS_FEATURE_QOS_POLICER_DLB */

/** \brief EVC policer index */
typedef u16 vtss_evc_policer_id_t;

/** \brief WRED group number */
typedef u32 vtss_wred_group_t;

#endif /* defined(VTSS_FEATURE_QOS) */

/****************************************************************************
 * VLAN types
 ****************************************************************************/

/** \brief VLAN Identifier */
typedef u16 vtss_vid_t; /* 0-4095 */

#define VTSS_VID_NULL     ((const vtss_vid_t)0)     /**< NULL VLAN ID */
#define VTSS_VID_DEFAULT  ((const vtss_vid_t)1)     /**< Default VLAN ID */
#define VTSS_VID_RESERVED ((const vtss_vid_t)0xFFF) /**< Reserved VLAN ID */
#define VTSS_VIDS         ((const vtss_vid_t)4096)  /**< Number of VLAN IDs */
#define VTSS_VID_ALL      ((const vtss_vid_t)0x1000)/**< Untagged VID: All VLAN IDs */

/**
 * \brief VLAN acceptable frame type
 */
typedef enum
{
    VTSS_VLAN_FRAME_ALL,      /**< Accept all frames */
    VTSS_VLAN_FRAME_TAGGED,   /**< Accept tagged frames only */
    VTSS_VLAN_FRAME_UNTAGGED  /**< Accept untagged frames only */
} vtss_vlan_frame_t;

/** \brief Ethernet Type **/
typedef u16 vtss_etype_t;

/**
 * VLAN tag with "arbitrary" TPID.
 */
#if defined(VTSS_FEATURE_QOS)
typedef struct {
    vtss_etype_t   tpid; /**< Tag Protocol Identifier */
    vtss_tagprio_t pcp;  /**< Priority Code Point     */
    BOOL           dei;  /**< Drop Eligible Indicator */
    vtss_vid_t     vid;  /**< VLAN Identifier         */
} vtss_vlan_tag_t;
#endif

#define VTSS_ETYPE_VTSS 0x8880 /**< Vitesse Ethernet Type */

/** \brief MAC Address */
typedef struct
{
    u8 addr[6];   /**< Network byte order */
} vtss_mac_t;

#define VTSS_MAC_ADDR_SZ_BYTES    6                 /**< Number of bytes for representing MAC address (SMAC/DMAC) type */
typedef u8 vtss_mac_addr_t[VTSS_MAC_ADDR_SZ_BYTES]; /**< MAC address (SMAC/DMAC) */

/** \brief MAC Address in specific VLAN */
typedef struct
{
    vtss_vid_t  vid;   /**< VLAN ID */
    vtss_mac_t  mac;   /**< MAC address */
} vtss_vid_mac_t;

#define MAC_ADDR_BROADCAST {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}  /**< Broadcast address used for addr in the vtss_mac_t struct */

/** \brief EVC ID */
typedef u16 vtss_evc_id_t;

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_EVCS        4096   /**< Maximum number of Ethernet Virtual Connections */
#elif defined(VTSS_CHIP_SERVAL)
#define VTSS_EVCS        1024   /**< Maximum number of Ethernet Virtual Connections */
#elif defined(VTSS_ARCH_CARACAL) || defined(VTSS_CHIP_SERVAL_LITE)
#define VTSS_EVCS        256    /**< Maximum number of Ethernet Virtual Connections */
#else
#define VTSS_EVCS        256    /**< Maximum number of Ethernet Virtual Connections */
#endif

typedef u32 vtss_isdx_t;   /**< Ingress Service Index type */
#define VTSS_ISDX_NONE (0) /**< Ingress Service Index number none */

/** \brief Aggregation Number. */
typedef u32 vtss_aggr_no_t;
#define VTSS_AGGRS           (VTSS_PORTS/2) /**< Number of LLAGs */
#define VTSS_AGGR_NO_NONE    0xffffffff     /**< Aggregation number none */
#define VTSS_AGGR_NO_START   0              /**< Aggregation start number */
#define VTSS_AGGR_NO_END     (VTSS_AGGR_NO_START+VTSS_AGGRS) /**< Aggregation number end */

/** \brief Description: GLAG number */
typedef u32 vtss_glag_no_t;

#if defined(VTSS_ARCH_JAGUAR_1)
#define VTSS_GLAGS         32         /**< Number of GLAGs */
#else
#define VTSS_GLAGS         2          /**< Number of GLAGs */
#endif
#define VTSS_GLAG_NO_NONE  0xffffffff /**< GLAG number none */
#define VTSS_GLAG_NO_START 0          /**< GLAG start number */
#define VTSS_GLAG_NO_END   (VTSS_GLAG_NO_START+VTSS_GLAGS) /**< GLAG end number */

/* Maximum 8 ports per GLAG */
#define VTSS_GLAG_PORTS           8 /**< Number of GLAG ports */
#define VTSS_GLAG_PORT_START      0 /**< GLAG port start number */
#define VTSS_GLAG_PORT_END        (VTSS_GLAG_PORT_START+VTSS_GLAG_PORTS) /**< GLAG port end number */
#define VTSS_GLAG_PORT_ARRAY_SIZE VTSS_GLAG_PORT_END /**< GLAG port array size */

/****************************************************************************
 * CPU queue types
 ****************************************************************************/
#if defined(VTSS_FEATURE_MAC_CPU_QUEUE)
/** \brief Description: CPU Rx queue number */
typedef u32 vtss_packet_rx_queue_t;

/** \brief Description: CPU Rx group number
 *  \details This is a value in range [0; VTSS_PACKET_RX_GRP_CNT[.
 */
typedef u32 vtss_packet_rx_grp_t;

/** \brief Description: CPU Tx group number
 *  \details This is a value in range [0; VTSS_PACKET_TX_GRP_CNT[.
 */
typedef u32 vtss_packet_tx_grp_t;

#if defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_SERVAL)
#undef  VTSS_PACKET_RX_QUEUE_CNT
#define VTSS_PACKET_RX_QUEUE_CNT    8  /**< Number of Rx packet queues */
#undef  VTSS_PACKET_RX_GRP_CNT
#define VTSS_PACKET_RX_GRP_CNT      2  /**< Number of Rx packet groups to which any queue can map */
#undef  VTSS_PACKET_TX_GRP_CNT
#define VTSS_PACKET_TX_GRP_CNT      2  /**< Number of Tx packet groups */
#endif /* VTSS_ARCH_LUTON26/SERVAL */

#if defined(VTSS_ARCH_JAGUAR_2)
#undef  VTSS_PACKET_RX_QUEUE_CNT
#define VTSS_PACKET_RX_QUEUE_CNT    8  /**< Number of Rx packet queues */
#undef  VTSS_PACKET_RX_GRP_CNT
#define VTSS_PACKET_RX_GRP_CNT      2  /**< Number of Rx packet groups to which any queue can map */
#undef  VTSS_PACKET_TX_GRP_CNT
#define VTSS_PACKET_TX_GRP_CNT      2  /**< Number of Tx packet groups */
#endif /* VTSS_ARCH_JAGUAR_2 */

#if defined(VTSS_ARCH_JAGUAR_1)
#undef VTSS_PACKET_RX_QUEUE_CNT
#define VTSS_PACKET_RX_QUEUE_CNT   10  /**< Number of Rx packet queues. The last two are only usable as super priority queues. */
#undef  VTSS_PACKET_RX_GRP_CNT
#define VTSS_PACKET_RX_GRP_CNT      4  /**< Number of Rx packet groups to which any queue can map */
#undef  VTSS_PACKET_TX_GRP_CNT
#define VTSS_PACKET_TX_GRP_CNT      5  /**< Number of Tx packet groups */
#endif

#define VTSS_PACKET_RX_QUEUE_NONE  (0xffffffff) /**< Rx queue not selected for a particular type of frames */ 
#define VTSS_PACKET_RX_QUEUE_START (0)          /**< Rx queue start number */
#define VTSS_PACKET_RX_QUEUE_END   (VTSS_PACKET_RX_QUEUE_START + VTSS_PACKET_RX_QUEUE_CNT) /**< Rx queue end number */

#endif /* defined(VTSS_FEATURE_MAC_CPU_QUEUE) */

#if defined(VTSS_FEATURE_PACKET_PORT_REG)
/** \brief Packet registration type */
typedef enum {
    VTSS_PACKET_REG_NORMAL,   /**< Global registration configuration is used */
    VTSS_PACKET_REG_FORWARD,  /**< Forward normally */
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
    VTSS_PACKET_REG_DISCARD,  /**< Discard */
    VTSS_PACKET_REG_CPU_COPY, /**< Copy to CPU */
#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */
    VTSS_PACKET_REG_CPU_ONLY  /**< Redirect to CPU */
} vtss_packet_reg_type_t;

/** \brief Packet registration per port */
typedef struct {
#if defined(VTSS_ARCH_SERVAL)
    vtss_packet_reg_type_t ipmc_ctrl_reg; /**< IP MC Control, NORMAL/FORWARD/CPU_COPY supported */
    vtss_packet_reg_type_t igmp_reg;      /**< IGMP, NORMAL/FORWARD/CPU_ONLY supported */
    vtss_packet_reg_type_t mld_reg;       /**< MLD, NORMAL/FORWARD/CPU_ONLY supported */
#endif /* VTSS_ARCH_SERVAL */
    vtss_packet_reg_type_t bpdu_reg[16];  /**< BPDU range: 01-80-C2-00-00-0X */
    vtss_packet_reg_type_t garp_reg[16];  /**< GARP range: 01-80-C2-00-00-2X */
} vtss_packet_rx_port_conf_t;
#endif /* VTSS_FEATURE_PACKET_PORT_REG */

/** \brief VDD power supply */
typedef enum {
    VTSS_VDD_1V0,               /**< 1.0V (default) */
    VTSS_VDD_1V2,               /**< 1.2V */
} vtss_vdd_t;

/****************************************************************************
 * L3 types
 ****************************************************************************/
/** \brief Description: UDP/TCP port number */
typedef u16 vtss_udp_tcp_t;

/** \brief IPv4 address/mask */
typedef u32 vtss_ip_t;

/** \brief IPv4 address/mask */
typedef vtss_ip_t vtss_ipv4_t;

/** \brief Prefix size */
typedef u32 vtss_prefix_size_t;

/** \brief IPv6 address/mask */
typedef struct
{
    u8 addr[16]; /**< Address */
} vtss_ipv6_t;

/* NOTE: This type may be used directly in SNMP
 * InetAddressType types.  */

/** \brief IP address type */
typedef enum {
    VTSS_IP_TYPE_NONE = 0, /**< Matches "InetAddressType_unknown" */
    VTSS_IP_TYPE_IPV4 = 1, /**< Matches "InetAddressType_ipv4"    */
    VTSS_IP_TYPE_IPV6 = 2, /**< Matches "InetAddressType_ipv6"    */
} vtss_ip_type_t;

/** \brief Either an IPv4 or IPv6 address  */
typedef struct {
    vtss_ip_type_t  type; /**< Union type */
    union {
        vtss_ipv4_t ipv4; /**< IPv4 address */
        vtss_ipv6_t ipv6; /**< IPv6 address */
    } addr;               /**< IP address */
} vtss_ip_addr_t;

/** \brief IPv4 network */
typedef struct
{
    vtss_ipv4_t        address;     /**< Network address */
    vtss_prefix_size_t prefix_size; /**< Prefix size */
} vtss_ipv4_network_t;

/** \brief IPv6 network */
typedef struct
{
    vtss_ipv6_t        address;     /**< Network address */
    vtss_prefix_size_t prefix_size; /**< Prefix size */
} vtss_ipv6_network_t;

/** \brief IPv6 network */
typedef struct
{
    vtss_ip_addr_t     address;     /**< Network address */
    vtss_prefix_size_t prefix_size; /**< Prefix size */
} vtss_ip_network_t;

/** \brief Routing entry type */
typedef enum
{
    VTSS_ROUTING_ENTRY_TYPE_INVALID = 0,
    VTSS_ROUTING_ENTRY_TYPE_IPV6_UC = 1,
    VTSS_ROUTING_ENTRY_TYPE_IPV4_MC = 2,
    VTSS_ROUTING_ENTRY_TYPE_IPV4_UC = 3,
} vtss_routing_entry_type_t;

/** \brief IPv4 unicast routing entry */
typedef struct
{
    vtss_ipv4_network_t network;     /**< Network to route */
    vtss_ipv4_t         destination; /**< IP address of next-hop router.
                                          Zero if local route */
} vtss_ipv4_uc_t;

/** \brief IPv6 routing entry */
typedef struct
{
    vtss_ipv6_network_t network;     /**< Network to route */
    vtss_ipv6_t         destination; /**< IP address of next-hop router.
                                          Zero if local route */
} vtss_ipv6_uc_t;

/** \brief Routing entry */
typedef struct
{
   /** Type of route */
   vtss_routing_entry_type_t type;

   union {
       /** IPv6 unicast route */
       vtss_ipv4_uc_t ipv4_uc;

       /** IPv6 unicast route */
       vtss_ipv6_uc_t ipv6_uc;
   } route; /**< Route */

   /** Link-local addresses needs to specify a egress vlan. */
   vtss_vid_t vlan;
} vtss_routing_entry_t;

/** \brief Routing interface statics counter */
typedef struct {
    u64 ipv4uc_received_octets;    /**< IPv4UC octets received and hardware forwarded */
    u64 ipv4uc_received_frames;    /**< IPv4UC frames received and hardware forwarded */
    u64 ipv6uc_received_octets;    /**< IPv6UC octets received and hardware forwarded */
    u64 ipv6uc_received_frames;    /**< IPv6UC frames received and hardware forwarded */

    u64 ipv4uc_transmitted_octets; /**< IPv4UC octets transmitted */
    u64 ipv4uc_transmitted_frames; /**< IPv4UC frames transmitted */
    u64 ipv6uc_transmitted_octets; /**< IPv6UC octets transmitted */
    u64 ipv6uc_transmitted_frames; /**< IPv6UC frames transmitted */
} vtss_l3_counters_t;

/****************************************************************************
 * VCAP types
 ****************************************************************************/
#if defined(VTSS_FEATURE_VCAP)

/** \brief VCAP 1 bit */
typedef enum
{
    VTSS_VCAP_BIT_ANY, /**< Value 0 or 1 */
    VTSS_VCAP_BIT_0,   /**< Value 0 */
    VTSS_VCAP_BIT_1    /**< Value 1 */
} vtss_vcap_bit_t;

/** \brief VCAP 8 bit value and mask */
typedef struct
{
    u8 value;   /**< Value */
    u8 mask;    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u8_t;

/** \brief VCAP 16 bit value and mask */
typedef struct
{
    u8 value[2];   /**< Value */
    u8 mask[2];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u16_t;

/** \brief VCAP 24 bit value and mask */
typedef struct
{
    u8 value[3];   /**< Value */
    u8 mask[3];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u24_t;

/** \brief VCAP 32 bit value and mask */
typedef struct
{
    u8 value[4];   /**< Value */
    u8 mask[4];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u32_t;

/** \brief VCAP 40 bit value and mask */
typedef struct
{
    u8 value[5];   /**< Value */
    u8 mask[5];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u40_t;

/** \brief VCAP 48 bit value and mask */
typedef struct
{
    u8 value[6];   /**< Value */
    u8 mask[6];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u48_t;

/** \brief VCAP 128 bit value and mask */
typedef struct
{
    u8 value[16];   /**< Value */
    u8 mask[16];    /**< Mask, cleared bits are wildcards */
} vtss_vcap_u128_t;

/** \brief VCAP VLAN ID value and mask */
typedef struct
{
    u16 value;   /**< Value */
    u16 mask;    /**< Mask, cleared bits are wildcards */
} vtss_vcap_vid_t;

/** \brief VCAP IPv4 address value and mask */
typedef struct
{
    vtss_ip_t value;   /**< Value */
    vtss_ip_t mask;    /**< Mask, cleared bits are wildcards */
} vtss_vcap_ip_t;

/** \brief VCAP UDP/TCP port range */
typedef struct
{
    BOOL           in_range;   /**< Port in range match */
    vtss_udp_tcp_t low;        /**< Port low value */
    vtss_udp_tcp_t high;       /**< Port high value */
} vtss_vcap_udp_tcp_t;

/** \brief Value/Range type */
typedef enum
{
    VTSS_VCAP_VR_TYPE_VALUE_MASK,        /**< Used as value/mask */
    VTSS_VCAP_VR_TYPE_RANGE_INCLUSIVE,   /**< Used as inclusive range: low <= range <= high */
    VTSS_VCAP_VR_TYPE_RANGE_EXCLUSIVE    /**< Used as exclusive range: range < low or range > high */
} vtss_vcap_vr_type_t;

/** \brief VCAP universal value or range type */
typedef u16 vtss_vcap_vr_value_t;

/** \brief VCAP universal value or range */
typedef struct
{
    vtss_vcap_vr_type_t type; /**< Type */
    union
    {
        struct
        {
            vtss_vcap_vr_value_t value; /**< Value */
            vtss_vcap_vr_value_t mask;  /**< Mask, cleared bits are wildcards */
        } v; /**< type == VTSS_VCAP_VR_TYPE_VALUE_MASK */
        struct
        {
            vtss_vcap_vr_value_t low;   /**< Low value */
            vtss_vcap_vr_value_t high;  /**< High value */
        } r; /**< type == VTSS_VCAP_VR_TYPE_RANGE_XXXXXX */
    } vr; /**< Value or range */
} vtss_vcap_vr_t;

/** \brief VCAP key type */
typedef enum {
    VTSS_VCAP_KEY_TYPE_NORMAL,     /**< Half key, SIP only */
    VTSS_VCAP_KEY_TYPE_DOUBLE_TAG, /**< Quarter key, two tags */
    VTSS_VCAP_KEY_TYPE_IP_ADDR,    /**< Half key, SIP and DIP */
    VTSS_VCAP_KEY_TYPE_MAC_IP_ADDR /**< Full key, MAC and IP addresses */
} vtss_vcap_key_type_t;
#endif /* VTSS_FEATURE_VCAP */

/****************************************************************************
 * ACL types
 ****************************************************************************/
#if defined(VTSS_FEATURE_ACL)

/** \brief ACL policer number */
typedef u32 vtss_acl_policer_no_t;
#define VTSS_ACL_POLICERS         16                                              /**< Number of ACL policers */
#define VTSS_ACL_POLICER_NO_START 0                                               /**< ACL policer start number */
#define VTSS_ACL_POLICER_NO_END   (VTSS_ACL_POLICER_NO_START + VTSS_ACL_POLICERS) /**< ACL policer end number */

/** \brief ACL policy number */
typedef u32 vtss_acl_policy_no_t;
#define VTSS_ACL_POLICY_NO_NONE  0xffffffff                                     /**< ACLs disabled on port */
#define VTSS_ACL_POLICY_NO_MIN   0                                              /**< ACLs policy minimum number */
#if defined(VTSS_ARCH_LUTON26) || defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_ACL_POLICY_NO_MAX   255                                            /**< ACLs policy maximum number */
#elif defined(VTSS_ARCH_SERVAL)
#define VTSS_ACL_POLICY_NO_MAX   63                                             /**< ACLs policy maximum number */
#else
#define VTSS_ACL_POLICY_NO_MAX   7                                              /**< ACLs policy maximum number */
#endif /* VTSS_ARCH_LUTON26/JAGUAR_1/JAGUAR_2 */
#define VTSS_ACL_POLICIES        (VTSS_ACL_POLICY_NO_MAX + 1)                   /**< Number of ACL policies */
#define VTSS_ACL_POLICY_NO_START  VTSS_ACL_POLICY_NO_MIN                        /**< ACL policy start number */
#define VTSS_ACL_POLICY_NO_END   (VTSS_ACL_POLICY_NO_START + VTSS_ACL_POLICIES) /**< ACL policy end number */

#endif /* defined(VTSS_FEATURE_ACL) */

/****************************************************************************
 * EVC types
 ****************************************************************************/
#if defined(VTSS_FEATURE_EVC)

/** \brief EVC Control Entry (ECE) ID */
typedef u32 vtss_ece_id_t;

/** \brief ECE direction */
typedef enum
{
    VTSS_ECE_DIR_BOTH,        /**< Bidirectional */
    VTSS_ECE_DIR_UNI_TO_NNI,  /**< UNI-to-NNI direction */
    VTSS_ECE_DIR_NNI_TO_UNI   /**< NNI-to-UNI direction */
} vtss_ece_dir_t;

/** \brief Ingress tag popping */
typedef enum
{
    VTSS_ECE_POP_TAG_0, /**< No tag popping */
    VTSS_ECE_POP_TAG_1, /**< Pop one tag */
    VTSS_ECE_POP_TAG_2  /**< Pop two tags (VTSS_ECE_DIR_NNI_TO_UNI only) */
} vtss_ece_pop_tag_t;

#if defined(VTSS_ARCH_SERVAL)
/** \brief ECE rule types */
typedef enum
{
    VTSS_ECE_RULE_BOTH, /**< Ingress and egress rules */
    VTSS_ECE_RULE_RX,   /**< Ingress rules */
    VTSS_ECE_RULE_TX    /**< Egress rules */
} vtss_ece_rule_t;

/** \brief ECE egress lookup types */
typedef enum
{
    VTSS_ECE_TX_LOOKUP_VID,     /**< VID lookup */
    VTSS_ECE_TX_LOOKUP_VID_PCP, /**< (VID, PCP) lookup */
    VTSS_ECE_TX_LOOKUP_ISDX     /**< ISDX lookup */
} vtss_ece_tx_lookup_t;

/** \brief PCP mode */
typedef enum
{
    VTSS_ECE_PCP_MODE_CLASSIFIED, /**< Classified PCP */
    VTSS_ECE_PCP_MODE_FIXED,      /**< Fixed PCP */
    VTSS_ECE_PCP_MODE_MAPPED      /**< PCP based on mapped (QOS, DP) */
} vtss_ece_pcp_mode_t;

/** \brief DEI mode */
typedef enum
{
    VTSS_ECE_DEI_MODE_CLASSIFIED, /**< Classified DEI */
    VTSS_ECE_DEI_MODE_FIXED,      /**< Fixed DEI */
    VTSS_ECE_DEI_MODE_DP          /**< DP-based DEI */
} vtss_ece_dei_mode_t;
#endif /* VTSS_ARCH_SERVAL */

/** \brief Counter */
typedef u64 vtss_counter_t;

/** \brief Counter pair */
typedef struct {
    vtss_counter_t frames; /**< Number of frames */
    vtss_counter_t bytes;  /**< Number of bytes */
} vtss_counter_pair_t;

#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_SERVAL)
/** \brief ECE inner tag type */
typedef enum
{
    VTSS_ECE_INNER_TAG_NONE,    /**< No inner tag */
    VTSS_ECE_INNER_TAG_C,       /**< Inner tag is C-tag */
    VTSS_ECE_INNER_TAG_S,       /**< Inner tag is S-tag */
    VTSS_ECE_INNER_TAG_S_CUSTOM /**< Inner tag is S-custom tag */
} vtss_ece_inner_tag_type_t;

/** \brief EVC/ECE counters */
typedef struct {
    vtss_counter_pair_t rx_green;   /**< Rx green frames/bytes */
    vtss_counter_pair_t rx_yellow;  /**< Rx yellow frames/bytes */
    vtss_counter_pair_t rx_red;     /**< Rx red frames/bytes */
    vtss_counter_pair_t rx_discard; /**< Rx discarded frames/bytes */
    vtss_counter_pair_t tx_discard; /**< Tx discarded frames/bytes */
    vtss_counter_pair_t tx_green;   /**< Tx green frames/bytes */
    vtss_counter_pair_t tx_yellow;  /**< Tx yellow frames/bytes */
} vtss_evc_counters_t;

#endif /* VTSS_ARCH_JAGUAR_1/SERVAL */

#endif /* VTSS_FEATURE_EVC */

#if defined(VTSS_FEATURE_MPLS) && defined(VTSS_FEATURE_EVC) && defined(VTSS_ARCH_SERVAL)
/****************************************************************************
 * MPLS types
 ****************************************************************************/

/** \brief MPLS cross-connect counters */
typedef struct {
    vtss_counter_pair_t rx_green;   /**< Rx green frames/bytes */
    vtss_counter_pair_t rx_yellow;  /**< Rx yellow frames/bytes */
    vtss_counter_pair_t rx_red;     /**< Rx red frames/bytes */
    vtss_counter_pair_t rx_discard; /**< Rx discarded frames/bytes */
    vtss_counter_pair_t tx_discard; /**< Tx discarded frames/bytes */
    vtss_counter_pair_t tx_green;   /**< Tx green frames/bytes */
    vtss_counter_pair_t tx_yellow;  /**< Tx yellow frames/bytes */
} vtss_mpls_xc_counters_t;
#endif /* VTSS_FEATURE_MPLS && VTSS_FEATURE_EVC && VTSS_ARCH_SERVAL */

/****************************************************************************
 * HQoS types
 ****************************************************************************/

/** \brief HQoS entry identifier (HQoS ID) */
typedef u16 vtss_hqos_id_t;

#define VTSS_HQOS_COUNT 256 /**< Maximum number of HQoS entries */

#define VTSS_HQOS_ID_NONE 0xffff /**< Special HQoS ID value */

/** \brief HQoS port scheduling mode
 *
 * The scheduling mode for the port affects which egress QoS options are available.
 * */
typedef enum
{
    VTSS_HQOS_SCH_MODE_NORMAL,      /**< Normal QoS configuration available for non-service traffic only (default) */
    VTSS_HQOS_SCH_MODE_BASIC,       /**< Basic QoS configuration available for non-service traffic only */
    VTSS_HQOS_SCH_MODE_HIERARCHICAL /**< Basic QoS configuration available per HQoS entry (HQoS) */
} vtss_hqos_sch_mode_t;

/****************************************************************************
 * 1588 types
 ****************************************************************************/

/**
 * \brief Clock adjustment rate in parts per billion (ppb) * 1<<16.
 * Range is +-2**47 ppb
 * For example, 8.25 ppb is expressed as 0x0000.0000.0008.4000 
 */
typedef i64 vtss_clk_adj_rate_t;

/**
 * \brief Time interval in ns * 1<<16
 * range +-2**47 ns = 140737 sec = 39 hours
 * For example, 2.5 ns is expressed as 0x0000.0000.0002.8000 
 */
typedef i64 vtss_timeinterval_t;

#define VTSS_ONE_MIA 1000000000 /**< One billion */
#define VTSS_ONE_MILL   1000000 /**< One million */
#define VTSS_MAX_TIMEINTERVAL 0x7fffffffffffffffLL /**< Maximum time interval */

#define VTSS_INTERVAL_SEC(t) ((i32)VTSS_DIV64((t)>>16, VTSS_ONE_MIA)) /**< One Second time interval */
#define VTSS_INTERVAL_MS(t)  ((i32)VTSS_DIV64((t)>>16, VTSS_ONE_MILL)) /**< One millisecond time interval */
#define VTSS_INTERVAL_US(t)  ((i32)VTSS_DIV64((t)>>16, 1000)) /**< One microsecond time interval */
#define VTSS_INTERVAL_NS(t)  ((i32)VTSS_MOD64((t)>>16, VTSS_ONE_MIA)) /**< This returns the ns part of the interval, not the total number of ns */
#define VTSS_INTERVAL_PS(t)  (((((i32)(t & 0xffff))*1000)+0x8000)/0x10000) /**< This returns the ps part of the interval, not the total number of ps */
#define VTSS_SEC_NS_INTERVAL(s,n) (((vtss_timeinterval_t)(n)+(vtss_timeinterval_t)(s)*VTSS_ONE_MIA)<<16)  /**< TBD */

/**
 * \brief Time stamp in seconds and nanoseconds
 */
typedef struct {
    u16 sec_msb; /**< Seconds msb */
    u32 seconds; /**< Seconds */
    u32 nanoseconds; /**< nanoseconds */
} vtss_timestamp_t;

#define VTSS_CLOCK_IDENTITY_LENGTH 8 /**< Length of unique PTP identifier */

/**
 * \brief PTP clock unique identifier
 */
typedef u8 vtss_clock_identity[VTSS_CLOCK_IDENTITY_LENGTH];

/****************************************************************************
 * SYNCE types
 ****************************************************************************/

#if defined(VTSS_ARCH_JAGUAR_2)
#define VTSS_SYNCE_CLK_PORT_ARRAY_SIZE  4    /**< SYNCE clock out port numberarray size */
#else
#define VTSS_SYNCE_CLK_PORT_ARRAY_SIZE  2    /**< SYNCE clock out port numberarray size */
#endif

#ifdef __cplusplus
}
#endif

#endif /* _VTSS_API_TYPES_H_ */
