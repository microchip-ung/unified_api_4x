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
 * \brief Initialization API
 * \details This header file describes functions used to create and initialize targets
 */

#ifndef _VTSS_INIT_API_H_
#define _VTSS_INIT_API_H_

#include <vtss/api/types.h>
#ifdef __cplusplus
extern "C" {
#endif

/** \brief Target chip type */
typedef enum {
    VTSS_TARGET_CU_PHY,                       /**< Cu PHY family */
    VTSS_TARGET_10G_PHY,                      /**< 10G PHY family */
    VTSS_TARGET_SPARX_III_11       = 0x7414,  /**< SparX-III-11 SME switch */
    VTSS_TARGET_SERVAL_LITE        = 0x7416,  /**< Serval Lite CE switch */
    VTSS_TARGET_SERVAL             = 0x7418,  /**< Serval CE switch */
    VTSS_TARGET_SEVILLE            = 0x9953,  /**< Seville switch */
    VTSS_TARGET_SPARX_III_10_UM    = 0x7420,  /**< SparxIII-10 unmanaged switch */
    VTSS_TARGET_SPARX_III_17_UM    = 0x7421,  /**< SparxIII-17 unmanaged switch */
    VTSS_TARGET_SPARX_III_25_UM    = 0x7422,  /**< SparxIII-25 unmanaged switch */
    VTSS_TARGET_CARACAL_LITE       = 0x7423,  /**< Caracal-Lite CE switch */
    VTSS_TARGET_SPARX_III_10       = 0x7424,  /**< SparxIII-10 switch */
    VTSS_TARGET_SPARX_III_18       = 0x7425,  /**< SparxIII-18 switch */
    VTSS_TARGET_SPARX_III_24       = 0x7426,  /**< SparxIII-24 switch */
    VTSS_TARGET_SPARX_III_26       = 0x7427,  /**< SparxIII-26 switch */
    VTSS_TARGET_SPARX_III_10_01    = 0x17424, /**< SparxIII-10-01 switch */
    VTSS_TARGET_CARACAL_1          = 0x7428,  /**< Caracal-1 CE switch */
    VTSS_TARGET_CARACAL_2          = 0x7429,  /**< Caracal-2 CE switch */
    VTSS_TARGET_JAGUAR_1           = 0x7460,  /**< Jaguar-1 CE switch */
    VTSS_TARGET_LYNX_1             = 0x7462,  /**< LynX-1 CE switch */
    VTSS_TARGET_E_STAX_III_48      = 0x7432,  /**< E-StaX-III-48 */
    VTSS_TARGET_E_STAX_III_68      = 0x7434,  /**< E-StaX-III-68 */
    VTSS_TARGET_E_STAX_III_24_DUAL = 0xD7431, /**< Dual E-StaX-III-24 */
    VTSS_TARGET_E_STAX_III_68_DUAL = 0xD7434, /**< Dual E-StaX-III-68 */
    VTSS_TARGET_DAYTONA            = 0x8492,  /**< Daytona FEC OTN Phy */
    VTSS_TARGET_TALLADEGA          = 0x8494,  /**< Talladega FEC OTN Phy */
    VTSS_TARGET_SERVAL_2           = 0x7438,  /**< Serval-2 CE switch */
    VTSS_TARGET_LYNX_2             = 0x7464,  /**< LynX-2 CE switch */
    VTSS_TARGET_JAGUAR_2           = 0x7468,  /**< Jaguar-2 CE switch */
    VTSS_TARGET_SPARX_IV_52        = 0x7442,  /**< Sparx-IV-52 switch */
    VTSS_TARGET_SPARX_IV_44        = 0x7444,  /**< Sparx-IV-44 switch */
    VTSS_TARGET_SPARX_IV_80        = 0x7448,  /**< Sparx-IV-80 switch */
    VTSS_TARGET_SPARX_IV_90        = 0x7449,  /**< Sparx-IV-80 switch */
} vtss_target_type_t;

/** \brief Create structure */
typedef struct {
    vtss_target_type_t target; /**< Target type */
} vtss_inst_create_t;

/** 
 * \brief Initialize create structure for target. 
 *
 * \param target [IN] Target name
 * \param create [IN] Create structure
 *
 * \return Return code.
 **/
vtss_rc vtss_inst_get(const vtss_target_type_t target,
                      vtss_inst_create_t       *const create);

/** 
 * \brief Create target instance.
 * 
 * \param create [IN] Create structure
 * \param inst [OUT]  Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_inst_create(const vtss_inst_create_t *const create,
                         vtss_inst_t              *const inst);

/**
 * \brief Destroy target instance.
 * 
 * \param inst [IN] Target instance reference.
 *
 * \return Return code.
 **/
vtss_rc vtss_inst_destroy(const vtss_inst_t inst);

/** 
 * \brief Register read function 
 *
 * \param chip_no [IN] Chip number, for targets with multiple chips
 * \param addr [IN]    Register address
 * \param value [OUT]  Register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_reg_read_t)(const vtss_chip_no_t chip_no,
                                   const u32            addr,
                                   u32                  *const value);

/** 
 * \brief Register write function 
 *
 * \param chip_no [IN] Chip number, for targets with multiple chips
 * \param addr [IN]    Register address
 * \param value [IN]   Register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_reg_write_t)(const vtss_chip_no_t chip_no,
                                  const u32            addr,
                                  const u32            value);

#define VTSS_I2C_NO_MULTIPLEXER -1 /**< Used to signal not to use internal i2c clock multiplexing */
/** 
 * \brief I2C read function 
 *
 * \param port_no [IN] Port number
 * \param i2c_addr [IN] I2C device address
 * \param addr [IN]   Register address
 * \param data [OUT]  Pointer the register(s) data value.
 * \param cnt [IN]    Number of registers to read
 * \param i2c_clk_sel [IN] If i2c clock multiplexing is supported then this is the i2c mux, else use VTSS_I2C_NO_MULTIPLEXER
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_i2c_read_t)(const vtss_port_no_t port_no, 
                                   const u8 i2c_addr, 
                                   const u8 addr, 
                                   u8 *const data, 
                                   const u8 cnt,
                                   const i8 i2c_clk_sel);

/** 
 * \brief I2C write function 
 *
 * \param port_no [IN] Port number
 * \param i2c_addr [IN] I2C device address
 * \param data [OUT]  Pointer the data to be written.
 * \param cnt [IN]    Number of data bytes to write
 * \param i2c_clk_sel [IN] If i2c clock multiplexing is supported then this is the i2c mux, else use VTSS_I2C_NO_MULTIPLEXER
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_i2c_write_t)(const vtss_port_no_t port_no, 
                                    const u8 i2c_addr, 
                                    u8 *const data, 
                                    const u8 cnt,
                                    const i8 i2c_clk_sel);

/**
 * \brief SPI read/write function
 *
 * \param inst [IN] Vitesse API instance.
 * \param port_no [IN] Port number.
 * \param bitsize [IN] Size (in bytes) of bitstream following this parameter.
 * \param data [IN|OUT] Pointer to the data to be written to SPI Slave, if doing write operation.
 *                      Pointer to the data read from SPI Slave, if doing read operation.
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_spi_read_write_t)(const vtss_inst_t inst,
                                       const vtss_port_no_t port_no,
                                       const u8 bitsize,
                                       u8 *const bitstream);

/**
 * \brief SPI 32 bit read/write function
 *
 * \param inst    [IN] Vitesse API instance.
 * \param port_no [IN] Port number.
 * \param read    [IN] Read/Write.
 * \param dev     [IN] MMD device number.
 * \param reg_num [IN] Register offset.
 * \param data [IN|OUT] Pointer to the data to be written to SPI Slave, if doing write operation.
 *                      Pointer to the data read from SPI Slave, if doing read operation.
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_spi_32bit_read_write_t)(const vtss_inst_t inst,
                                               vtss_port_no_t port_no,
                                               BOOL           read,
                                               u8             dev,
                                               u16            reg_num,
                                               u32            *const data);

/**
 * \brief SPI 64 bit read/write function
 *
 * \param inst    [IN] Vitesse API instance.
 * \param port_no [IN] Port number.
 * \param read    [IN] Read/Write.
 * \param dev     [IN] MMD device number.
 * \param reg_num [IN] Register offset.
 * \param data [IN|OUT] Pointer to the data to be written to SPI Slave, if doing write operation.
 *                      Pointer to the data read from SPI Slave, if doing read operation.
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_spi_64bit_read_write_t)(const vtss_inst_t inst,
                                               vtss_port_no_t port_no,
                                               BOOL           read,
                                               u8             dev,
                                               u16            reg_num,
                                               u64            *const data);

/** 
 * \brief MII management read function (IEEE 802.3 clause 22)
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number
 * \param addr [IN]    Register address (0-31)
 * \param value [OUT]  Register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_miim_read_t)(const vtss_inst_t    inst,
                                    const vtss_port_no_t port_no,
                                    const u8             addr,
                                    u16                  *const value);

/** 
 * \brief MII management write function (IEEE 802.3 clause 22)
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number
 * \param addr [IN]    Register address (0-31)
 * \param value [IN]   Register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_miim_write_t)(const vtss_inst_t    inst,
                                     const vtss_port_no_t port_no,
                                     const u8             addr,
                                     const u16            value);

/** 
 * \brief MMD management read function (IEEE 802.3 clause 45)
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number
 * \param mmd [IN]     MMD address (0-31)
 * \param addr [IN]    Register address (0-65535)
 * \param value [OUT]  Register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_mmd_read_t)(const vtss_inst_t    inst,
                                   const vtss_port_no_t port_no,
                                   const u8             mmd,
                                   const u16            addr,
                                   u16                  *const value);

/** 
 * \brief MMD management read increment function (IEEE 802.3 clause 45)
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number
 * \param mmd [IN]     MMD address (0-31)
 * \param addr [IN]    Start register address (0-65535)
 * \param buf [OUT]    The register values (pointer provided by user)
 * \param count [IN]   Number of register reads (increment register reads)
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_mmd_read_inc_t)(const vtss_inst_t    inst,
                                       const vtss_port_no_t port_no,
                                       const u8             mmd,
                                       const u16            addr,
                                       u16                  *const buf,
                                       u8                   count);

/** 
 * \brief MMD management write function (IEEE 802.3 clause 45)
 *
 * \param inst [IN]    Target instance reference.
 * \param port_no [IN] Port number
 * \param mmd [IN]     MMD address (0-31)
 * \param addr [IN]    Start register address (0-65535)
 * \param buf [IN]     The register value
 *
 * \return Return code.
 **/
typedef vtss_rc (*vtss_mmd_write_t)(const vtss_inst_t    inst,
                                    const vtss_port_no_t port_no,
                                    const u8             mmd,
                                    const u16            addr,
                                    const u16            value);

/** \brief PI data width */
typedef enum {
    VTSS_PI_WIDTH_16 = 0, /* 16 bit (default) */
    VTSS_PI_WIDTH_8       /* 8 bit */
} vtss_pi_width_t;

/** \brief PI configuration */
typedef struct {
#if defined(VTSS_ARCH_LUTON26)
    vtss_pi_width_t width;                  /**< Width */
    BOOL            use_extended_bus_cycle; /**< Use extended bus cycle for slow registers */
#endif /* VTSS_ARCH_LUTON26 */
    u32             cs_wait_ns;             /**< Minimum CS wait time in nanoseconds */
#if defined(VTSS_ARCH_DAYTONA)
    u32             cs_hold_ns;             /**< Data hold time in nanoseconds */
#endif 
} vtss_pi_conf_t;

#if defined(VTSS_FEATURE_PORT_MUX)
/** \brief Port mux configuration */
typedef enum {
#if defined(VTSS_ARCH_LUTON26)
    VTSS_PORT_MUX_MODE_0,       /**< Ports muxed to Serdes blocks: 3xQSGMII, 1x2G5, 1xSGMII */
    VTSS_PORT_MUX_MODE_1,       /**< Ports muxed to Serdes blocks: 2x2G5, 10xSGMII          */
    VTSS_PORT_MUX_MODE_2,       /**< Ports muxed to Serdes blocks: 2xQSGMII, 8xSGMII         */
#endif /* VTSS_ARCH_LUTON26 */
#if defined(VTSS_ARCH_JAGUAR_1)
    VTSS_PORT_MUX_MODE_0,       /**< Ports muxed to Serdes blocks: 24xSGMII, 4x10Gb, 1xRGMII */
    VTSS_PORT_MUX_MODE_1,       /**< Ports muxed to Serdes blocks: 20xSGMII, 4x2G5, 3x10G, 1xRGMII */
    VTSS_PORT_MUX_MODE_7,       /**< Ports muxed to Serdes blocks: 16xSGMII, 8x2G5, 2x10G, 1xRGMII */
#endif /* VTSS_ARCH_JAGUAR_1 */
#if defined(VTSS_ARCH_JAGUAR_2)
    VTSS_PORT_MUX_MODE_AUTO,    /**< Port mux mode autodetected (not possible for speeds close to 80Gb (aggregated)) */
    VTSS_PORT_MUX_MODE_0,       /**< Ports muxed to Serdes blocks: 24x2G5, 4x10Gb, NPI, chip ports 8-23,48-53 */
    VTSS_PORT_MUX_MODE_1,       /**< Ports muxed to Serdes blocks: 32x2G5, 2x10Gb, NPI, chip ports 8-31,48-50 */
    VTSS_PORT_MUX_MODE_2,       /**< Supports 48x1G + 4x10G + NPI (JR2-RevC) */
#endif /* VTSS_ARCH_JAGUAR_2 */
} vtss_port_mux_mode_t;
#endif /* VTSS_FEATURE_PORT_MUX */

#if defined(VTSS_FEATURE_SERDES_MACRO_SETTINGS)
/** \brief Serdes fields */
typedef struct {
    u32        ob_post0;                  /**< Trace length */
    u32        ob_sr;                     /**< Slew Rate */
} serdes_fields_t;

/** \brief Serdes macro configuration */
typedef struct {
    vtss_vdd_t       serdes1g_vdd;        /**< Serdes1g supply */
    vtss_vdd_t       serdes6g_vdd;        /**< Serdes6g supply */
    BOOL             ib_cterm_ena;        /**< AC(0)/DC(1) coupled */
    serdes_fields_t  qsgmii;              /**< Appl/Board specific fields for QSGMII */
} vtss_serdes_macro_conf_t;
#endif /* VTSS_FEATURE_SERDES_MACRO_SETTINGS */

#if defined(VTSS_ARCH_JAGUAR_1_DUAL)
/** \brief dual device interconnect mode */
typedef enum {
    VTSS_DUAL_CONNECT_MODE_0, /**< XAUI_0 and XAUI_1 used for device interconnect */
    VTSS_DUAL_CONNECT_MODE_1  /**< XAUI_2 and XAUI_3 used for device interconnect */
} vtss_dual_connect_mode_t;
#endif /* VTSS_ARCH_JAGUAR_1_DUAL */

#if defined(VTSS_FEATURE_WARM_START)
/** \brief Restart information source */
typedef enum {
    VTSS_RESTART_INFO_SRC_NONE,   /* Restart information not stored in PHYs */
    VTSS_RESTART_INFO_SRC_CU_PHY, /* Restart information stored in 1G PHY */
    VTSS_RESTART_INFO_SRC_10G_PHY /* Restart information stored in 10G PHY */
} vtss_restart_info_src_t;
#endif /* VTSS_FEATURE_WARM_START */

#if defined(VTSS_ARCH_JAGUAR_1)
/** \brief The major modes of QS */
typedef enum { 
    VTSS_QS_MODE_DISABLED, /**< Manual settings are disabled, defaults are used */
    VTSS_QS_MODE_ENABLED   /**< Manual settings are enabled. */  
} vtss_qs_mode_t;

#define VTSS_QS_CONF_MAX 0xFFFFFFF1 /**< Configure QS to max possible value */
#define VTSS_QS_CONF_MIN 0xFFFFFFF2 /**< Configure QS to min value (guaranteed) */

/** \brief Queue System settings */
typedef struct {
    vtss_qs_mode_t mode;             /**< The mode of the queue system - default:VTSS_QS_MODE_DISABLED */
    vtss_pct_t oversubscription;     /**< Queue System oversubscription 0-50%.*/

    /** \brief The settings for each API port and queue in the system */
    struct {
        u32 port_min;                /**< Minumum Guaranteed port buffer. Bytes.        */
        u32 port_max;                /**< Maximum port buffer - Not guaranteed. Bytes.  */
        u32 queue_min[VTSS_PRIOS];   /**< Minumum Guaranteed queue buffer. Bytes.       */
        u32 queue_max[VTSS_PRIOS];   /**< Maximum queue buffer - Not guaranteed. Bytes. */
    } port[VTSS_PORTS]; /**< One configuration per port */

} vtss_qs_conf_t;
#endif /* VTSS_ARCH_JAGUAR_1 */

#if defined(VTSS_ARCH_DAYTONA)
/** \brief Daytona Config modes as defined in product requirement table 7. and target spec*/
typedef enum {
    VTSS_CONFIG_MODE_PSS,
    VTSS_CONFIG_MODE_PSS_W,
    VTSS_CONFIG_MODE_PSOC_A,
    VTSS_CONFIG_MODE_PSOC_WA,
    VTSS_CONFIG_MODE_PSOC_S,
    VTSS_CONFIG_MODE_PSOC_WS,
    VTSS_CONFIG_MODE_PEO_WMWS,
    VTSS_CONFIG_MODE_PEO_WMWA,
    VTSS_CONFIG_MODE_PEE_W,
    VTSS_CONFIG_MODE_PEE_MW,
    VTSS_CONFIG_MODE_XAUI_2M_PEE_MW,
    VTSS_CONFIG_MODE_PEE_P,
    VTSS_CONFIG_MODE_PEE_MP,
    VTSS_CONFIG_MODE_XAUI_2M_PEE_MP,
    VTSS_CONFIG_MODE_PFF_P,
    VTSS_CONFIG_MODE_PEO_WA,
    VTSS_CONFIG_MODE_PEO_MWA,
    VTSS_CONFIG_MODE_PEO_WS,
    VTSS_CONFIG_MODE_PEO_MWS,
    VTSS_CONFIG_MODE_PEO_P2E_20,
    VTSS_CONFIG_MODE_PEO_MP2E_20,
    VTSS_CONFIG_MODE_PEO_P2E_100,
    VTSS_CONFIG_MODE_PEO_P1E_100,
    VTSS_CONFIG_MODE_XAUI_2M_PEO_P1E_100,
    VTSS_CONFIG_MODE_PFO_P2F,
    VTSS_CONFIG_MODE_PGO_2,
    VTSS_CONFIG_MODE_PGO_2SUB,
    VTSS_CONFIG_MODE_PGO_2SUBSMALL,
    VTSS_CONFIG_MODE_PGO_2GEN,
    VTSS_CONFIG_MODE_BOO_2,
    VTSS_CONFIG_MODE_BOO_1E,
    VTSS_CONFIG_MODE_BOO_2E,
    VTSS_CONFIG_MODE_BOO_2F,
    VTSS_CONFIG_MODE_BMX_2,
    VTSS_CONFIG_MODE_BMX_2E,
    VTSS_CONFIG_MODE_BMX_1E,
    VTSS_CONFIG_MODE_BMX_2F,
    VTSS_CONFIG_MODE_BOE_WMWA,
    VTSS_CONFIG_MODE_BOE_WMWS,
    VTSS_CONFIG_MODE_BOE_PMP2E_20,
    VTSS_CONFIG_MODE_BOE_PMP1E_20,
    VTSS_CONFIG_MODE_BOEC_2E,
    VTSS_CONFIG_MODE_BOEC_1E,
    VTSS_CONFIG_MODE_BOG_2,
    VTSS_CONFIG_MODE_BOG_2SUB,
    VTSS_CONFIG_MODE_BOG_W2,
    VTSS_CONFIG_MODE_BOSC_A,
    VTSS_CONFIG_MODE_BOSC_WA,
    VTSS_CONFIG_MODE_BOSC_S,
    VTSS_CONFIG_MODE_BOSC_WS,
    VTSS_CONFIG_MODE_BOFC_2F,
    VTSS_CONFIG_MODE_BO8F,
    VTSS_CONFIG_MODE_TSOC_A,
    VTSS_CONFIG_MODE_TSOC_WA,
    VTSS_CONFIG_MODE_TSOC_S,
    VTSS_CONFIG_MODE_TSOC_WS,
    VTSS_CONFIG_MODE_TEO_PMP_2E,
    VTSS_CONFIG_MODE_TEO_PMP_1E,
    VTSS_CONFIG_MODE_TEO_PP_2E,
    VTSS_CONFIG_MODE_TFO_PP2F,
    VTSS_CONFIG_MODE_TEOC_2E,
    VTSS_CONFIG_MODE_TEOC_1E,
    VTSS_CONFIG_MODE_TFOC_2F,
    VTSS_CONFIG_MODE_TEO_WMWA,
    VTSS_CONFIG_MODE_TEO_WMWS,
    VTSS_CONFIG_MODE_TGO_2,
    VTSS_CONFIG_MODE_TGO_2SUB,
    VTSS_CONFIG_MODE_TGO_2SUBSMALL,
    VTSS_CONFIG_MODE_TGO_W2,
    VTSS_CONFIG_MODE_T8FO,
    VTSS_CONFIG_MODE_TOO_2,
    VTSS_CONFIG_MODE_TOO_2E2,
    VTSS_CONFIG_MODE_TOO_1E2,
    VTSS_CONFIG_MODE_TOO_2F2,
    VTSS_CONFIG_MODE_TEE_L,
    VTSS_CONFIG_MODE_TEE_PMP_S,
    VTSS_CONFIG_MODE_TEE_PMP_A,
    VTSS_CONFIG_MODE_TEE_PMW,
    VTSS_CONFIG_MODE_TEE_PP,
    VTSS_CONFIG_MODE_TEE_W,
    VTSS_CONFIG_MODE_TFF,
    VTSS_CONFIG_MODE_TSS,
    VTSS_CONFIG_MODE_PEE_PP,
    VTSS_CONFIG_MODE_PEE_PP_S,
    VTSS_CONFIG_MODE_TEO_PMWS,
    VTSS_CONFIG_MODE_TEE_PP_S,
    VTSS_CONFIG_MODE_DISABLED,
    VTSS_CONFIG_MODE_MAX
} vtss_config_mode_t;

/** \brief EV initialization configuration */
typedef struct {
    vtss_config_mode_t mode;        /**< Mode to be configured */
    u32                xfi_clk;     /**< xfi clock frequency 161Mhz or default*/
    u32                x4_case;     /**< X4 frequency case 1 or 2 */
}vtss_ev_conf_t;
#endif /* VTSS_ARCH_DAYTONA */
/** \brief Initialization configuration. */
typedef struct {
    /* Register access function are not used for VTSS_TARGET_CU_PHY */
    vtss_reg_read_t   reg_read;     /**< Register read function */
    vtss_reg_write_t  reg_write;    /**< Register write function */

    /* MII management access must be setup for VTSS_TARGET_CU_PHY */
    vtss_miim_read_t  miim_read;    /**< MII management read function */
    vtss_miim_write_t miim_write;   /**< MII management write function */ 

    /* MMD management access must be setup for 10G phys */
    vtss_mmd_read_t     mmd_read;     /**< MMD management read function */
    vtss_mmd_read_inc_t mmd_read_inc; /**< MMD management read increment function */
    vtss_mmd_write_t    mmd_write;    /**< MMD management write function */

    vtss_spi_read_write_t   spi_read_write;/**< Board specific SPI read/write callout function */

    vtss_spi_32bit_read_write_t   spi_32bit_read_write;/**< Board specific SPI read/write callout function for 32 bit data */
    vtss_spi_64bit_read_write_t   spi_64bit_read_write;/**< Board specific SPI read/write callout function for 64 bit data*/

#if defined(VTSS_FEATURE_WARM_START)
    BOOL                    warm_start_enable; /**< Allow warm start */
    vtss_restart_info_src_t restart_info_src;  /**< Source of restart information */
    vtss_port_no_t          restart_info_port; /**< Port used to store PHY restart information */
#endif /* VTSS_FEATURE_WARM_START */

#if defined(VTSS_FEATURE_PORT_MUX)
    vtss_port_mux_mode_t mux_mode;         /**< Mux mode (port connection to Serdes Macroes) */
#endif /* VTSS_FEATURE_PORT_MUX */
#if defined(VTSS_ARCH_JAGUAR_1_DUAL)
    vtss_port_mux_mode_t     mux_mode_2;        /**< Mux mode for secondary device */
    vtss_dual_connect_mode_t dual_connect_mode; /**< Device interconnect mode */
#endif /* VTSS_ARCH_JAGUAR_1_DUAL */
    vtss_pi_conf_t    pi;           /**< Parallel Interface configuration */

#if defined(VTSS_FEATURE_SERDES_MACRO_SETTINGS)
    vtss_serdes_macro_conf_t serdes; /**< Serdes macro configuration */ 
#endif /* VTSS_FEATURE_SERDES_MACRO_SETTINGS */
#if defined(VTSS_ARCH_DAYTONA)
    vtss_config_mode_t  conf_mode[VTSS_DAYTONA_CHANNELS];    /**< Chip configuration mode pr channel 0..xx*/
    u32                 two_lane_upi; /**< 0 => four lane operation, 1 => DDR_A, 2 => DDR_K, 3 => DDR_M two lane operation */
    u32                 xfi_clk[VTSS_DAYTONA_CHANNELS];/**< To accept 161Mz as line frequency */
    u32                 x4_case[VTSS_DAYTONA_CHANNELS];/**< To accept X4 frequency case */
#endif /* VTSS_ARCH_DAYTONA */
#if defined(VTSS_ARCH_JAGUAR_1)
    vtss_qs_conf_t      qs_conf; /**< Queue system configuration*/
#endif /* VTSS_ARCH_JAGUAR_1 */
} vtss_init_conf_t;


/** 
 * \brief Get default initialization configuration
 *
 * \param inst [IN]  Target instance reference
 * \param conf [OUT] Initialization configuration
 *
 * \return Return code.
 **/
vtss_rc vtss_init_conf_get(const vtss_inst_t inst,
                           vtss_init_conf_t  *const conf);

/** 
 * \brief Set initialization configuration.
 *
 * \param inst [IN] Target instance reference
 * \param conf [IN] Initialization configuration
 *
 * \return Return code.
 **/
vtss_rc vtss_init_conf_set(const vtss_inst_t      inst,
                           const vtss_init_conf_t *const conf);

#if defined(VTSS_FEATURE_WARM_START)
/** 
 * \brief Indicate configuration end. 
 * If a warm start has been done, the stored configuration will be applied.
 *
 * \param inst [IN] Target instance reference
 *
 * \return Return code.
 **/
vtss_rc vtss_restart_conf_end(const vtss_inst_t inst);

/** \brief API version */
typedef u16 vtss_version_t;

/** \brief Restart type */
typedef enum {
    VTSS_RESTART_COLD, /**< Cold: Chip and CPU restart, e.g. power cycling */
    VTSS_RESTART_COOL, /**< Cool: Chip and CPU restart done by CPU */
    VTSS_RESTART_WARM  /**< Warm: CPU restart only */
} vtss_restart_t;

/** \brief Restart status */
typedef struct {
    vtss_restart_t restart;      /**< Previous restart mode */
    vtss_version_t prev_version; /**< Previous API version */
    vtss_version_t cur_version;  /**< Current API version */
} vtss_restart_status_t;

/** 
 * \brief Get restart status
 *
 * \param inst [IN]    Target instance reference
 * \param status [OUT] Restart status
 *
 * \return Return code.
 **/
vtss_rc vtss_restart_status_get(const vtss_inst_t inst,
                                vtss_restart_status_t *const status);

/** 
 * \brief Get restart configuration (next restart mode)
 *
 * \param inst [IN]     Target instance reference
 * \param restart [OUT] Restart mode
 *
 * \return Return code.
 **/
vtss_rc vtss_restart_conf_get(const vtss_inst_t inst,
                              vtss_restart_t *const restart);

/** 
 * \brief Set restart configuration (next restart mode)
 *
 * \param inst [IN]    Target instance reference
 * \param restart [IN] Restart mode
 *
 * \return Return code.
 **/
vtss_rc vtss_restart_conf_set(const vtss_inst_t inst,
                              const vtss_restart_t restart);

#endif /* VTSS_FEATURE_WARM_START */

#if defined(VTSS_ARCH_JAGUAR_1)
/** 
 * \brief Configure the Queue System
 *
 * \param inst [IN]    Target instance reference
 * \param qs   [IN]    The configuration of the queue system
 *
 * \return Return code.
 **/
vtss_rc vtss_qs_conf_set(const vtss_inst_t inst, 
                         const vtss_qs_conf_t *const qs);

/** 
 * \brief Get the configuration of the Queue System
 *
 * \param inst [IN]    Target instance reference
 * \param qs   [OUT]   The configuration of the queue system
 *
 * \return Return code.
 **/
vtss_rc vtss_qs_conf_get(const vtss_inst_t inst, 
                         vtss_qs_conf_t *const qs);
#endif /* VTSS_ARCH_JAGUAR_1 */


#ifdef __cplusplus
}
#endif
#endif /* _VTSS_INIT_API_H_ */
