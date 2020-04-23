#ifndef _VTSS_SEVILLE_REGS_VCAP_CORE_H_
#define _VTSS_SEVILLE_REGS_VCAP_CORE_H_

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

#include "vtss_seville_regs_common.h"

/*********************************************************************** 
 *
 * Target: \a VCAP_CORE
 *
 * \see vtss_target_VCAP_CORE_e
 *
 * VCAP ES0 Configuration
 *
 ***********************************************************************/

/**
 * Register Group: \a VCAP_CORE:VCAP_CORE_CFG
 *
 * VCAP operations
 */


/** 
 * \brief Initiation of read/write/move/initialization operations
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CFG:VCAP_UPDATE_CTRL
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL(target)  VTSS_IOREG(target,0x0)

/** 
 * \brief
 * Specifies the operation that will be performed when
 * VCAP_UPDATE_CTRL.UPDATE_SHOT is set. For all of the operations it is
 * possible to disable read/write of entries, actions, and/or counter by
 * setting VCAP_UPDATE_CTRL.UPDATE_ENTRY_DIS,
 * VCAP_UPDATE_CTRL.UPDATE_ACTION_DIS, and/or
 * VCAP_UPDATE_CTRL.UPDATE_CNT_DIS respectively. Writing/moving to
 * unimplemented addresses are ignored. Reading/moving from unimplemented
 * addresses returns never-match for entries, and zeros from
 * actions/counters.
 * 
 * For version 2 VCAPs: Active rules may only be written to empty
 * (initialized) entries; that is software must not overwrite one active
 * entry with a new entry. To empty one or more active rules; use the init
 * operation with VCAP_UPDATE_CTRL.UPDATE_ACTION_DIS,
 * VCAP_UPDATE_CTRL.UPDATE_CNT_DIS, and VCAP_UPDATE_CTRL.CLEAR_CACHE bits
 * set to '1'. Move operations automatically empty enties; so it is ok when
 * source and destination ranges overlap. Version 1 VCAPs does not restict
 * overwriting of active rules.
 * 
 * Write operation writes the contents of the cache to the VCAP address
 * specified by VCAP_UPDATE_CTRL.UPDATE_ADDR.
 * 
 * Read opeation reads from the VCAP address VCAP_UPDATE_CTRL.UPDATE_ADDR
 * into the cache.
 * 
 * Move up opeation moves one or more VCAP addresses from a high address to
 * a lower address, this is equivalent to decreasing priority of a rule.
 * The starting address is specified by VCAP_UPDATE_CTRL.UPDATE_ADDR, the
 * number of addresses (the range) that is moved is defined by
 * VCAP_MV_CFG.MV_SIZE, the distance to move is defined by VCAP_MV_CFG.
 * MV_NUM_POS. After moving an address the entry will be cleared (set to
 * never-match.) For version 2 VCAPs: When the counter-move is enabled then
 * counter-values are guaranteed to be consistent. When counter-move is
 * disabled then move operation is faster and destination counter-value
 * will be set to zero. For version 1 VCAPs; see
 * VCAP_UPDATE_CTRL.MV_TRAFFIC_IGN for more information.
 * 
 * Move down opeation moves one or more VCAP addresses from a low address
 * to a higer address, this is equivalent to increasing priority of a rule.
 * This operation is equivalent to "Move up" except for the direction that
 * it moves addresses, see "Move up" for more details.
 * 
 * Init operation writes the contents of the cache to one or more VCAP
 * addresses. The starting address is specified by
 * VCAP_UPDATE_CTRL.UPDATE_ADDR, the number of addresses (the range) that
 * is written is defined by VCAP_MV_CFG.MV_SIZE. Setting
 * VCAP_UPDATE_CTRL.CLEAR_CACHE at the same time as setting the operation
 * will clear the cache and cause the init operation to empty the range of
 * addresses.
 *
 * \details 
 * 000: Write from cache to VCAP
 * 001: Read from VCAP to cache
 * 010: Move entry and/or action up (decreasing addresses)
 * 011: Move entry and/or action down (increasing addresses)
 * 100: Initialize VCAP with the cache-value
 *
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_CMD
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_CMD(x)  VTSS_ENCODE_BITFIELD(x,22,3)
#define  VTSS_M_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_CMD     VTSS_ENCODE_BITMASK(22,3)
#define  VTSS_X_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_CMD(x)  VTSS_EXTRACT_BITFIELD(x,22,3)

/** 
 * \brief
 * Set to disable update of entries for VCAP operations: For
 * read-operations entry-cache will remain unchanged. For write/move/init
 * operations the VCAP-entry will remain unchanged.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_ENTRY_DIS
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_ENTRY_DIS  VTSS_BIT(21)

/** 
 * \brief
 * Set to disable update of actions for VCAP operations: For
 * read-operations action-cache will remain unchanged. For write/move/init
 * operations the VCAP-action will remain unchanged.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_ACTION_DIS
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_ACTION_DIS  VTSS_BIT(20)

/** 
 * \brief
 * Set to disable update of counter for VCAP operations: For
 * read-operations counter-cache will remain unchanged. For write/init
 * operations the VCAP-counter will remain unchanged. For move operations
 * the destination VCAP-counters will be set to zeros.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_CNT_DIS
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_CNT_DIS  VTSS_BIT(19)

/** 
 * \brief
 * The address to access for VCAP operations. For VCAPs that has defaults;
 * the default actions are placed after the normal actions, each default
 * action takes up one complete full-word.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_ADDR
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_ADDR(x)  VTSS_ENCODE_BITFIELD(x,3,16)
#define  VTSS_M_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_ADDR     VTSS_ENCODE_BITMASK(3,16)
#define  VTSS_X_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_ADDR(x)  VTSS_EXTRACT_BITFIELD(x,3,16)

/** 
 * \brief
 * Set to initiate the opeation specified in VCAP_UPDATE_CTRL.UPDATE_CMD.
 * This bit is automatically cleared by hardware when the operation is
 * finished.
 * Software must not change write fields in the VCAP target while this
 * field is set (while operation is active.)
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . UPDATE_SHOT
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_UPDATE_SHOT  VTSS_BIT(2)

/** 
 * \brief
 * Set to clear the cache. This field is cleared immediately by hardware
 * (at the same time as clearing the cache).
 * For version 1 VCAPs the entry/mask is set to match-0 encoding. Action
 * and counter will be set to zeros.
 * For version 2 VCAPs the entry/mask is set to match-off endcoding. Action
 * and counter will be set to zeros.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . CLEAR_CACHE
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_CLEAR_CACHE  VTSS_BIT(1)

/** 
 * \brief
 * For version 1 VCAPs: Set to ignore interrupting traffic during move
 * operations, this will increase speed of the move operations but
 * counter-events may be lost for the VCAP addresses that are moved. When
 * this field is cleared, then interrupting traffic will cause a restart of
 * the move operation (to ensure consistent counter values) and becasue of
 * this, move operations on a heavily loaded device may take a long time to
 * finish.
 * This field is not used for version 2 VCAPs, and value is don't care.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL . MV_TRAFFIC_IGN
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_UPDATE_CTRL_MV_TRAFFIC_IGN  VTSS_BIT(0)


/** 
 * \brief Configuration of move/initialization
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CFG:VCAP_MV_CFG
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG(target)  VTSS_IOREG(target,0x1)

/** 
 * \brief
 * Specifies the distance to move moved during move operations. I.e. if
 * this field is set to 4 for a move-down operation, then source address n
 * is moved to destination address n+5.
 *
 * \details 
 * 0: Distance is one position
 * 1: Distance is two positions
 * n: Distance is n+1 positions
 *
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG . MV_NUM_POS
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_NUM_POS(x)  VTSS_ENCODE_BITFIELD(x,16,16)
#define  VTSS_M_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_NUM_POS     VTSS_ENCODE_BITMASK(16,16)
#define  VTSS_X_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_NUM_POS(x)  VTSS_EXTRACT_BITFIELD(x,16,16)

/** 
 * \brief
 * Specifies the number of addresses to moved/initialize during  move/init
 * operations.
 *
 * \details 
 * 0: Address VCAP_UPDATE_CTRL.UPDATE_ADDR is moved/initialized
 * 1: Addresses VCAP_UPDATE_CTRL.UPDATE_ADDR and
 * VCAP_UPDATE_CTRL.UPDATE_ADDR+1 are moved/initialized
 * n: Addresses VCAP_UPDATE_CTRL.UPDATE_ADDR through
 * VCAP_UPDATE_CTRL.UPDATE_ADDR+n are moved/initialized
 *
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG . MV_SIZE
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_SIZE(x)  VTSS_ENCODE_BITFIELD(x,0,16)
#define  VTSS_M_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_SIZE     VTSS_ENCODE_BITMASK(0,16)
#define  VTSS_X_VCAP_CORE_VCAP_CORE_CFG_VCAP_MV_CFG_MV_SIZE(x)  VTSS_EXTRACT_BITFIELD(x,0,16)

/**
 * Register Group: \a VCAP_CORE:VCAP_CORE_CACHE
 *
 * VCAP cache
 */


/** 
 * \brief Entry data cache
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CACHE:VCAP_ENTRY_DAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 * @param ri Register: VCAP_ENTRY_DAT (??), 0-63
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CACHE_VCAP_ENTRY_DAT(target,ri)  VTSS_IOREG(target,0x2 + (ri))


/** 
 * \brief Entry mask cache
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CACHE:VCAP_MASK_DAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 * @param ri Register: VCAP_MASK_DAT (??), 0-63
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CACHE_VCAP_MASK_DAT(target,ri)  VTSS_IOREG(target,0x42 + (ri))


/** 
 * \brief Action cache
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CACHE:VCAP_ACTION_DAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 * @param ri Register: VCAP_ACTION_DAT (??), 0-63
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CACHE_VCAP_ACTION_DAT(target,ri)  VTSS_IOREG(target,0x82 + (ri))


/** 
 * \brief Counter cache
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CACHE:VCAP_CNT_DAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 * @param ri Register: VCAP_CNT_DAT (??), 0-31
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CACHE_VCAP_CNT_DAT(target,ri)  VTSS_IOREG(target,0xc2 + (ri))


/** 
 * \brief Entry type-group
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_CACHE:VCAP_TG_DAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CORE_CACHE_VCAP_TG_DAT(target)  VTSS_IOREG(target,0xe2)

/**
 * Register Group: \a VCAP_CORE:VCAP_CORE_STICKY
 *
 * Sticky bits
 */


/** 
 * \brief Debug information
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CORE_STICKY:VCAP_STICKY
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CORE_STICKY_VCAP_STICKY(target)  VTSS_IOREG(target,0xe5)

/** 
 * \brief
 * A move operation has resulted in deleting of one or more rules.
 * This field applies only to version 1 VCAPs, for version 2 VCAPs it is
 * not implemented and reading it will return zero.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_VCAP_CORE_STICKY_VCAP_STICKY . VCAP_ROW_DELETED_STICKY
 */
#define  VTSS_F_VCAP_CORE_VCAP_CORE_STICKY_VCAP_STICKY_VCAP_ROW_DELETED_STICKY  VTSS_BIT(0)

/**
 * Register Group: \a VCAP_CORE:VCAP_CONST
 *
 * VCAP constants
 */


/** 
 * \brief VCAP version
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:VCAP_VER
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_VCAP_VER(target)  VTSS_IOREG(target,0xe6)


/** 
 * \brief Entry width
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ENTRY_WIDTH
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ENTRY_WIDTH(target)  VTSS_IOREG(target,0xe7)


/** 
 * \brief Number of full-word entries
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ENTRY_CNT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ENTRY_CNT(target)  VTSS_IOREG(target,0xe8)


/** 
 * \brief Number of subwords per full-word entry
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ENTRY_SWCNT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ENTRY_SWCNT(target)  VTSS_IOREG(target,0xe9)


/** 
 * \brief Entry type-group width
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ENTRY_TG_WIDTH
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ENTRY_TG_WIDTH(target)  VTSS_IOREG(target,0xea)


/** 
 * \brief Number of defaults
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ACTION_DEF_CNT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ACTION_DEF_CNT(target)  VTSS_IOREG(target,0xeb)


/** 
 * \brief Action width
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:ACTION_WIDTH
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_ACTION_WIDTH(target)  VTSS_IOREG(target,0xec)


/** 
 * \brief Counter width
 *
 * \details
 * Register: \a VCAP_CORE:VCAP_CONST:CNT_WIDTH
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_VCAP_CONST_CNT_WIDTH(target)  VTSS_IOREG(target,0xed)

/**
 * Register Group: \a VCAP_CORE:TCAM_BIST
 *
 * Build in test for TCAM
 */


/** 
 * \brief Control of the TCAM
 *
 * \details
 * Register: \a VCAP_CORE:TCAM_BIST:TCAM_CTRL
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_TCAM_BIST_TCAM_CTRL(target)  VTSS_IOREG(target,0xf0)

/** 
 * \brief
 * Set this field to start manual initialization of the TCAM. This field is
 * cleared once initialization is complete.
 * The TCAM has random contents after reset and must be initialized prior
 * to usage.
 *
 * \details 
 * Field: ::VTSS_VCAP_CORE_TCAM_BIST_TCAM_CTRL . TCAM_INIT
 */
#define  VTSS_F_VCAP_CORE_TCAM_BIST_TCAM_CTRL_TCAM_INIT  VTSS_BIT(0)


/** 
 * \brief Status for the TCAM
 *
 * \details
 * Register: \a VCAP_CORE:TCAM_BIST:TCAM_STAT
 *
 * @param target A \a ::vtss_target_VCAP_CORE_e target
 */
#define VTSS_VCAP_CORE_TCAM_BIST_TCAM_STAT(target)  VTSS_IOREG(target,0xf2)

/** 
 * \brief
 * Indicates the current operational state of the TCAM.
 *
 * \details 
 * '0': Busy with initialization.
 * '1': Ready to be used.
 *
 * Field: ::VTSS_VCAP_CORE_TCAM_BIST_TCAM_STAT . TCAM_RDY
 */
#define  VTSS_F_VCAP_CORE_TCAM_BIST_TCAM_STAT_TCAM_RDY  VTSS_BIT(0)


#endif /* _VTSS_SEVILLE_REGS_VCAP_CORE_H_ */