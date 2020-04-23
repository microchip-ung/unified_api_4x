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

#ifndef _VTSS_JAGUAR_REG_H_
#define _VTSS_JAGUAR_REG_H_

#include "vtss_jaguar_regs.h"


/* Commands for Mac Table Command register */
#define MAC_CMD_LEARN           0  /* Insert (Learn) 1 entry */
#define MAC_CMD_UNLEARN         1  /* Unlearn (Forget) 1 entry */
#define MAC_CMD_LOOKUP          2  /* Look up 1 entry */
#define MAC_CMD_READ            3  /* Read entry at Mac Table Index */
#define MAC_CMD_WRITE           4  /* Write entry at Mac Table Index */
#define MAC_CMD_SCAN            5  /* Scan (Age or find next) */
#define MAC_CMD_FIND_SMALLEST   6  /* Get next entry */
#define MAC_CMD_CLEAR_ALL       7  /* Delete all entries in table */

/* Commands for MAC_ENTRY_ADDR_TYPE */

#define  MAC_ENTRY_ADDR_TYPE_UPSID_PN           0
#define  MAC_ENTRY_ADDR_TYPE_UPSID_CPU_OR_INT   1
#define  MAC_ENTRY_ADDR_TYPE_GLAG               2
#define  MAC_ENTRY_ADDR_TYPE_MC_IDX             3
#define  MAC_ENTRY_ADDR_TYPE_CONID_FWD          4
#define  MAC_ENTRY_ADDR_TYPE_PATHGRP_FWD        5

/* Commands for MAC_ENTRY_ADDR_TYPE */
#define  MAC_ENTRY_TYPE_STD_ENTRY        0
#define  MAC_ENTRY_TYPE_IPV4_MC_ENTRY    1
#define  MAC_ENTRY_TYPE_IPV6_MC_ENTRY    2
#define  MAC_ENTRY_TYPE_PATH_CCM_MEPID   3

/* IS0 Entry types */
#define JR_IS0_TYPE_ISID     0
#define JR_IS0_TYPE_DBL_VID  1
#define JR_IS0_TYPE_MPLS     2
#define JR_IS0_TYPE_MAC_ADDR 3

/* IS1 Entry types */
#define JR_IS1_TYPE_VLAN_PAG 0
#define JR_IS1_TYPE_QOS      1

/* IS2 entry types */
#define IS2_ENTRY_TYPE_ETYPE    0
#define IS2_ENTRY_TYPE_LLC      1
#define IS2_ENTRY_TYPE_SNAP     2
#define IS2_ENTRY_TYPE_ARP      3
#define IS2_ENTRY_TYPE_UDP_TCP  4
#define IS2_ENTRY_TYPE_IP_OTHER 5
#define IS2_ENTRY_TYPE_IPV6     6
#define IS2_ENTRY_TYPE_OAM      7
#define IS2_ENTRY_TYPE_CUSTOM_0 8
#define IS2_ENTRY_TYPE_CUSTOM_1 9
#define IS2_ENTRY_TYPE_CUSTOM_2 10
#define IS2_ENTRY_TYPE_CUSTOM_3 11
#define IS2_ENTRY_TYPE_CUSTOM_4 12
#define IS2_ENTRY_TYPE_CUSTOM_5 13
#define IS2_ENTRY_TYPE_CUSTOM_6 14
#define IS2_ENTRY_TYPE_CUSTOM_7 15

/* IS2 lookup types */
#define IS2_LOOKUP_NONE   0
#define IS2_LOOKUP_FIRST  1
#define IS2_LOOKUP_SECOND 2
#define IS2_LOOKUP_BOTH   3

/* ES0 entry types */
#define JR_ES0_TYPE_ISDX 0
#define JR_ES0_TYPE_VID  1

/* ES0 action types */
#define JR_ES0_ACTION_MACINMAC 0
#define JR_ES0_ACTION_TAG      1


#endif /* _JAGUAR_REG_H */

