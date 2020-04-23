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

#define VTSS_TRACE_GROUP VTSS_TRACE_GROUP_L2
#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_common.h"

#if defined(VTSS_FEATURE_LAYER2)

/* Determine port membership considering aggregations etc. */
static void vtss_pgid_members_get(vtss_state_t *vtss_state,
                                  u32 pgid, BOOL member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_pgid_entry_t *pgid_entry;
    vtss_port_eps_t   *protect;
    vtss_port_no_t    port_no, port;
    vtss_aggr_no_t    aggr_no;
    vtss_dgroup_no_t  dgroup_no;

    /* Store raw port members */
    pgid_entry = &vtss_state->l2.pgid_table[pgid];
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++)
        member[port_no] = (port_no < vtss_state->port_count ? pgid_entry->member[port_no] : 0);

    /* Reserved entries are used direcly (e.g. GLAG masks) */
    if (pgid_entry->resv)
        return;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        /* Check if 1+1 protected port is member */
        protect = &vtss_state->l2.port_protect[port_no];
        if ((port = protect->conf.port_no) != VTSS_PORT_NO_NONE &&
            protect->conf.type == VTSS_EPS_PORT_1_PLUS_1) {
            if (member[port])    /* Include working port if protection port is member */
                member[port_no] = 1;
            if (member[port_no]) /* Include protection port if working port is member */
                member[port] = 1;
        }

        /* Check if aggregated ports or destination group members are port members */
        aggr_no = vtss_state->l2.port_aggr_no[port_no];
        dgroup_no = vtss_state->l2.dgroup_port_conf[port_no].dgroup_no;
        for (port = VTSS_PORT_NO_START; port < vtss_state->port_count; port++) {
            if (member[port] &&
                ((aggr_no != VTSS_AGGR_NO_NONE && vtss_state->l2.port_aggr_no[port] == aggr_no) ||
                 vtss_state->l2.dgroup_port_conf[port].dgroup_no == dgroup_no)) {
                member[port_no] = 1;
            }
        }
    }
#if defined VTSS_FEATURE_VSTAX_V2
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if ((aggr_no = vtss_state->l2.port_glag_no[port_no]) == VTSS_GLAG_NO_NONE)
            continue;
        for (port = VTSS_PORT_NO_START; port < vtss_state->port_count; port++)
            if (vtss_state->l2.port_glag_no[port] == aggr_no && member[port])
                member[port_no] = 1;
    }
#endif       
}

/* - MAC address table --------------------------------------------- */

void vtss_mach_macl_get(const vtss_vid_mac_t *vid_mac, u32 *mach, u32 *macl)
{
    *mach = ((vid_mac->vid<<16) | (vid_mac->mac.addr[0]<<8) | vid_mac->mac.addr[1]);
    *macl = ((vid_mac->mac.addr[2]<<24) | (vid_mac->mac.addr[3]<<16) |
             (vid_mac->mac.addr[4]<<8) | vid_mac->mac.addr[5]);
}

void vtss_mach_macl_set(vtss_vid_mac_t *vid_mac, u32 mach, u32 macl)
{
    vid_mac->vid = ((mach >> 16) & 0xfff);
    vid_mac->mac.addr[0] = ((mach >> 8) & 0xff);
    vid_mac->mac.addr[1] = ((mach >> 0) & 0xff);
    vid_mac->mac.addr[2] = ((macl >> 24) & 0xff);
    vid_mac->mac.addr[3] = ((macl >> 16) & 0xff);
    vid_mac->mac.addr[4] = ((macl >> 8) & 0xff);
    vid_mac->mac.addr[5] = ((macl >> 0) & 0xff);
}

static vtss_mac_entry_t *vtss_mac_entry_get(vtss_state_t *vtss_state,
                                            u32 mach, u32 macl, BOOL next)
{
    u32              first,last,mid;
    vtss_mac_entry_t *cur,*old;

    /* Check if table is empty */
    if (vtss_state->l2.mac_ptr_count == 0)
        return NULL;

    /* Locate page using binary search */
    for (first = 0, last = vtss_state->l2.mac_ptr_count-1; first != last; ) {
        mid = (first + last)/2;
        cur = vtss_state->l2.mac_list_ptr[mid];
        if (cur->mach > mach || (cur->mach == mach && cur->macl >= macl)) {
            /* Entry greater or equal, search lower half */
            last = mid;
        } else {
            /* Entry is smaller, search upper half */
            first = mid + 1;
        }
    }

    cur = vtss_state->l2.mac_list_ptr[first];
    /* Go back one page if entry greater */
    if (first != 0 && (cur->mach > mach || (cur->mach == mach && cur->macl > macl)))
        cur = vtss_state->l2.mac_list_ptr[first-1];

    /* Look for greater entry using linear search */
    for (old = NULL; cur != NULL; old = cur, cur = cur->next)
        if (cur->mach > mach || (cur->mach == mach && cur->macl > macl))
            break;

    return (next ? cur : old);
}

/* Update MAC table page pointers */
static void vtss_mac_pages_update(vtss_state_t *vtss_state)
{
    u32              i,count;
    vtss_mac_entry_t *cur;

    for (i = 0, cur = vtss_state->l2.mac_list_used; i < VTSS_MAC_PTR_SIZE && cur!=NULL ; i++) {
        vtss_state->l2.mac_list_ptr[i] = cur;

        /* Move one page forward */
        for (count = 0; count != VTSS_MAC_PAGE_SIZE && cur != NULL ; cur = cur->next, count++)
            ;
    }
    vtss_state->l2.mac_ptr_count = i;
}

/* Add MAC table entry */
static vtss_mac_entry_t *vtss_mac_entry_add(vtss_state_t *vtss_state,
                                            const vtss_mac_user_t user, 
                                            const vtss_vid_mac_t *vid_mac)
{
    u32              mach, macl;
    vtss_mac_entry_t *cur, *tmp;

    /* Calculate MACH and MACL */
    vtss_mach_macl_get(vid_mac, &mach, &macl);

    /* Look for previous or existing entry in used list */
    if ((tmp = vtss_mac_entry_get(vtss_state, mach, macl, 0)) != NULL &&
        tmp->mach == mach && tmp->macl == macl)
        return (tmp->user == user ? tmp : NULL);

    /* Allocate entry from free list */
    if ((cur = vtss_state->l2.mac_list_free) == NULL) {
        VTSS_E("no free MAC entries");
        return NULL;
    }
    vtss_state->l2.mac_list_free = cur->next;
    cur->mach = mach;
    cur->macl = macl;
    cur->user = user;

    if (tmp == NULL) {
        /* Insert first */
        cur->next = vtss_state->l2.mac_list_used;
        vtss_state->l2.mac_list_used = cur;
    } else {
        /* Insert after previous entry */
        cur->next = tmp->next;
        tmp->next = cur;
    }
    vtss_state->l2.mac_table_count++;

    /* Update page pointers */
    vtss_mac_pages_update(vtss_state);

    return cur;
}

/* Delete MAC table entry */
static vtss_rc vtss_mac_entry_del(vtss_state_t *vtss_state,
                                  const vtss_mac_user_t user, const vtss_vid_mac_t *vid_mac)
{
    u32              mach, macl;
    vtss_mac_entry_t *cur, *old;

    /* Calculate MACH and MACL */
    vtss_mach_macl_get(vid_mac, &mach, &macl);

    /* Look for entry */
    for (old = NULL, cur = vtss_state->l2.mac_list_used; cur != NULL; old = cur, cur = cur->next) {
        if (cur->mach == mach && cur->macl == macl) {
            if (cur->user != user) {
                /* Deleting entries added by other users is not allowed */
                return VTSS_RC_ERROR;
            }

            /* Remove from used list */
            if (old == NULL)
                vtss_state->l2.mac_list_used = cur->next;
            else
                old->next = cur->next;

            /* Insert in free list */
            cur->next = vtss_state->l2.mac_list_free;
            vtss_state->l2.mac_list_free = cur;
            vtss_state->l2.mac_table_count--;
            
            /* Update page pointers */
            vtss_mac_pages_update(vtss_state);
            break;
        }
    }
    
    return VTSS_RC_OK;
}

static vtss_rc vtss_mac_entry_update(vtss_state_t *vtss_state, vtss_mac_entry_t *cur, u32 pgid)
{
    vtss_mac_table_entry_t entry;

    vtss_mach_macl_set(&entry.vid_mac, cur->mach, cur->macl);
    entry.locked = 1;
    entry.aged = 0;
    entry.copy_to_cpu = cur->cpu_copy;
    return VTSS_FUNC(l2.mac_table_add, &entry, pgid);
}

static BOOL vtss_ipmc_mac(vtss_state_t *vtss_state, const vtss_vid_mac_t *vid_mac)
{
    /* The JR2 architecture treats IPMC entries like other entries */
    return (vtss_state->arch == VTSS_ARCH_JR2 ? 0 :
            (VTSS_MAC_IPV4_MC(vid_mac->mac.addr) || VTSS_MAC_IPV6_MC(vid_mac->mac.addr)));
}

/* Update IPv4 and IPv6 multicast entries on aggregation changes */
static vtss_rc vtss_mac_table_update(vtss_state_t *vtss_state)
{
    u32               pgid = VTSS_PGID_NONE;
    vtss_pgid_entry_t *pgid_entry = &vtss_state->l2.pgid_table[pgid];
    vtss_mac_entry_t  *cur;
    vtss_port_no_t    port_no;
    vtss_vid_mac_t    vid_mac;      

    for (cur = vtss_state->l2.mac_list_used; cur != NULL; cur = cur->next) {
        vtss_mach_macl_set(&vid_mac, cur->mach, cur->macl);
        if (!vtss_ipmc_mac(vtss_state, &vid_mac))
            continue;
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            pgid_entry->member[port_no] = VTSS_PORT_BF_GET(cur->member, port_no);
        vtss_pgid_members_get(vtss_state, pgid, pgid_entry->member);
        (void)vtss_mac_entry_update(vtss_state, cur, pgid);
    }

    return VTSS_RC_OK;
}

/* Write PGID member to chip */
static vtss_rc vtss_pgid_table_write(vtss_state_t *vtss_state, u32 pgid)
{
    BOOL member[VTSS_PORT_ARRAY_SIZE];

    VTSS_N("pgid: %u", pgid);

    /* Ignore unused entries */
    if (vtss_state->l2.pgid_table[pgid].references == 0)
        return VTSS_RC_OK;

    /* Get port members */
    vtss_pgid_members_get(vtss_state, pgid, member);

    /* Update PGID table */
    return VTSS_FUNC(l2.pgid_table_write, pgid, member);
}

/* Allocate PGID */
static vtss_rc vtss_pgid_alloc(vtss_state_t *vtss_state, 
                               u32 *new, const BOOL member[VTSS_PORT_ARRAY_SIZE],
                               BOOL cpu_copy, vtss_packet_rx_queue_t cpu_queue)
{
    u32               pgid, pgid_free = *new;
    BOOL              pgid_found = 0;
    vtss_pgid_entry_t *pgid_entry;
    vtss_port_no_t    port_no;

    VTSS_D("enter");

    /* Search for matching or unused entry in PGID table */
    for (pgid = 0; pgid < vtss_state->l2.pgid_count; pgid++)  {
        pgid_entry = &vtss_state->l2.pgid_table[pgid];
        if (pgid_entry->resv) /* Skip reserved entries */
            continue;

        if (pgid_entry->references == 0) {
            /* Check if the first unused entry is found or the pgid can be reused */
            if (pgid_free == VTSS_PGID_NONE || pgid == *new) {
                pgid_found = 1;
                pgid_free = pgid;
            }
        } else {
            /* Check if an existing entry matches */
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
                if (member[port_no] != pgid_entry->member[port_no])
                    break;
            if (port_no == vtss_state->port_count && 
                pgid_entry->cpu_copy == cpu_copy && pgid_entry->cpu_queue == cpu_queue) {
                VTSS_D("reusing pgid: %u", pgid);
                *new = pgid;
                pgid_entry->references++;
                return VTSS_RC_OK;
            }
        }
    }

    /* No pgid found */
    if (pgid_found == 0) {
        VTSS_E("no more pgids");
        return VTSS_RC_ERROR;
    }

    VTSS_D("using pgid: %u", pgid_free);
    *new = pgid_free;
    pgid_entry = &vtss_state->l2.pgid_table[pgid_free];
    pgid_entry->references = 1;
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
        pgid_entry->member[port_no] = member[port_no];
    pgid_entry->cpu_copy = cpu_copy;
    pgid_entry->cpu_queue = cpu_queue;
    return vtss_pgid_table_write(vtss_state, pgid_free);
}

/* Free PGID */
static vtss_rc vtss_pgid_free(vtss_state_t *vtss_state, u32 pgid)
{
    vtss_pgid_entry_t *pgid_entry;

    VTSS_D("pgid: %u", pgid);

    /* Ignore IPv4/IPv6 MC free */
    if (pgid == VTSS_PGID_NONE)
        return VTSS_RC_OK;

    if (pgid > vtss_state->l2.pgid_count) {
        VTSS_E("illegal pgid: %u", pgid);
        return VTSS_RC_ERROR;
    }

    /* Do not free reserved PGIDs */
    pgid_entry = &vtss_state->l2.pgid_table[pgid];
    if (pgid_entry->resv)
        return VTSS_RC_OK;

    /* Check if pgid already free */
    if (pgid_entry->references == 0) {
        VTSS_E("pgid: %u already free", pgid);
        return VTSS_RC_ERROR;
    }

    pgid_entry->references--;
    return VTSS_RC_OK;
}

/* Determine logical port */
static vtss_port_no_t vtss_aggr_port(vtss_state_t *vtss_state, vtss_port_no_t port)
{
    vtss_port_no_t port_no;
    vtss_aggr_no_t aggr_no;
    vtss_port_no_t lport_no = port; /* Use port itself by default */

    if ((aggr_no = vtss_state->l2.port_aggr_no[port]) != VTSS_AGGR_NO_NONE) {
        /* Aggregated, use first operational port in aggregation */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            if (vtss_state->l2.port_state[port_no] &&
                vtss_state->l2.port_aggr_no[port_no] == aggr_no) {
                lport_no = port_no;
                break;
            }
    }
#if defined(VTSS_FEATURE_AGGR_GLAG)
    if ((aggr_no = vtss_state->l2.port_glag_no[port]) != VTSS_AGGR_NO_NONE) {
        /* Aggregated, use first operational port in aggregation */
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            if (vtss_state->l2.port_state[port_no] &&
                vtss_state->l2.port_glag_no[port_no] == aggr_no) {
                lport_no = port_no;
                break;
            }
    }
#endif
    return lport_no;
}

/* Update source, destination and aggregation masks */
vtss_rc vtss_update_masks(vtss_state_t *vtss_state,
                          BOOL src_update, BOOL dest_update, BOOL aggr_update)
{
    vtss_rc             rc;
    vtss_port_no_t      i_port, e_port, port_p;
    vtss_aggr_no_t      aggr_no;
    BOOL                member[VTSS_PORT_ARRAY_SIZE], learn[VTSS_PORT_ARRAY_SIZE];
    BOOL                rx_forward[VTSS_PORT_ARRAY_SIZE], tx_forward[VTSS_PORT_ARRAY_SIZE];
    u32                 pgid;
    u32                 port_count = vtss_state->port_count;
    u32                 ac, aggr_count[VTSS_PORT_ARRAY_SIZE];
    u32                 aggr_index[VTSS_PORT_ARRAY_SIZE], n;
    vtss_port_eps_t     *protect;
    vtss_chip_no_t      chip_no = vtss_state->chip_no;
#if defined(VTSS_FEATURE_AGGR_GLAG)
    vtss_glag_no_t      glag_no;
    vtss_port_no_t      port_glag_index[VTSS_PORT_ARRAY_SIZE];
    u32                 glag_port_count[VTSS_GLAGS];

    memset(port_glag_index, 0, sizeof(port_glag_index)); /* Please Lint */
#endif /* VTSS_FEATURE_AGGR_GLAG */

    VTSS_N("enter");

    if (vtss_state->warm_start_cur) {
        VTSS_D("warm start, returning");
        return VTSS_RC_OK;
    }

    /* Determine learning, Rx and Tx forwarding state per port */
    for (i_port = VTSS_PORT_NO_START; i_port < port_count; i_port++) {
        protect = &vtss_state->l2.port_protect[i_port];

        /* Learning */
        learn[i_port] = (vtss_state->l2.port_state[i_port] &&
                         vtss_state->l2.stp_state[i_port] != VTSS_STP_STATE_DISCARDING &&
                         VTSS_PORT_RX_FORWARDING(vtss_state->port.forward[i_port]) &&
                         vtss_state->l2.auth_state[i_port] == VTSS_AUTH_STATE_BOTH &&
                         (protect->conf.port_no == VTSS_PORT_NO_NONE ||
                          protect->conf.type == VTSS_EPS_PORT_1_FOR_1 ||
                          protect->selector == VTSS_EPS_SELECTOR_WORKING));

#if defined(VTSS_FEATURE_VSTAX_V2)
        if (learn[i_port]) {
            u8 ttl;
            // Possibly discard all frames received on a stack port.
            if (i_port == vtss_state->l2.vstax_conf.port_0 ||
                i_port == vtss_state->l2.vstax_conf.port_1) {
                vtss_vstax_chip_info_t *chip_info = &vtss_state->l2.vstax_info.chip_info[VTSS_CHIP_NO(i_port)];

                if (i_port == vtss_state->l2.vstax_conf.port_0 || vtss_state->chip_count == 2) {
                    // If we're updating stack port 0 or we're on a two-chip solution, pick
                    // index 0 of port_conf[], because on two-chip solutions, external stack
                    // ports are always stack port A (stack port B are interconnects).
                    ttl = chip_info->port_conf[0].ttl;
                } else {
                    ttl = chip_info->port_conf[1].ttl;
                }

                // Rx discard when TTL is set to 0.
                learn[i_port] = ttl != 0;
            }
        }
#endif /* defined(VTSS_FEATURE_VSTAX_V2) */

        /* Rx forwarding */
        rx_forward[i_port] = (learn[i_port] &&
                              vtss_state->l2.stp_state[i_port] == VTSS_STP_STATE_FORWARDING);

        /* Tx forwarding */
        tx_forward[i_port] = (vtss_state->l2.port_state[i_port] &&
                              vtss_state->l2.stp_state[i_port] == VTSS_STP_STATE_FORWARDING &&
                              VTSS_PORT_TX_FORWARDING(vtss_state->port.forward[i_port]) &&
                              vtss_state->l2.auth_state[i_port] != VTSS_AUTH_STATE_NONE);
        vtss_state->l2.tx_forward[i_port] = tx_forward[i_port];
    }

    /* Determine state for protection ports */
    for (i_port = VTSS_PORT_NO_START; i_port < port_count; i_port++) {
        protect = &vtss_state->l2.port_protect[i_port];
        port_p = protect->conf.port_no;

        /* If 1+1 working port is active, discard on Rx */
        if (port_p != VTSS_PORT_NO_NONE &&
            protect->conf.type == VTSS_EPS_PORT_1_PLUS_1 &&
            protect->selector == VTSS_EPS_SELECTOR_WORKING) {
            learn[port_p] = 0;
            rx_forward[port_p] = 0;
        }
    }

    /* Update learn mask */
    if (src_update && (rc = VTSS_FUNC(l2.learn_state_set, learn)) != VTSS_RC_OK)
        return rc;

    /* Update source masks */
    for (i_port = VTSS_PORT_NO_START; src_update && i_port < port_count; i_port++) {
        /* Exclude all ports by default */
        memset(member, 0, sizeof(member));

        /* Store Rx forward information */
        vtss_state->l2.rx_forward[i_port] = rx_forward[i_port];

        /* Check if ingress forwarding is allowed */
        if (rx_forward[i_port]) {
#if defined(VTSS_FEATURE_PVLAN)
            vtss_pvlan_no_t     pvlan_no;

            /* Include members of the same PVLAN */
            for (pvlan_no = VTSS_PVLAN_NO_START; pvlan_no < VTSS_PVLAN_NO_END; pvlan_no++) {
                if (vtss_state->l2.pvlan_table[pvlan_no].member[i_port] == 0)
                    continue;
                /* The ingress port is a member of this PVLAN */
                for (e_port = VTSS_PORT_NO_START; e_port < port_count; e_port++)
                    if (vtss_state->l2.pvlan_table[pvlan_no].member[e_port])
                        member[e_port] = 1; /* Egress port also member */
            }
#else
            /* Include all members if PVLANs are not supported */
            for (e_port = VTSS_PORT_NO_START; e_port < port_count; e_port++)
                member[e_port] = 1; /* Egress port also member */
#endif /* VTSS_FEATURE_PVLAN */

            /* Exclude protection port if it exists */
            if ((port_p = vtss_state->l2.port_protect[i_port].conf.port_no) != VTSS_PORT_NO_NONE)
                member[port_p] = 0;

            member[i_port] = 0;
            aggr_no = vtss_state->l2.port_aggr_no[i_port];
            for (e_port = VTSS_PORT_NO_START; e_port < port_count; e_port++) {
                /* Exclude members of the same aggregation */
                if (aggr_no != VTSS_AGGR_NO_NONE &&
                    vtss_state->l2.port_aggr_no[e_port] == aggr_no)
                    member[e_port] = 0;

                /* Exclude working port if it exists */
                if (vtss_state->l2.port_protect[e_port].conf.port_no == i_port)
                    member[e_port] = 0;
                VTSS_N("i_port: %u %sforwarding to e_port %u",
                       i_port, member[e_port] ? "" : "NOT ", e_port);

                /* Exclude ports, which are not egress forwarding */
                if (!VTSS_PORT_TX_FORWARDING(vtss_state->port.forward[e_port]))
                    member[e_port] = 0;
                
#if defined(VTSS_FEATURE_PVLAN)
                /* Exclude ports not allowed by assymmetric PVLANs */
                if (!vtss_state->l2.apvlan_table[i_port][e_port])
                    member[e_port] = 0;
#endif /* VTSS_FEATURE_PVLAN */
            }
        }
        if ((rc = VTSS_FUNC(l2.src_table_write, i_port, member)) != VTSS_RC_OK)
            return rc;
    } /* src_update */

    /* Update PGID table (destination masks) */
#if defined(VTSS_FEATURE_AGGR_GLAG)
    /* Setup GLAG masks */
    if (aggr_update)
        dest_update = 1;
    for (glag_no = VTSS_GLAG_NO_START; dest_update && glag_no < VTSS_GLAG_NO_END; glag_no++) {
        vtss_port_no_t port_no;
        u32            glag_idx, gport, gcount=0; 

        glag_idx = (glag_no - VTSS_GLAG_NO_START);
#if defined(VTSS_FEATURE_VSTAX_V2)
        if (vtss_state->arch == VTSS_ARCH_JR1) {
            BOOL               glag_src[VTSS_PORTS];
            vtss_glag_conf_t   *glag_conf;
            u32                glag_members, glag_indx;

            /* By default, all ports are included in the GLAG source mask */
            for (port_no = VTSS_PORT_NO_START; port_no < port_count; port_no++) {
                glag_src[port_no] = 1;
            }

            /* Determine number of Glag members  */
            glag_members = 0;
            for (gport = VTSS_GLAG_PORT_START; gport < VTSS_GLAG_PORT_END; gport++, glag_members++) {
                if (vtss_state->l2.glag_conf[glag_idx][gport].entry.upspn == VTSS_UPSPN_NONE) 
                    break;
            }                

            /* Calculate number of active ports and GLAG source mask */
            gcount = 0;
            for (gport = VTSS_GLAG_PORT_START; gport < VTSS_GLAG_PORT_END; gport++) {
                glag_conf = &vtss_state->l2.glag_conf[glag_idx][gport];
                if (glag_conf->entry.upspn == VTSS_UPSPN_NONE) {
                    /* End of list */
                    break;
                }
                if (glag_conf->port_no != VTSS_PORT_NO_NONE) {
                    /* Local port */
                    if (!tx_forward[glag_conf->port_no]) {
                        /* Skip local ports not in forwarding state */
                        continue;
                    }

                    /* Exclude from GLAG source mask */
                    glag_src[glag_conf->port_no] = 0;
                    
                    /* Count number of forwarding ports and assign index */
                    VTSS_D("glag_no: %u, index: %u, port_no: %u", 
                           glag_no, gcount, glag_conf->port_no);
                    port_glag_index[glag_conf->port_no] = gcount;
                }
                
                /* Find the glag index that matches the aggregation mask index */
                glag_indx = (glag_members - gcount) % glag_members;

                /* Setup GLAG port entry */ 
                /* Note that the GLAG PGID table and the aggregation mask are AND'ed together to find a forwarding mask */
                /* and must therefore match for unicast entries. Therefore the 'glag_indx' is used in the below function */
                VTSS_FUNC_RC(l2.glag_port_write, glag_no, glag_indx, &glag_conf->entry);
                gcount++;
            }
            
            VTSS_FUNC_RC(l2.glag_src_table_write, glag_no, gcount, glag_src);
        }
#endif /* VTSS_FEATURE_VSTAX_V2 */        
        glag_port_count[glag_idx] = gcount;        
    } /* GLAG loop */
#endif /* VTSS_FEATURE_AGGR_GLAG */
    if (dest_update) {
        for (pgid = 0; pgid < vtss_state->l2.pgid_count; pgid++) {
            VTSS_RC(vtss_pgid_table_write(vtss_state, pgid));
        }
        /* Update destination masks encoded in MAC address table */
        VTSS_RC(vtss_mac_table_update(vtss_state));
    } /* dest_update */

    /* Update aggregation masks */
    if (aggr_update) {

        /* Count number of operational ports and index of each port */
        for (i_port = VTSS_PORT_NO_START; i_port < port_count; i_port++) {
            aggr_count[i_port] = 0;
            aggr_index[i_port] = 0;

            /* If port is not forwarding, continue */
            if (!tx_forward[i_port])
                continue;

            aggr_no = vtss_state->l2.port_aggr_no[i_port];
            VTSS_D("port_no: %u, aggr_no: %u is forwarding", i_port, aggr_no);

            if (aggr_no == VTSS_AGGR_NO_NONE) {
                /* Not aggregated */
                aggr_count[i_port]++;
                continue;
            }
            for (e_port = VTSS_PORT_NO_START; e_port < port_count; e_port++) {
                if (tx_forward[e_port] && vtss_state->l2.port_aggr_no[e_port] == aggr_no) {
                    /* Port is forwarding and member of the same aggregation */
                    aggr_count[i_port]++;
                    if (e_port < i_port)
                        aggr_index[i_port]++;
                }
            }
        }

        for (ac = 0; ac < vtss_state->l2.ac_count; ac++) {
            /* Include one forwarding port from each aggregation */
            for (i_port = VTSS_PORT_NO_START; i_port < port_count; i_port++) {
                n = (aggr_index[i_port] + ac);
                member[i_port] = (aggr_count[i_port] != 0 && (n % aggr_count[i_port]) == 0);

#if defined(VTSS_FEATURE_AGGR_GLAG)
                /* Exclude GLAG member port if not AC owner */
                glag_no = vtss_state->l2.port_glag_no[i_port];
                if (member[i_port] &&
                    glag_no != VTSS_GLAG_NO_NONE &&
                    ((port_glag_index[i_port] + ac) %
                     glag_port_count[glag_no - VTSS_GLAG_NO_START]) != 0)
                    member[i_port] = 0;
#endif /* VTSS_FEATURE_AGGR_GLAG */
                if (ac == 0)
                    /* Store Tx forward information for the first aggregation code */
                    vtss_state->l2.tx_forward_aggr[i_port] = member[i_port];
            }
            /* Write to aggregation table */
            if ((rc = VTSS_FUNC(l2.aggr_table_write, ac, member)) != VTSS_RC_OK)
                return rc;
        }

        /* Update port map table on aggregation changes */
        for (i_port = VTSS_PORT_NO_START; i_port < port_count; i_port++) {
            if ((rc = VTSS_FUNC(l2.pmap_table_write, i_port, vtss_aggr_port(vtss_state, i_port))) != VTSS_RC_OK)
                return rc;
        }
    } /* aggr_update */

    /* Restore chip number in case we were called from a port specific function,
       which had selected the chip number based on the port map. */
    VTSS_SELECT_CHIP(chip_no);

    return VTSS_RC_OK;
}

vtss_rc vtss_mac_add(vtss_state_t *vtss_state,
                     vtss_mac_user_t user, const vtss_mac_table_entry_t *const entry)
{
    u32                    pgid, count = 0, port_count = vtss_state->port_count;
    vtss_mac_table_entry_t old_entry;
    vtss_mac_entry_t       *mac_entry;
    vtss_port_no_t         port_no;
    BOOL                   ipmc, member[VTSS_PORT_ARRAY_SIZE];
    vtss_vid_mac_t         vid_mac;
    vtss_pgid_entry_t      *pgid_entry;
    BOOL                   cpu_copy = 0, vstax = 0;
    vtss_packet_rx_queue_t cpu_queue = 0;

    vid_mac = entry->vid_mac;
    VTSS_D("vid: %d, mac: %02x-%02x-%02x-%02x-%02x-%02x",
           vid_mac.vid,
           vid_mac.mac.addr[0], vid_mac.mac.addr[1], vid_mac.mac.addr[2],
           vid_mac.mac.addr[3], vid_mac.mac.addr[4], vid_mac.mac.addr[5]);

    ipmc = vtss_ipmc_mac(vtss_state, &vid_mac);

#if defined(VTSS_FEATURE_MAC_CPU_QUEUE)
    if (entry->copy_to_cpu) {
        cpu_copy = 1;
        cpu_queue = entry->cpu_queue;
    }
#endif /* VTSS_FEATURE_MAC_CPU_QUEUE */

    if (entry->locked) {
        /* Add all locked entries to state block */
        if ((mac_entry = vtss_mac_entry_add(vtss_state, user, &entry->vid_mac)) == NULL) {
            return VTSS_RC_ERROR;
        }
        for (port_no = VTSS_PORT_NO_START; port_no < port_count; port_no++)
            VTSS_PORT_BF_SET(mac_entry->member, port_no, entry->destination[port_no]);
        mac_entry->cpu_copy = cpu_copy;
        mac_entry->cpu_queue = cpu_queue;
    } else {
        /* Unlocked entry, must be non-IPMC with zero/one/all destination port and no CPU copy */
        if (ipmc) {
            VTSS_E("IP multicast entry must be locked");
            return VTSS_RC_ERROR;
        }

        for (port_no = VTSS_PORT_NO_START; port_no < port_count; port_no++) {
            if (entry->destination[port_no])
                count++;
        }
        if (cpu_copy || (count != 0 && count != 1 && count != port_count)) {
            VTSS_E("unlocked can only have zero, one or all destination ports and no cpu_copy");
            return VTSS_RC_ERROR;
        }

        /* Delete any previous locked entry from state block */
        VTSS_RC(vtss_mac_entry_del(vtss_state, user, &vid_mac));
    }

    /* No further processing in warm start mode */
    if (vtss_state->warm_start_cur)
        return VTSS_RC_OK;

    if (ipmc) {
        /* IPv4/IPv6 multicast address, use pseudo PGID */
        pgid = VTSS_PGID_NONE;
        pgid_entry = &vtss_state->l2.pgid_table[pgid];
        for (port_no = VTSS_PORT_NO_START; port_no < port_count; port_no++)
            pgid_entry->member[port_no] = VTSS_BOOL(entry->destination[port_no]);
        vtss_pgid_members_get(vtss_state, pgid, pgid_entry->member);
    } else {
        /* Free old PGID if the address exists */
        old_entry.vid_mac = vid_mac;
        pgid = 0; /* Please Lint */
        if (VTSS_FUNC(l2.mac_table_get, &old_entry, &pgid) == VTSS_RC_OK && old_entry.locked) {
#if defined(VTSS_FEATURE_VSTAX_V2)
            /* Do not free PGID for old VStaX entry */
            vstax = old_entry.vstax2.enable;
#endif /* VTSS_FEATURE_VSTAX_V2 */
            if (!vstax) {
                VTSS_RC(vtss_pgid_free(vtss_state, pgid));
            }
        }

        /* Allocate new PGID */
        pgid = VTSS_PGID_NONE;
        for (port_no = VTSS_PORT_NO_START; port_no < port_count; port_no++) {
            member[port_no] = VTSS_BOOL(entry->destination[port_no]);
            if (!entry->locked && member[port_no])
                pgid = port_no;
        }

#if defined(VTSS_FEATURE_VSTAX_V2)
        vstax = entry->vstax2.enable;
#endif /* VTSS_FEATURE_VSTAX_V2 */
        if (vstax) {
            /* Do not allocate PGID for VStaX entry */
            pgid = 0;
        } else if (entry->locked) {
            VTSS_RC(vtss_pgid_alloc(vtss_state, &pgid, member, cpu_copy, cpu_queue));
        } else if (count == 0) {
            pgid = vtss_state->l2.pgid_drop;
        } else if (count == port_count) {
            pgid = vtss_state->l2.pgid_flood;
        }
    }

    vtss_state->l2.mac_status.learned = 1;

    return VTSS_FUNC(l2.mac_table_add, entry, pgid);
}

vtss_rc vtss_mac_table_add(const vtss_inst_t             inst,
                           const vtss_mac_table_entry_t  *const entry)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("vid: %d, mac: %02x-%02x-%02x-%02x-%02x-%02x, copy_to_cpu: %u locked: %u",
           entry->vid_mac.vid,
           entry->vid_mac.mac.addr[0], entry->vid_mac.mac.addr[1], entry->vid_mac.mac.addr[2],
           entry->vid_mac.mac.addr[3], entry->vid_mac.mac.addr[4], entry->vid_mac.mac.addr[5],
           entry->copy_to_cpu,
           entry->locked);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_add(vtss_state, VTSS_MAC_USER_NONE, entry);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_del(vtss_state_t *vtss_state,
                     vtss_mac_user_t user, const vtss_vid_mac_t *const vid_mac)
{
    vtss_rc                rc;
    vtss_mac_table_entry_t entry;
    u32                    pgid = 0; /* Please Lint */
    BOOL                   vstax = 0;

    VTSS_D("vid: %d, mac: %02x-%02x-%02x-%02x-%02x-%02x",
           vid_mac->vid,
           vid_mac->mac.addr[0], vid_mac->mac.addr[1], vid_mac->mac.addr[2],
           vid_mac->mac.addr[3], vid_mac->mac.addr[4], vid_mac->mac.addr[5]);

    /* Delete entry from state block */
    VTSS_RC(vtss_mac_entry_del(vtss_state, user, vid_mac));

    /* No further processing in warm start mode */
    if (vtss_state->warm_start_cur)
        return VTSS_RC_OK;
    
    /* Free PGID and delete if the entry exists */
    entry.vid_mac = *vid_mac;
    if ((rc = VTSS_FUNC(l2.mac_table_get, &entry, &pgid)) == VTSS_RC_OK) {
#if defined(VTSS_FEATURE_VSTAX_V2)
        /* Do not free PGID for old VStaX entry */
        vstax = entry.vstax2.enable;
#endif /* VTSS_FEATURE_VSTAX_V2 */
        if (entry.locked && !vstax)
            VTSS_RC(vtss_pgid_free(vtss_state, pgid));
        vtss_state->l2.mac_status.aged = 1;
        rc = VTSS_FUNC(l2.mac_table_del, vid_mac);
    }
    return rc;
}

vtss_rc vtss_mac_table_del(const vtss_inst_t     inst,
                           const vtss_vid_mac_t  *const vid_mac)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_del(vtss_state, VTSS_MAC_USER_NONE, vid_mac);
    VTSS_EXIT();
    return rc;
}

static void vtss_mac_pgid_get(vtss_state_t *vtss_state,
                              vtss_mac_table_entry_t *const entry, u32 pgid)
{
    vtss_pgid_members_get(vtss_state, pgid, entry->destination);
    
#if defined(VTSS_FEATURE_MAC_CPU_QUEUE)
    /* The CPU copy flag may be set in the MAC address table or PGID table */
    {
        vtss_pgid_entry_t *pgid_entry = &vtss_state->l2.pgid_table[pgid];
    
        if (pgid_entry->cpu_copy) {
            entry->copy_to_cpu = 1;
            entry->cpu_queue = pgid_entry->cpu_queue;
        }
    }
#endif /* VTSS_FEATURE_MAC_CPU_QUEUE */
}

vtss_rc vtss_mac_get(vtss_state_t *vtss_state,
                     const vtss_vid_mac_t   *const vid_mac,
                     vtss_mac_table_entry_t *const entry)
{
    vtss_rc rc;
    u32     pgid = 0; /* Please Lint */

    VTSS_D("vid: %d, mac: %02x-%02x-%02x-%02x-%02x-%02x",
           vid_mac->vid,
           vid_mac->mac.addr[0], vid_mac->mac.addr[1], vid_mac->mac.addr[2],
           vid_mac->mac.addr[3], vid_mac->mac.addr[4], vid_mac->mac.addr[5]);

    entry->vid_mac = *vid_mac;
    if ((rc = VTSS_FUNC(l2.mac_table_get, entry, &pgid)) != VTSS_RC_OK)
        return rc;

    vtss_mac_pgid_get(vtss_state, entry, pgid);

    return VTSS_RC_OK;
}

vtss_rc vtss_mac_table_get(const vtss_inst_t       inst,
                           const vtss_vid_mac_t    *const vid_mac,
                           vtss_mac_table_entry_t  *const entry)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_get(vtss_state, vid_mac, entry);
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_mac_get_next(vtss_state_t *vtss_state,
                                 const vtss_vid_mac_t   *const vid_mac,
                                 vtss_mac_table_entry_t *const entry)
{
    vtss_rc                rc = VTSS_RC_ERROR;
    u32                    pgid = 0; /* Please Lint */
    vtss_mac_table_entry_t mac_entry;
    u32                    mach, macl;
    vtss_mac_entry_t       *cur, *cmp;
    vtss_vid_mac_t         vid_mac_next;

    VTSS_D("vid: %d, mac: %02x-%02x-%02x-%02x-%02x-%02x",
           vid_mac->vid,
           vid_mac->mac.addr[0], vid_mac->mac.addr[1], vid_mac->mac.addr[2],
           vid_mac->mac.addr[3], vid_mac->mac.addr[4], vid_mac->mac.addr[5]);

    mac_entry.vid_mac = *vid_mac;
    vtss_mach_macl_get(vid_mac, &mach, &macl);
    for (cur = vtss_mac_entry_get(vtss_state, mach, macl, 1); cur != NULL; cur = cur->next) {
        if (cur->user != VTSS_MAC_USER_NONE) {
            continue;
        }

        /* Lookup in chip */
        vtss_mach_macl_set(&vid_mac_next, cur->mach, cur->macl);
        if ((rc = vtss_mac_get(vtss_state, &vid_mac_next, entry)) == VTSS_RC_OK) {
            VTSS_D("found sw entry 0x%08x%08x", cur->mach, cur->macl);
            break;
        }
    }

    while (VTSS_FUNC(l2.mac_table_get_next, &mac_entry, &pgid) == VTSS_RC_OK) {
        vtss_mac_pgid_get(vtss_state, &mac_entry, pgid);
        vtss_mach_macl_get(&mac_entry.vid_mac, &mach, &macl);
        VTSS_D("found chip entry 0x%08x%08x", mach, macl);

        if ((cmp = vtss_mac_entry_get(vtss_state, mach, macl, 0)) != NULL &&
            cmp->mach == mach && cmp->macl == macl && cmp->user != VTSS_MAC_USER_NONE) {
            continue;
        }

        /* Check for 'cur == NULL' is to please Lint */
        if (rc != VTSS_RC_OK || cur == NULL || (mach < cur->mach || (mach == cur->mach && macl < cur->macl)))
            *entry = mac_entry;

        rc = VTSS_RC_OK;
        break;
    }

    return rc;
}


vtss_rc vtss_mac_table_get_next(const vtss_inst_t       inst,
                                const vtss_vid_mac_t    *const vid_mac,
                                vtss_mac_table_entry_t  *const entry)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_get_next(vtss_state, vid_mac, entry);
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_MAC_AGE_AUTO)
vtss_rc vtss_mac_table_age_time_get(const vtss_inst_t          inst,
                                    vtss_mac_table_age_time_t  *const age_time)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *age_time = vtss_state->l2.mac_age_time;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_age_time_set(const vtss_inst_t                inst,
                                    const vtss_mac_table_age_time_t  age_time)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("age_time: %u", age_time);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.mac_age_time = age_time;
        rc = VTSS_FUNC_0(l2.mac_table_age_time_set);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_MAC_AGE_AUTO */

static vtss_rc vtss_mac_age(vtss_state_t     *vtss_state,
                            BOOL             pgid_age,
                            u32              pgid,
                            BOOL             vid_age,
                            const vtss_vid_t vid,
                            u32              count)
{
    vtss_rc        rc;
    u32            i;
    vtss_port_no_t port_no = VTSS_PORT_NO_NONE;

    VTSS_D("enter, pgid_age: %u, pgid: %u, vid_age: %u, vid: %u, count: %u", pgid_age, pgid, vid_age, vid, count);

    if (pgid_age && pgid < vtss_state->port_count)
        port_no = vtss_aggr_port(vtss_state, pgid);

    for (i = 0; i < count; i++) {
        if ((rc = VTSS_FUNC_COLD(l2.mac_table_age, pgid_age, pgid, vid_age, vid)) != VTSS_RC_OK)
            return rc;

        /* Age logical port */
        if (port_no != VTSS_PORT_NO_NONE &&
            (rc = VTSS_FUNC_COLD(l2.mac_table_age, pgid_age, port_no, vid_age, vid)) != VTSS_RC_OK)
            return rc;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_mac_table_age(const vtss_inst_t  inst)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 0, 0, 0, 0, 1);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_vlan_age(const vtss_inst_t  inst,
                                const vtss_vid_t   vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("vid: %u", vid);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 0, 0, 1, vid, 1);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_flush(const vtss_inst_t inst)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 0, 0, 0, 0, 2);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_port_flush(const vtss_inst_t     inst,
                                  const vtss_port_no_t  port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 1, port_no, 0, 0, 2);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_vlan_flush(const vtss_inst_t  inst,
                                  const vtss_vid_t   vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("vid: %u", vid);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 0, 0, 1, vid, 2);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_vlan_port_flush(const vtss_inst_t     inst,
                                       const vtss_port_no_t  port_no,
                                       const vtss_vid_t      vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u, vid: %u", port_no, vid);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_age(vtss_state, 1, port_no, 1, vid, 2);
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_VSTAX_V2)
vtss_rc vtss_mac_table_upsid_upspn_flush(const vtss_inst_t        inst,
                                         const vtss_vstax_upsid_t upsid,
                                         const vtss_vstax_upspn_t upspn)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        VTSS_VSTAX_UPSID_LEGAL(upsid)) {
        rc = VTSS_FUNC(l2.mac_table_upsid_upspn_flush, upsid, upspn);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mac_table_upsid_flush(const vtss_inst_t        inst,
                                   const vtss_vstax_upsid_t upsid)
{
    return vtss_mac_table_upsid_upspn_flush(inst, upsid, VTSS_UPSPN_NONE);
}
#endif /* VTSS_FEATURE_VSTAX_V2 */

vtss_rc vtss_vid_check(const vtss_vid_t vid)
{
    if (vid == VTSS_VID_NULL || vid >= VTSS_VIDS) {
        VTSS_E("illegal vid: %u", vid);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

#if defined(VTSS_FEATURE_AGGR_GLAG)
static vtss_rc vtss_glag_check(vtss_state_t *vtss_state, const vtss_glag_no_t glag_no, u32 *pgid)
{
    if (glag_no >= VTSS_GLAG_NO_END) {
        VTSS_E("illegal glag_no: %d", glag_no);
        return VTSS_RC_ERROR;
    }
    *pgid = (vtss_state->l2.pgid_glag_dest + glag_no - VTSS_GLAG_NO_START);
    return VTSS_RC_OK;
}

static vtss_rc vtss_mac_glag_add(vtss_state_t *vtss_state,
                                 const vtss_mac_table_entry_t  *const entry,
                                 const vtss_glag_no_t          glag_no)
{
    u32 pgid;

    VTSS_D("glag_no: %d", glag_no);

    VTSS_RC(vtss_glag_check(vtss_state, glag_no, &pgid));

    /* Delete entry if it exists (free IPv4/IPv6 MC and PGID) */
    (void)vtss_mac_del(vtss_state, VTSS_MAC_USER_NONE, &entry->vid_mac);
    vtss_state->l2.mac_status.learned = 1;

    return VTSS_FUNC(l2.mac_table_add, entry, pgid);
}

vtss_rc vtss_mac_table_glag_add(const vtss_inst_t             inst,
                                const vtss_mac_table_entry_t  *const entry,
                                const vtss_glag_no_t          glag_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_glag_add(vtss_state, entry, glag_no);
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_mac_glag_flush(vtss_state_t *vtss_state, const vtss_glag_no_t glag_no)
{
    u32 pgid = 0;

    VTSS_D("glag_no: %u", glag_no);
    VTSS_RC(vtss_glag_check(vtss_state, glag_no, &pgid));
    return vtss_mac_age(vtss_state, 1, pgid, 0, 0, 2);
}

vtss_rc vtss_mac_table_glag_flush(const vtss_inst_t     inst,
                                  const vtss_glag_no_t  glag_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("glag_no: %u", glag_no);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_glag_flush(vtss_state, glag_no);
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_mac_vlan_glag_flush(vtss_state_t *vtss_state,
                                        const vtss_glag_no_t  glag_no,
                                        const vtss_vid_t      vid)
{
    u32 pgid;

    VTSS_D("glag_no: %u", glag_no);
    VTSS_RC(vtss_glag_check(vtss_state, glag_no, &pgid));
    VTSS_RC(vtss_vid_check(vid));
    return vtss_mac_age(vtss_state, 1, pgid, 1, vid, 2);
}

vtss_rc vtss_mac_table_vlan_glag_flush(const vtss_inst_t     inst,
                                       const vtss_glag_no_t  glag_no,
                                       const vtss_vid_t      vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("glag_no: %u vid: %d", glag_no, vid);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = vtss_mac_vlan_glag_flush(vtss_state, glag_no, vid);
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_AGGR_GLAG */

vtss_rc vtss_mac_table_status_get(const vtss_inst_t        inst,
                                  vtss_mac_table_status_t  *const status)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = VTSS_FUNC(l2.mac_table_status_get, status)) == VTSS_RC_OK) {
        /* Read and clear events */
        if (vtss_state->l2.mac_status.learned)
            status->learned = 1;
        if (vtss_state->l2.mac_status.aged)
            status->aged = 1;
        memset(&vtss_state->l2.mac_status, 0, sizeof(*status));
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_learn_port_mode_get(const vtss_inst_t     inst,
                                 const vtss_port_no_t  port_no,
                                 vtss_learn_mode_t     *const mode)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *mode = vtss_state->l2.learn_mode[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_learn_port_mode_set(const vtss_inst_t        inst,
                                 const vtss_port_no_t     port_no,
                                 const vtss_learn_mode_t  *const mode)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u, auto: %u, cpu: %u, discard: %u",
           port_no, mode->automatic, mode->cpu, mode->discard);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.learn_mode[port_no] = *mode;
        rc = VTSS_FUNC_COLD(l2.learn_port_mode_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

/* - Operational state --------------------------------------------- */

vtss_rc vtss_port_state_get(const vtss_inst_t     inst,
                            const vtss_port_no_t  port_no,
                            BOOL                  *const state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *state = vtss_state->l2.port_state[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_port_state_set(const vtss_inst_t     inst,
                            const vtss_port_no_t  port_no,
                            BOOL                  state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_I("port_no: %u, state: %s", port_no, state ? "up" : "down");
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.port_state[port_no] = state;
        rc = vtss_update_masks(vtss_state, 1, 0, 1);
    }
    VTSS_EXIT();
    return rc;
}

/* - Spanning Tree ------------------------------------------------- */

vtss_rc vtss_stp_port_state_get(const vtss_inst_t     inst,
                                const vtss_port_no_t  port_no,
                                vtss_stp_state_t      *const state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *state = vtss_state->l2.stp_state[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_stp_port_state_set(const vtss_inst_t       inst,
                                const vtss_port_no_t    port_no,
                                const vtss_stp_state_t  state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_I("port_no: %u, state: %s",
           port_no,
           state == VTSS_STP_STATE_DISCARDING ? "discard" :
           state == VTSS_STP_STATE_LEARNING ? "learn" : "forward");
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.stp_state[port_no] = state;
        rc = vtss_update_masks(vtss_state, 1, 0, 1);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mstp_vlan_msti_get(const vtss_inst_t  inst,
                                const vtss_vid_t   vid,
                                vtss_msti_t        *const msti)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *msti = vtss_state->l2.vlan_table[vid].msti;
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_msti_check(const vtss_msti_t msti)
{
    if (msti >= VTSS_MSTI_END) {
        VTSS_E("illegal msti: %u", msti);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_mstp_vlan_msti_set(const vtss_inst_t  inst,
                                const vtss_vid_t   vid,
                                const vtss_msti_t  msti)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_N("vid: %u, msti: %u", vid, msti);

    VTSS_RC(vtss_vid_check(vid));
    VTSS_RC(vtss_msti_check(msti));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.vlan_table[vid].msti = msti;
        rc = VTSS_FUNC_COLD(l2.mstp_vlan_msti_set, vid);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mstp_port_msti_state_get(const vtss_inst_t     inst,
                                      const vtss_port_no_t  port_no,
                                      const vtss_msti_t     msti,
                                      vtss_stp_state_t      *const state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_msti_check(msti));
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *state = vtss_state->l2.mstp_table[msti].state[port_no - VTSS_PORT_NO_START];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mstp_port_msti_state_set(const vtss_inst_t       inst,
                                      const vtss_port_no_t    port_no,
                                      const vtss_msti_t       msti,
                                      const vtss_stp_state_t  state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_N("port_no: %u, msti: %u, state: %s",
           port_no, msti,
           state == VTSS_STP_STATE_DISCARDING ? "discard" :
           state == VTSS_STP_STATE_LEARNING ? "learn" : "forward");

    VTSS_RC(vtss_msti_check(msti));
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.mstp_table[msti].state[port_no - VTSS_PORT_NO_START] = state;
        rc = VTSS_FUNC_COLD(l2.mstp_state_set, port_no, msti);
    }
    VTSS_EXIT();
    return rc;
}

static vtss_rc vtss_erpi_check(const vtss_erpi_t erpi)
{
    if (erpi >= VTSS_ERPI_END) {
        VTSS_E("illegal erpi: %u", erpi);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_erps_vlan_member_get(const vtss_inst_t inst,
                                  const vtss_erpi_t erpi,
                                  const vtss_vid_t  vid,
                                  BOOL              *const member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_erpi_check(erpi));
    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *member = VTSS_BF_GET(vtss_state->l2.erps_table[erpi].vlan_member, vid);
    VTSS_EXIT();
    return rc;
}

/* Update ERPS discard state for VLAN */
static void vtss_erps_vlan_update(vtss_state_t *vtss_state,
                                  vtss_erpi_t erpi, vtss_vid_t vid, BOOL member)
{
    vtss_port_no_t    port_no;
    vtss_vlan_entry_t *vlan_entry;
    vtss_erps_entry_t *erps_entry = &vtss_state->l2.erps_table[erpi];

    if (VTSS_BF_GET(erps_entry->vlan_member, vid) != member) {
        VTSS_BF_SET(erps_entry->vlan_member, vid, member);

        /* Update ERPI discard state for VLAN */
        vlan_entry = &vtss_state->l2.vlan_table[vid];
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (!VTSS_PORT_BF_GET(erps_entry->port_member, port_no)) {
                /* Update number of rings with port in discarding state */
                if (member)
                    vlan_entry->erps_discard_cnt[port_no]++;
                else
                    vlan_entry->erps_discard_cnt[port_no]--;
            }
        }
    }
}

vtss_rc vtss_erps_vlan_member_set(const vtss_inst_t inst,
                                  const vtss_erpi_t erpi,
                                  const vtss_vid_t  vid,
                                  const BOOL        member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_N("erpi: %u, vid: %u, member: %u", erpi, vid, member);

    VTSS_RC(vtss_erpi_check(erpi));
    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_erps_vlan_update(vtss_state, erpi, vid, VTSS_BOOL(member));
        rc = VTSS_FUNC_COLD(l2.erps_vlan_member_set, erpi, vid);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_erps_port_state_get(const vtss_inst_t    inst,
                                 const vtss_erpi_t    erpi,
                                 const vtss_port_no_t port_no,
                                 vtss_erps_state_t    *const state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_erpi_check(erpi));
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *state = (VTSS_PORT_BF_GET(vtss_state->l2.erps_table[erpi].port_member, port_no) ?
                  VTSS_ERPS_STATE_FORWARDING : VTSS_ERPS_STATE_DISCARDING);
    VTSS_EXIT();
    return rc;
}

/* Update ERPS discard state for port */
static BOOL vtss_erps_port_update(vtss_state_t *vtss_state,
                                  vtss_erpi_t erpi, vtss_port_no_t port_no, BOOL forward)
{
    vtss_vid_t          vid;
    vtss_vlan_entry_t   *vlan_entry;
    vtss_erps_entry_t   *erps_entry = &vtss_state->l2.erps_table[erpi];
    vtss_erps_counter_t count;

    /* Check if forwarding state has changed */
    if (forward == VTSS_PORT_BF_GET(erps_entry->port_member, port_no))
        return 0;
    VTSS_PORT_BF_SET(erps_entry->port_member, port_no, forward);

    /* Update ERPI discard state for port in VLANs enabled for the ring */
    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        if (VTSS_BF_GET(erps_entry->vlan_member, vid)) {
            vlan_entry = &vtss_state->l2.vlan_table[vid];
            count = vlan_entry->erps_discard_cnt[port_no];
            if (forward) {
                /* Change to forwarding state */
                count--;
                if (count == 0)
                    vlan_entry->update = 1;
            } else {
                /* Change to discarding state */
                count++;
                if (count == 1)
                    vlan_entry->update = 1;
            }
            vlan_entry->erps_discard_cnt[port_no] = count;
        }
    }

    return 1;
}

vtss_rc vtss_erps_port_state_set(const vtss_inst_t       inst,
                                 const vtss_erpi_t       erpi,
                                 const vtss_port_no_t    port_no,
                                 const vtss_erps_state_t state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    BOOL         forward = VTSS_BOOL(state == VTSS_ERPS_STATE_FORWARDING);

    VTSS_N("erpi: %u, port_no: %u, state: %s",
           erpi, port_no, forward ? "forward" : "discard");

    VTSS_RC(vtss_erpi_check(erpi));
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        if (vtss_erps_port_update(vtss_state, erpi, port_no, forward))
            rc = VTSS_FUNC_COLD(l2.erps_port_state_set, erpi, port_no);
    }
    VTSS_EXIT();
    return rc;
}

/* - VLAN ---------------------------------------------------------- */

#if defined(VTSS_FEATURE_VLAN_PORT_V2)
vtss_rc vtss_vlan_conf_get(const vtss_inst_t inst,
                           vtss_vlan_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *conf = vtss_state->l2.vlan_conf;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_conf_set(const vtss_inst_t      inst,
                           const vtss_vlan_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.vlan_conf = *conf;
        rc = VTSS_FUNC_COLD_0(l2.vlan_conf_set);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_VLAN_PORT_V2 */

vtss_rc vtss_vlan_port_conf_get(const vtss_inst_t      inst,
                                const vtss_port_no_t   port_no,
                                vtss_vlan_port_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_N("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->l2.vlan_port_conf[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_port_conf_set(const vtss_inst_t            inst,
                                const vtss_port_no_t         port_no,
                                const vtss_vlan_port_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.vlan_port_conf[port_no] = *conf;
        rc = VTSS_FUNC_COLD(l2.vlan_port_conf_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_port_members_get(const vtss_inst_t  inst,
                                   const vtss_vid_t   vid,
                                   BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = VTSS_PORT_BF_GET(vtss_state->l2.vlan_table[vid].member, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_port_members_set(const vtss_inst_t  inst,
                                   const vtss_vid_t   vid,
                                   const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t      *vtss_state;
    vtss_rc           rc;
    vtss_port_no_t    port_no;
    vtss_vlan_entry_t *vlan_entry;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vlan_entry = &vtss_state->l2.vlan_table[vid];
        vlan_entry->enabled = 0;
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            VTSS_PORT_BF_SET(vlan_entry->member, port_no, member[port_no]);
            if (member[port_no])
                vlan_entry->enabled = 1;
        }
        rc = VTSS_FUNC_COLD(l2.vlan_port_members_set, vid);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_vid_conf_get(const vtss_inst_t    inst,
                               const vtss_vid_t     vid,
                               vtss_vlan_vid_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *conf = vtss_state->l2.vlan_table[vid].conf;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_vid_conf_set(const vtss_inst_t          inst,
                               const vtss_vid_t           vid,
                               const vtss_vlan_vid_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.vlan_table[vid].conf = *conf;
        rc = vtss_cmn_vlan_members_set(vtss_state, vid);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_VLAN_TX_TAG)
vtss_rc vtss_vlan_tx_tag_get(const vtss_inst_t  inst,
                             const vtss_vid_t   vid,
                             vtss_vlan_tx_tag_t tx_tag[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("vid: %u", vid);
    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            tx_tag[port_no] = vtss_state->l2.vlan_table[vid].tx_tag[port_no];
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_tx_tag_set(const vtss_inst_t        inst,
                             const vtss_vid_t         vid,
                             const vtss_vlan_tx_tag_t tx_tag[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    
    VTSS_D("vid: %u", vid);
    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(l2.vlan_tx_tag_set, vid, tx_tag);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_VLAN_TX_TAG */

/* - Port Isolation------------------------------------------------- */

vtss_rc vtss_isolated_vlan_get(const vtss_inst_t  inst,
                               const vtss_vid_t   vid,
                               BOOL               *const isolated)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *isolated = vtss_state->l2.vlan_table[vid].isolated;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_isolated_vlan_set(const vtss_inst_t  inst,
                               const vtss_vid_t   vid,
                               const BOOL         isolated)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.vlan_table[vid].isolated = isolated;
        rc = VTSS_FUNC_COLD(l2.isolated_vlan_set, vid);
    }
    VTSS_EXIT();
    return rc;

}

vtss_rc vtss_isolated_port_members_get(const vtss_inst_t  inst,
                                       BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.isolated_port[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_isolated_port_members_set(const vtss_inst_t  inst,
                                       const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.isolated_port[port_no] = member[port_no];
        rc = VTSS_FUNC_COLD_0(l2.isolated_port_members_set);
    }
    VTSS_EXIT();
    return rc;
}

 /* - Private VLAN (PVLAN) ------------------------------------------ */

#if defined(VTSS_FEATURE_PVLAN)
static vtss_rc vtss_pvlan_no_check(const vtss_pvlan_no_t pvlan_no)
{
    if (pvlan_no >= VTSS_PVLAN_NO_END) {
        VTSS_E("illegal pvlan_no: %u", pvlan_no);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_pvlan_port_members_get(const vtss_inst_t      inst,
                                    const vtss_pvlan_no_t  pvlan_no,
                                    BOOL                   member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t       *vtss_state;
    vtss_rc            rc;
    vtss_port_no_t     port_no;
    vtss_pvlan_entry_t *pvlan_entry;

    VTSS_RC(vtss_pvlan_no_check(pvlan_no));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        pvlan_entry = &vtss_state->l2.pvlan_table[pvlan_no];
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = pvlan_entry->member[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_pvlan_port_members_set(const vtss_inst_t      inst,
                                    const vtss_pvlan_no_t  pvlan_no,
                                    const BOOL             member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t       *vtss_state;
    vtss_rc            rc;
    vtss_port_no_t     port_no;
    vtss_pvlan_entry_t *pvlan_entry;

    VTSS_RC(vtss_pvlan_no_check(pvlan_no));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        pvlan_entry = &vtss_state->l2.pvlan_table[pvlan_no];
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            pvlan_entry->member[port_no] = member[port_no];
        rc = vtss_update_masks(vtss_state, 1, 0, 0);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_apvlan_port_members_get(const vtss_inst_t    inst,
                                     const vtss_port_no_t port_no,
                                     BOOL                 member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t e_port;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        for (e_port = VTSS_PORT_NO_START; e_port < vtss_state->port_count; e_port++) {
            member[e_port] = vtss_state->l2.apvlan_table[port_no][e_port];
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_apvlan_port_members_set(const vtss_inst_t    inst,
                                     const vtss_port_no_t port_no,
                                     const BOOL           member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t e_port;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        for (e_port = VTSS_PORT_NO_START; e_port < vtss_state->port_count; e_port++) {
            vtss_state->l2.apvlan_table[port_no][e_port] = member[e_port];
        }
        rc = vtss_update_masks(vtss_state, 1, 0, 0);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_PVLAN */

vtss_rc vtss_dgroup_port_conf_get(const vtss_inst_t       inst,
                                  const vtss_port_no_t    port_no,
                                  vtss_dgroup_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->l2.dgroup_port_conf[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_dgroup_port_conf_set(const vtss_inst_t             inst,
                                  const vtss_port_no_t          port_no,
                                  const vtss_dgroup_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK &&
        (rc = vtss_port_no_check(vtss_state, conf->dgroup_no)) == VTSS_RC_OK) {
        vtss_state->l2.dgroup_port_conf[port_no] = *conf;
        rc = vtss_update_masks(vtss_state, 0, 1, 0);
    }
    VTSS_EXIT();
    return rc;
}

/* - Aggregation --------------------------------------------------- */

#if defined(VTSS_FEATURE_VSTAX_V2)
static vtss_rc vtss_vstax_glag_conf_set(vtss_state_t *vtss_state,
                                        const vtss_glag_no_t           glag_no,
                                        const vtss_vstax_glag_entry_t  entry[VTSS_GLAG_PORT_ARRAY_SIZE])
{
    u32              port, port_no, gport;
    vtss_port_map_t  *map;
    vtss_glag_conf_t *conf;

    /* Delete old local GLAG members */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (vtss_state->l2.port_glag_no[port_no] == glag_no)
            vtss_state->l2.port_glag_no[port_no] = VTSS_GLAG_NO_NONE;
    }
    
    for (gport = VTSS_GLAG_PORT_START; gport < VTSS_GLAG_PORT_END; gport++) {
        conf = &vtss_state->l2.glag_conf[glag_no - VTSS_GLAG_NO_START][gport];
        conf->entry = entry[gport];
        if ((port = entry[gport].upspn) == VTSS_UPSPN_NONE) {
            /* End of list */
            if (gport == VTSS_GLAG_PORT_START) /* Deleting GLAG, flush MAC table */
                VTSS_RC(vtss_mac_glag_flush(vtss_state, glag_no));
            break;
        }
               
        /* Update local members */
        conf->port_no = VTSS_PORT_NO_NONE;
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            map = &vtss_state->port.map[port_no];
            if (vtss_state->l2.vstax_info.upsid[map->chip_no] == entry[gport].upsid &&
                map->chip_port == port && !vtss_state->l2.vstax_port_enabled[port_no]) {
                /* Found local front port */
                vtss_state->l2.port_glag_no[port_no] = glag_no;
                conf->port_no = port_no;
            }
        }
    }

    /* Update masks */
    return vtss_update_masks(vtss_state, 0, 1, 1);
}
#endif /* VTSS_FEATURE_VSTAX_V2 */

static vtss_rc vtss_aggr_no_check(vtss_state_t *vtss_state, const vtss_aggr_no_t aggr_no)
{
    if (aggr_no >= (vtss_state->port_count/2)) {
        VTSS_E("illegal aggr_no: %u", aggr_no);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_aggr_port_members_get(const vtss_inst_t     inst,
                                   const vtss_aggr_no_t  aggr_no,
                                   BOOL                  member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;
    vtss_glag_no_t glag_no;

    VTSS_D("aggr_no: %u", aggr_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_aggr_no_check(vtss_state, aggr_no)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            glag_no = vtss_state->l2.port_aggr_no[port_no];
#if defined(VTSS_FEATURE_VSTAX_V2)
            if (vtss_state->chip_count == 2) {
                /* LLAGs implemented using GLAGs */
                glag_no = vtss_state->l2.port_glag_no[port_no];
            }
#endif /* VTSS_FEATURE_VSTAX_V2 */
            member[port_no] = VTSS_BOOL(glag_no == aggr_no);
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aggr_port_members_set(const vtss_inst_t     inst,
                                   const vtss_aggr_no_t  aggr_no,
                                   const BOOL            member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("aggr_no: %u", aggr_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_aggr_no_check(vtss_state, aggr_no)) == VTSS_RC_OK) {
#if defined(VTSS_FEATURE_VSTAX_V2)
        if (vtss_state->chip_count == 2) {
            /* LLAGs implemented using GLAGs */
            vtss_glag_no_t          glag_no = aggr_no;
            vtss_vstax_glag_entry_t entry[VTSS_GLAG_PORT_ARRAY_SIZE];
            u32                     gport = 0;
            vtss_port_map_t         *map;
            
            memset(entry, 0, sizeof(entry)); /* Please Lint */
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                if (member[port_no]) {
                    map = &vtss_state->port.map[port_no];
                    entry[gport].upsid = vtss_state->l2.vstax_info.upsid[map->chip_no];
                    entry[gport].upspn = map->chip_port;
                    gport++;
                    if (gport == VTSS_GLAG_PORT_ARRAY_SIZE)
                        break;
                }
            }
            if (gport != VTSS_GLAG_PORT_ARRAY_SIZE) /* Mark end of list */
                entry[gport].upspn = VTSS_UPSPN_NONE;
            rc = vtss_vstax_glag_conf_set(vtss_state, glag_no, entry);
        } else
#endif /* VTSS_FEATURE_VSTAX_V2 */
        {
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                if (member[port_no])
                    vtss_state->l2.port_aggr_no[port_no] = aggr_no;
                else if (vtss_state->l2.port_aggr_no[port_no] == aggr_no)
                    vtss_state->l2.port_aggr_no[port_no] = VTSS_AGGR_NO_NONE;
            }
            rc = vtss_update_masks(vtss_state, 1, 1, 1);
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aggr_mode_get(const vtss_inst_t  inst,
                           vtss_aggr_mode_t   *const mode)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *mode = vtss_state->l2.aggr_mode;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_aggr_mode_set(const vtss_inst_t       inst,
                           const vtss_aggr_mode_t  *const mode)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.aggr_mode = *mode;
        rc = VTSS_FUNC_0(l2.aggr_mode_set);
    }
    VTSS_EXIT();
    return rc;
}

/* - Global link aggregations across stack ------------------------- */

#if defined(VTSS_FEATURE_AGGR_GLAG)
static vtss_rc vtss_glag_no_check(const vtss_glag_no_t glag_no)
{
    if (glag_no >= VTSS_GLAG_NO_END) {
        VTSS_E("illegal glag_no: %u", glag_no);
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_aggr_glag_members_get(const vtss_inst_t     inst,
                                   const vtss_glag_no_t  glag_no,
                                   BOOL                  member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("glag_no: %u", glag_no);
    VTSS_RC(vtss_glag_no_check(glag_no));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = VTSS_BOOL(vtss_state->l2.port_glag_no[port_no] == glag_no &&
                                        vtss_state->l2.port_state[port_no]);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_VSTAX_V2)
vtss_rc vtss_vstax_glag_get(const vtss_inst_t        inst,
                            const vtss_glag_no_t     glag_no,
                            vtss_vstax_glag_entry_t  entry[VTSS_GLAG_PORT_ARRAY_SIZE])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;
    u32          gentry;

    VTSS_D("glag_no: %u", glag_no);
    VTSS_RC(vtss_glag_no_check(glag_no));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (gentry = VTSS_GLAG_PORT_START; gentry < VTSS_GLAG_PORT_END; gentry++) {            
            entry[gentry] = vtss_state->l2.glag_conf[glag_no - VTSS_GLAG_NO_START][gentry].entry;
        }        
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vstax_glag_set(const vtss_inst_t              inst,
                            const vtss_glag_no_t           glag_no,
                            const vtss_vstax_glag_entry_t  entry[VTSS_GLAG_PORT_ARRAY_SIZE])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("glag_no: %u", glag_no);
    VTSS_RC(vtss_glag_no_check(glag_no));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = vtss_vstax_glag_conf_set(vtss_state, glag_no, entry);        
    }
        
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_VSTAX_V2 */

#endif /* VTSS_FEATURE_AGGR_GLAG */

/* - Mirroring ----------------------------------------------------- */

vtss_rc vtss_mirror_conf_get(const vtss_inst_t  inst,
                             vtss_mirror_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *conf = vtss_state->l2.mirror_conf;
    }
        
    VTSS_EXIT();
    return rc;
}


vtss_rc vtss_mirror_conf_set(const vtss_inst_t        inst,
                             const vtss_mirror_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_port_no_none_check(vtss_state, conf->port_no)) == VTSS_RC_OK) {
        vtss_state->l2.mirror_conf = *conf;
        rc = VTSS_FUNC_COLD_0(l2.mirror_port_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_monitor_port_get(const vtss_inst_t  inst,
                                     vtss_port_no_t     *const port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *port_no = vtss_state->l2.mirror_conf.port_no;
    }
        
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_monitor_port_set(const vtss_inst_t     inst,
                                     const vtss_port_no_t  port_no)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK &&
        (rc = vtss_port_no_none_check(vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.mirror_conf.port_no = port_no;
        rc = VTSS_FUNC_COLD_0(l2.mirror_port_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_ingress_ports_get(const vtss_inst_t  inst,
                                      BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.mirror_ingress[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_ingress_ports_set(const vtss_inst_t  inst,
                                      const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.mirror_ingress[port_no] = VTSS_BOOL(member[port_no]);
        rc = VTSS_FUNC_COLD_0(l2.mirror_ingress_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_egress_ports_get(const vtss_inst_t  inst,
                                     BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.mirror_egress[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_egress_ports_set(const vtss_inst_t  inst,
                                     const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.mirror_egress[port_no] = VTSS_BOOL(member[port_no]);
        rc = VTSS_FUNC_COLD_0(l2.mirror_egress_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_cpu_egress_get(const vtss_inst_t  inst,
                                   BOOL               *member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *member = vtss_state->l2.mirror_cpu_egress;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_cpu_egress_set(const vtss_inst_t  inst,
                                   const BOOL         member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.mirror_cpu_egress = VTSS_BOOL(member);
        rc = VTSS_FUNC_COLD_0(l2.mirror_cpu_egress_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_cpu_ingress_get(const vtss_inst_t  inst,
                                    BOOL               *member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *member = vtss_state->l2.mirror_cpu_ingress;
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mirror_cpu_ingress_set(const vtss_inst_t  inst,
                                    const BOOL         member)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.mirror_cpu_ingress = VTSS_BOOL(member);
        rc = VTSS_FUNC_COLD_0(l2.mirror_cpu_ingress_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_eps_port_conf_get(const vtss_inst_t     inst,
                               const vtss_port_no_t  port_no,
                               vtss_eps_port_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->l2.port_protect[port_no].conf;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_eps_port_conf_set(const vtss_inst_t           inst,
                               const vtss_port_no_t        port_no,
                               const vtss_eps_port_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u, type: %u, conf.port_no: %u", port_no, conf->type, conf->port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.port_protect[port_no].conf = *conf;
        rc = vtss_update_masks(vtss_state, 1, 1, 1);
        if (rc == VTSS_RC_OK)
            rc = VTSS_FUNC_COLD(l2.eps_port_set, port_no);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_eps_port_selector_get(const vtss_inst_t    inst,
                                   const vtss_port_no_t port_no,
                                   vtss_eps_selector_t  *const selector)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *selector = vtss_state->l2.port_protect[port_no].selector;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_eps_port_selector_set(const vtss_inst_t          inst,
                                   const vtss_port_no_t       port_no,
                                   const vtss_eps_selector_t  selector)
{
    vtss_state_t         *vtss_state;
    vtss_rc              rc;
    vtss_eps_port_conf_t *conf;

    VTSS_D("port_no: %u selector: %u", port_no, selector);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.port_protect[port_no].selector = selector;
        rc = vtss_update_masks(vtss_state, 1, 0, 1);
        if (rc == VTSS_RC_OK)
            rc = VTSS_FUNC_COLD(l2.eps_port_set, port_no);
        conf = &vtss_state->l2.port_protect[port_no].conf;
        if (rc == VTSS_RC_OK && conf->type == VTSS_EPS_PORT_1_FOR_1) {
            /* Flush port for 1:1 protection */
            if (selector == VTSS_EPS_SELECTOR_PROTECTION) {
                /* Flush working port */
                rc = vtss_mac_age(vtss_state, 1, port_no, 0, 0, 2);
            } else if (conf->port_no != VTSS_PORT_NO_NONE) {
                /* Flush protecting port */
                rc = vtss_mac_age(vtss_state, 1, conf->port_no, 0, 0, 2);
            }
        }
    }
    VTSS_EXIT();
    return rc;
}

/* - Flooding control ---------------------------------------------- */
vtss_rc vtss_uc_flood_members_get(const vtss_inst_t  inst,
                                  BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.uc_flood[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_uc_flood_members_set(const vtss_inst_t  inst,
                                  const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.uc_flood[port_no] = member[port_no];
        rc = VTSS_FUNC_COLD_0(l2.flood_conf_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mc_flood_members_get(const vtss_inst_t  inst,
                                  BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.mc_flood[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_mc_flood_members_set(const vtss_inst_t  inst,
                                  const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.mc_flood[port_no] = member[port_no];
        rc = VTSS_FUNC_COLD_0(l2.flood_conf_set);
    }
    VTSS_EXIT();
    return rc;
}

/* - IGMP ---------------------------------------------------- */

vtss_rc vtss_ipv4_mc_flood_members_get(const vtss_inst_t  inst,
                                       BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.ipv4_mc_flood[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv4_mc_flood_members_set(const vtss_inst_t  inst,
                                       const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.ipv4_mc_flood[port_no] = VTSS_BOOL(member[port_no]);
        rc = VTSS_FUNC_COLD_0(l2.flood_conf_set);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_IPV4_MC_SIP)
vtss_rc vtss_ipv4_mc_add(const vtss_inst_t inst,
                         const vtss_vid_t  vid,
                         const vtss_ip_t   sip,
                         const vtss_ip_t   dip,
                         const BOOL        member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(l2.ipv4_mc_add, vid, sip, dip, member);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv4_mc_del(const vtss_inst_t inst,
                         const vtss_vid_t  vid,
                         const vtss_ip_t   sip,
                         const vtss_ip_t   dip)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(l2.ipv4_mc_del, vid, sip, dip);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_IPV4_MC_SIP */

/* - MLD  ----------------------------------------------------- */

#if defined(VTSS_FEATURE_IPV6_MC_SIP)
vtss_rc vtss_ipv6_mc_add(const vtss_inst_t inst,
                         const vtss_vid_t  vid,
                         const vtss_ipv6_t sip,
                         const vtss_ipv6_t dip,
                         const BOOL        member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(l2.ipv6_mc_add, vid, sip, dip, member);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv6_mc_del(const vtss_inst_t inst,
                         const vtss_vid_t  vid,
                         const vtss_ipv6_t sip,
                         const vtss_ipv6_t dip)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        rc = VTSS_FUNC_COLD(l2.ipv6_mc_del, vid, sip, dip);
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_IPV6_MC_SIP */

vtss_rc vtss_ipv6_mc_flood_members_get(const vtss_inst_t  inst,
                                       BOOL               member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            member[port_no] = vtss_state->l2.ipv6_mc_flood[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv6_mc_flood_members_set(const vtss_inst_t  inst,
                                       const BOOL         member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            vtss_state->l2.ipv6_mc_flood[port_no] = VTSS_BOOL(member[port_no]);
        rc = VTSS_FUNC_COLD_0(l2.flood_conf_set);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv6_mc_ctrl_flood_get(const vtss_inst_t  inst,
                                    BOOL               *const scope)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *scope = vtss_state->l2.ipv6_mc_scope;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_ipv6_mc_ctrl_flood_set(const vtss_inst_t  inst,
                                    const BOOL         scope)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("scope: %u", scope);
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.ipv6_mc_scope = VTSS_BOOL(scope);
        rc = VTSS_FUNC_COLD_0(l2.flood_conf_set);
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_VSTAX)
vtss_rc vtss_vstax_conf_get(const vtss_inst_t  inst,
                            vtss_vstax_conf_t  *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        *conf = vtss_state->l2.vstax_conf;
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vstax_conf_set(const vtss_inst_t        inst,
                            const vtss_vstax_conf_t  *const conf)
{
    vtss_state_t   *vtss_state;
    vtss_rc        rc;
    vtss_port_no_t port_no;
    BOOL           enabled, changed = 0;

    VTSS_D("enter");
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        if (!VTSS_VSTAX_UPSID_LEGAL(conf->upsid_0)) {
            VTSS_E("illegal upsid_0: %d", conf->upsid_0);
            rc = VTSS_RC_ERROR;
        } else if (!VTSS_VSTAX_UPSID_LEGAL(conf->upsid_1)) {
            VTSS_E("illegal upsid_1: %d", conf->upsid_1);
            rc = VTSS_RC_ERROR;
        } else if ((rc = vtss_port_no_none_check(vtss_state, conf->port_0)) == VTSS_RC_OK &&
                   (rc = vtss_port_no_none_check(vtss_state, conf->port_1)) == VTSS_RC_OK) {
            /* Store VStaX configuration directly */
            vtss_state->l2.vstax_conf = *conf;

            /* Store UPSIDs in VStaX info */
            vtss_state->l2.vstax_info.upsid[0] = vtss_state->l2.vstax_conf.upsid_0;
            vtss_state->l2.vstax_info.upsid[1] = vtss_state->l2.vstax_conf.upsid_1;
            
            /* Store information about VStaX enabled ports */
            for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
                enabled = VTSS_BOOL(port_no == conf->port_0 || port_no == conf->port_1);
                if (vtss_state->l2.vstax_port_enabled[port_no] != enabled) {
                    vtss_state->l2.vstax_port_enabled[port_no] = enabled;
                    changed = 1;
                }
            }
            
            /* Set VStaX configuration */
            rc = VTSS_FUNC_0(l2.vstax_conf_set);

            /* Update VLAN memberships if stack ports changed */
            if (changed)
                rc = vtss_cmn_vlan_update_all(vtss_state);
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vstax_port_conf_get(const vtss_inst_t      inst,
                                 const vtss_chip_no_t   chip_no,
                                 const BOOL             stack_port_a,
                                 vtss_vstax_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("chip_no: %u, port %s", chip_no, stack_port_a ? "A" : "B");
    VTSS_ENTER();
    if ((rc = vtss_inst_chip_no_check(inst, &vtss_state, chip_no)) == VTSS_RC_OK)
        *conf = vtss_state->l2.vstax_info.chip_info[chip_no].port_conf[stack_port_a ? 0 : 1];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vstax_port_conf_set(const vtss_inst_t            inst,
                                 const vtss_chip_no_t         chip_no,
                                 const BOOL                   stack_port_a,
                                 const vtss_vstax_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("chip_no: %u, port %s", chip_no, stack_port_a ? "A" : "B");
    VTSS_ENTER();
    if ((rc = vtss_inst_chip_no_check(inst, &vtss_state, chip_no)) == VTSS_RC_OK) {
        vtss_state->l2.vstax_info.chip_info[chip_no].port_conf[stack_port_a ? 0 : 1] = *conf;
        if ((rc = VTSS_FUNC(l2.vstax_port_conf_set, stack_port_a)) == VTSS_RC_OK) {
            // Also update source masks, in case we need to change the mode from
            // discarding to forwarding all frames received on the port, or vice versa.
            rc = vtss_update_masks(vtss_state, 1, 0, 0);
        }
    }

    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vstax_master_upsid_get(const vtss_inst_t        inst,
                                          vtss_vstax_upsid_t *const master_upsid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (master_upsid == NULL) {
        return VTSS_RC_ERROR;
    }

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        *master_upsid = vtss_state->l2.vstax_info.master_upsid;
    }
    VTSS_EXIT();

    return rc;
}

vtss_rc vtss_vstax_master_upsid_set(const vtss_inst_t        inst,
                                    const vtss_vstax_upsid_t master_upsid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    if (!VTSS_VSTAX_UPSID_LEGAL(master_upsid) && master_upsid != VTSS_VSTAX_UPSID_UNDEF) {
        return VTSS_RC_ERROR;
    }

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK) {
        vtss_state->l2.vstax_info.master_upsid = master_upsid;
        if (vtss_state->l2.vstax_master_upsid_set == NULL) {
            rc = VTSS_RC_OK; // If no CIL function exists, don't return an error.
        } else {
            rc = vtss_state->l2.vstax_master_upsid_set(vtss_state);
        }
    }
    VTSS_EXIT();
    return rc;
}

#if defined(VTSS_FEATURE_VSTAX_V2)
vtss_rc vtss_vstax_topology_set(const vtss_inst_t              inst, 
                                const vtss_chip_no_t           chip_no,
                                const vtss_vstax_route_table_t *table)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_chip_no_check(inst, &vtss_state, chip_no)) == VTSS_RC_OK) {
        if (table->topology_type == VTSS_VSTAX_TOPOLOGY_CHAIN ||
            table->topology_type == VTSS_VSTAX_TOPOLOGY_RING) {
            vtss_state->l2.vstax_info.chip_info[chip_no].rt_table = *table;
            rc = VTSS_FUNC_0(l2.vstax_topology_set);
        } else {
            VTSS_E("illegal topology_type: %d", table->topology_type);
            rc = VTSS_RC_ERROR;
        }
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_VSTAX_V2 */
#endif /* VTSS_FEATURE_VSTAX */

#if defined(VTSS_FEATURE_VCL)
vtss_rc vtss_vcl_port_conf_get(const vtss_inst_t    inst,
                               const vtss_port_no_t port_no,
                               vtss_vcl_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->l2.vcl_port_conf[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vcl_port_conf_set(const vtss_inst_t          inst,
                               const vtss_port_no_t       port_no,
                               const vtss_vcl_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_vcl_port_conf_t *vcl_port_conf = &vtss_state->l2.vcl_port_conf[port_no];

#if defined(VTSS_ARCH_SERVAL)
        vtss_state->vcap.port_conf_old = vtss_state->vcap.port_conf[port_no]; /* Needed by Serval1 CIL function */
#endif /* VTSS_ARCH_SERVAL */

        vtss_state->l2.vcl_port_conf_old = *vcl_port_conf;
        *vcl_port_conf = *conf;
        rc = VTSS_FUNC_COLD(l2.vcl_port_conf_set, port_no);
        if (rc != VTSS_RC_OK) {
            /* Restore configuration if operation failed */
            *vcl_port_conf = vtss_state->l2.vcl_port_conf_old;
        }
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vce_init(const vtss_inst_t inst,
                      const vtss_vce_type_t  type,
                      vtss_vce_t *const vce)
{
    vtss_vce_key_t *key = &vce->key;
    
    VTSS_D("type: %d", type);
    memset(vce, 0, sizeof(*vce));
    key->type = type;
    key->mac.dmac_mc = VTSS_VCAP_BIT_ANY;
    key->mac.dmac_bc = VTSS_VCAP_BIT_ANY;
    key->tag.dei = VTSS_VCAP_BIT_ANY;
    key->tag.tagged = VTSS_VCAP_BIT_ANY;
    key->tag.s_tag = VTSS_VCAP_BIT_ANY;

    switch (type) {
    case VTSS_VCE_TYPE_ANY:
    case VTSS_VCE_TYPE_ETYPE:
    case VTSS_VCE_TYPE_LLC:
    case VTSS_VCE_TYPE_SNAP:
    case VTSS_VCE_TYPE_IPV6:
        break;
    case VTSS_VCE_TYPE_IPV4:
        key->frame.ipv4.fragment = VTSS_VCAP_BIT_ANY;
        key->frame.ipv4.options = VTSS_VCAP_BIT_ANY;
        break;
    default:
        VTSS_E("unknown type: %d", type);
        return VTSS_RC_ERROR;
    }

    vce->action.policy_no = VTSS_ACL_POLICY_NO_NONE;
    return VTSS_RC_OK;
}

vtss_rc vtss_vce_add(const vtss_inst_t    inst,
                     const vtss_vce_id_t  vce_id,
                     const vtss_vce_t     *const vce)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("vce_id: %u before %u %s",
            vce->id, vce_id, vce_id == VTSS_VCE_ID_LAST ? "(last)" : "");

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vce_add, vce_id, vce);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vce_del(const vtss_inst_t    inst,
                     const vtss_vce_id_t  vce_id)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("vce_id: %u", vce_id);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vce_del, vce_id);
    VTSS_EXIT();
    return rc;
}
#endif /* VCL */

#if defined(VTSS_FEATURE_VLAN_TRANSLATION)
vtss_rc vtss_vlan_trans_group_add(const vtss_inst_t      inst,
                                  const u16              group_id,
                                  const vtss_vid_t       vid,
                                  const vtss_vid_t       trans_vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("group_id: %u, vid: %u, trans_vid: %u", group_id, vid, trans_vid);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_trans_group_add, group_id, vid, trans_vid);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_trans_group_del(const vtss_inst_t      inst,
                                  const u16              group_id,
                                  const vtss_vid_t       vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("group_id: %u, vid: %u", group_id, vid);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_trans_group_del, group_id, vid);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_trans_group_get(const vtss_inst_t                inst,
                                  vtss_vlan_trans_grp2vlan_conf_t  *conf,
                                  BOOL                             next)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("group_id: %u", conf->group_id);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_trans_group_get, conf, next);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_trans_group_to_port_set(const vtss_inst_t                     inst,
                                          const vtss_vlan_trans_port2grp_conf_t *conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("group_id: %u", conf->group_id);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_trans_port_conf_set, conf);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_trans_group_to_port_get(const vtss_inst_t                inst,
                                          vtss_vlan_trans_port2grp_conf_t  *conf,
                                          BOOL                             next)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("group_id: %u", conf->group_id);

    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_trans_port_conf_get, conf, next);
    VTSS_EXIT();
    return rc;
}

#endif /* VLAN_TRANSLATION */

#if defined(VTSS_ARCH_SERVAL)
vtss_rc vtss_vcap_port_conf_get(const vtss_inst_t     inst,
                                const vtss_port_no_t  port_no,
                                vtss_vcap_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *conf = vtss_state->vcap.port_conf[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vcap_port_conf_set(const vtss_inst_t           inst,
                                const vtss_port_no_t        port_no,
                                const vtss_vcap_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_vcap_port_conf_t *vcap_port_conf = &vtss_state->vcap.port_conf[port_no];

        vtss_state->vcap.port_conf_old = *vcap_port_conf;
        *vcap_port_conf = *conf;
        rc = VTSS_FUNC_COLD(l2.vcap_port_conf_set, port_no);
        if (rc != VTSS_RC_OK) {
            /* Restore configuration if operation failed */
            *vcap_port_conf = vtss_state->vcap.port_conf_old;
        }
    }
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_ARCH_SERVAL */

/* VLAN counters */
#if defined(VTSS_FEATURE_VLAN_COUNTERS)
vtss_rc vtss_vlan_counters_get(const vtss_inst_t    inst,
                               const vtss_vid_t     vid,
                               vtss_vlan_counters_t *const counters)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_counters_get, vid, counters);
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_vlan_counters_clear(const vtss_inst_t  inst,
                                 const vtss_vid_t   vid)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_RC(vtss_vid_check(vid));
    VTSS_ENTER();
    if ((rc = vtss_inst_check(inst, &vtss_state)) == VTSS_RC_OK)
        rc = VTSS_FUNC(l2.vlan_counters_clear, vid);
    VTSS_EXIT();
    return rc;
}
#endif /* VTSS_FEATURE_VLAN_COUNTERS */

/* - Port Based Network Access Control, 802.1X --------------------- */

vtss_rc vtss_auth_port_state_get(const vtss_inst_t     inst,
                                 const vtss_port_no_t  port_no,
                                 vtss_auth_state_t     *const state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK)
        *state = vtss_state->l2.auth_state[port_no];
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_auth_port_state_set(const vtss_inst_t        inst,
                                 const vtss_port_no_t     port_no,
                                 const vtss_auth_state_t  state)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_D("port_no: %u", port_no);
    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        vtss_state->l2.auth_state[port_no] = state;
        rc = vtss_update_masks(vtss_state, 1, 0, 1);
    }
    VTSS_EXIT();
    return rc;
}

/* - SFLOW --------------------------------------------------------- */

#ifdef VTSS_FEATURE_SFLOW
vtss_rc vtss_sflow_port_conf_get(const vtss_inst_t      inst,
                                 const u32              port_no,
                                 vtss_sflow_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        *conf = vtss_state->l2.sflow_conf[port_no];
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_sflow_port_conf_set(const vtss_inst_t            inst,
                                 const vtss_port_no_t         port_no,
                                 const vtss_sflow_port_conf_t *const conf)
{
    vtss_state_t *vtss_state;
    vtss_rc      rc;

    VTSS_ENTER();
    if ((rc = vtss_inst_port_no_check(inst, &vtss_state, port_no)) == VTSS_RC_OK) {
        rc = VTSS_FUNC(l2.sflow_port_conf_set, port_no, conf);
    }
    VTSS_EXIT();
    return rc;
}

vtss_rc vtss_sflow_sampling_rate_convert(const vtss_inst_t  inst,
                                         const BOOL         power2,
                                         const u32          rate_in,
                                               u32          *const rate_out)
{
    // Don't take API lock and don't change vtss_state, since this is
    // a non-state-dependant function.
    vtss_state_t *vtss_state = vtss_inst_check_no_persist(inst);

    return VTSS_FUNC_FROM_STATE(vtss_state, l2.sflow_sampling_rate_convert, power2, rate_in, rate_out);
}

#endif /* VTSS_FEATURE_SFLOW */

/* - Warm start synchronization ------------------------------------ */

#if defined(VTSS_FEATURE_WARM_START)
static void vtss_mac_entry_add_sync(vtss_state_t *vtss_state,
                                    vtss_mac_entry_t *cur, u32 pgid, BOOL new)
{
    vtss_rc        rc;
    BOOL           *member;
    vtss_vid_mac_t vid_mac;
    vtss_port_no_t port_no;

    /* Skip if already added */
    if (cur->flags & VTSS_MAC_FLAG_ADDED)
        return;

    /* Add entry */
    cur->flags |= VTSS_MAC_FLAG_ADDED;
    member = vtss_state->l2.pgid_table[VTSS_PGID_NONE].member;
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
        member[port_no] = VTSS_PORT_BF_GET(cur->member, port_no);
    vtss_mach_macl_set(&vid_mac, cur->mach, cur->macl);
    if (vtss_ipmc_mac(vtss_state, &vid_mac)) {
        rc = VTSS_RC_OK;
        pgid = VTSS_PGID_NONE;
        vtss_pgid_members_get(vtss_state, pgid, member);
    } else {
        rc = vtss_pgid_alloc(vtss_state, &pgid, member, cur->cpu_copy, cur->cpu_queue);
    }
    if (rc == VTSS_RC_OK) {
        VTSS_I("adding %s: %08x-%08x, pgid: %u",
               new ? "new" : "old" , cur->mach, cur->macl, pgid);
        (void)vtss_mac_entry_update(vtss_state, cur, pgid);
    }
}

/* Synchronize static entries in the MAC address table */
static vtss_rc vtss_mac_table_sync(vtss_state_t *vtss_state)
{
    vtss_mac_table_entry_t mac_entry;
    u32                    mach, macl, pgid = 0; /* Please Lint */
    vtss_mac_entry_t       *cur = vtss_state->l2.mac_list_used;
    BOOL                   found, next = 1;

    /* Step 1:
       Delete static hardware entries not present in the API table.
       Reuse PGIDs for entries present in the API table */
    memset(&mac_entry, 0, sizeof(mac_entry));
    while (1) {
        if (next) {
            /* Get next static HW entry */
            found = 0;
            while (VTSS_FUNC(l2.mac_table_get_next, &mac_entry, &pgid) == VTSS_RC_OK) {
                if (mac_entry.locked) {
                    found = 1;
                    break;
                }
            }
            if (!found)
                break;
        }
        
        vtss_mach_macl_get(&mac_entry.vid_mac, &mach, &macl);
        next = 1;
        if (cur == NULL || cur->mach > mach || (cur->mach == mach && cur->macl > macl)) {
            /* SW entry non-existing or bigger, delete HW entry */
            VTSS_I("deleting %08x-%08x", mach, macl);
            (void) VTSS_FUNC(l2.mac_table_del, &mac_entry.vid_mac);
        } else {
            if (cur->mach == mach && cur->macl == macl) {
                /* SW entry matches, update HW entry reusing PGID */
                vtss_mac_entry_add_sync(vtss_state, cur, pgid, 0);
            } else {
                /* SW entry smaller, do not get next HW entry */
                next = 0;
            }
            cur = cur->next;
        }
    }

    /* Step 2: Add new entries */
    for (cur = vtss_state->l2.mac_list_used; cur != NULL; cur = cur->next) {
        vtss_mac_entry_add_sync(vtss_state, cur, VTSS_PGID_NONE, 1); /* Do not reuse PGID */
    }
    return VTSS_RC_OK;
}

vtss_rc vtss_l2_restart_sync(vtss_state_t *vtss_state)
{
    vtss_port_no_t port_no;
    vtss_vid_t     vid;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        VTSS_FUNC_RC(l2.learn_port_mode_set, port_no);
        VTSS_FUNC_RC(l2.vlan_port_conf_set, port_no);
#if defined(VTSS_ARCH_SERVAL)
        vtss_state->vcap.port_conf_old = vtss_state->vcap.port_conf[port_no];
        VTSS_FUNC_RC(l2.vcap_port_conf_set, port_no);
#elif defined(VTSS_FEATURE_VCL)
        VTSS_FUNC_RC(l2.vcl_port_conf_set, port_no); /* Not needed on Serval1 as l2.vcap_port_conf_set does the same thing */
#endif /* VTSS_ARCH_SERVAL */
    }    

    VTSS_FUNC_RC_0(l2.isolated_port_members_set);
    VTSS_FUNC_RC_0(l2.mirror_port_set);
    VTSS_FUNC_RC_0(l2.mirror_ingress_set);
    VTSS_FUNC_RC_0(l2.mirror_egress_set);
    VTSS_FUNC_RC_0(l2.mirror_cpu_egress_set);
    VTSS_FUNC_RC_0(l2.mirror_cpu_ingress_set);
    VTSS_FUNC_RC_0(l2.flood_conf_set);
    VTSS_RC(vtss_mac_table_sync(vtss_state));
    VTSS_RC(vtss_update_masks(vtss_state, 1, 1, 1));
    for (vid = 0; vid < VTSS_VIDS; vid++) {
        VTSS_FUNC_RC(l2.vlan_port_members_set, vid);
    }
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_WARM_START */

/* - Instance create and initialization ---------------------------- */

vtss_rc vtss_l2_inst_create(vtss_state_t *vtss_state)
{
    vtss_port_no_t        port_no;
    vtss_vlan_port_conf_t *vlan;
    vtss_vid_t            vid;
    vtss_vlan_entry_t     *vlan_entry;
    vtss_mstp_entry_t     *mstp_entry;
    u32                   i;

#if defined(VTSS_FEATURE_AGGR_GLAG)
    {
        u32 glag_no;

        for (glag_no = VTSS_GLAG_NO_START; glag_no < VTSS_GLAG_NO_END; glag_no++) {
#if defined(VTSS_FEATURE_VSTAX_V2)
            vtss_state->l2.glag_conf[glag_no][VTSS_GLAG_PORT_START].entry.upspn = VTSS_UPSPN_NONE;
#endif /* VTSS_FEATURE_VSTAX_V2 */
        }
    }
#endif /* VTSS_FEATURE_AGGR_GLAG */

#if defined(VTSS_FEATURE_VLAN_PORT_V2)
    vtss_state->l2.vlan_conf.s_etype = VTSS_ETYPE_TAG_S; /* Default S-tag Ethernet type */
#endif /* VTSS_FEATURE_VLAN_PORT_V2 */

    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++) {
        vlan = &vtss_state->l2.vlan_port_conf[port_no];
        vlan->pvid = VTSS_VID_DEFAULT;
        vlan->untagged_vid = VTSS_VID_DEFAULT;
        vtss_state->l2.learn_mode[port_no].automatic = 1;
        vtss_state->l2.stp_state[port_no] = VTSS_STP_STATE_FORWARDING;
        vtss_state->l2.port_aggr_no[port_no] = VTSS_AGGR_NO_NONE;
        vtss_state->l2.auth_state[port_no] = VTSS_AUTH_STATE_BOTH;
#if defined(VTSS_FEATURE_PVLAN)
        vtss_state->l2.pvlan_table[VTSS_PVLAN_NO_DEFAULT].member[port_no] = 1;
        {
            vtss_port_no_t e_port;
            
            for (e_port = VTSS_PORT_NO_START; e_port < VTSS_PORT_NO_END; e_port++) {
                vtss_state->l2.apvlan_table[port_no][e_port] = 1;
            }
        }
#endif /* VTSS_FEATURE_PVLAN */
        vtss_state->l2.dgroup_port_conf[port_no].dgroup_no = port_no;
        vtss_state->l2.uc_flood[port_no] = 1;
        vtss_state->l2.mc_flood[port_no] = 1;
        vtss_state->l2.ipv4_mc_flood[port_no] = 1;
        vtss_state->l2.ipv6_mc_flood[port_no] = 1;
        vtss_state->l2.port_protect[port_no].conf.port_no = VTSS_PORT_NO_NONE;
        vtss_state->l2.port_protect[port_no].selector = VTSS_EPS_SELECTOR_WORKING;
#if defined(VTSS_FEATURE_AGGR_GLAG)
        vtss_state->l2.port_glag_no[port_no] = VTSS_GLAG_NO_NONE;
#endif /* VTSS_FEATURE_AGGR_GLAG */
    }

    /* Initialize VLAN table */
    for (vid = 0; vid < VTSS_VIDS; vid++) {
        vlan_entry = &vtss_state->l2.vlan_table[vid];
        vlan_entry->msti = VTSS_MSTI_START;
        if (vid != VTSS_VID_NULL) {
            vlan_entry->conf.learning = 1;
            vlan_entry->evc_learning = 1;
        }
#if defined(VTSS_FEATURE_VLAN_SVL)
        vlan_entry->conf.fid = vid;
#endif /* VTSS_FEATURE_VLAN_SVL */
        if (vid != VTSS_VID_DEFAULT)
            continue;
        vlan_entry->enabled = 1;
        for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++)
            VTSS_PORT_BF_SET(vlan_entry->member, port_no, 1);
    }
    
    /* All ports are forwarding in first MSTP instance */
    mstp_entry = &vtss_state->l2.mstp_table[VTSS_MSTI_START];
    for (port_no = VTSS_PORT_NO_START; port_no < VTSS_PORT_NO_END; port_no++)
        mstp_entry->state[port_no] = VTSS_STP_STATE_FORWARDING;
    
    vtss_state->l2.aggr_mode.smac_enable = 1;
    
    vtss_state->l2.mirror_conf.port_no = VTSS_PORT_NO_NONE;
    
    /* Initialize MAC address table */
    vtss_state->l2.mac_table_max = VTSS_MAC_ADDRS;
    for (i = 0; i < VTSS_MAC_ADDRS; i++) {
        /* Insert first in free list */
        vtss_state->l2.mac_table[i].next = vtss_state->l2.mac_list_free;
        vtss_state->l2.mac_list_free = &vtss_state->l2.mac_table[i];
    }
    vtss_state->l2.mac_age_time = 300;

#if defined(VTSS_FEATURE_VSTAX)
    vtss_state->l2.vstax_conf.port_0       = VTSS_PORT_NO_NONE;
    vtss_state->l2.vstax_conf.port_1       = VTSS_PORT_NO_NONE;
    vtss_state->l2.vstax_info.master_upsid = VTSS_VSTAX_UPSID_UNDEF;
#endif /* VTSS_FEATURE_VSTAX */

#if defined(VTSS_FEATURE_VLAN_TRANSLATION)
    {
        vtss_vlan_trans_grp2vlan_entry_t *grp_entry;
        vtss_vlan_trans_grp2vlan_t       *grp_conf;
        vtss_vlan_trans_port2grp_entry_t *port_entry;
        vtss_vlan_trans_port2grp_t       *port_conf;

        /* Initialize VLAN Translation Group free/used lists */
        grp_conf = &vtss_state->l2.vt_trans_conf;
        grp_conf->used = NULL;
        for (i = VTSS_VLAN_TRANS_FIRST_GROUP_ID; i <= VTSS_VLAN_TRANS_MAX_CNT; i++) {
            grp_entry = &grp_conf->trans_list[i - VTSS_VLAN_TRANS_FIRST_GROUP_ID];
            /* Insert in free list */
            grp_entry->next = grp_conf->free;
            grp_conf->free = grp_entry;
        }
        /* Initialize VLAN Translation Port free/used lists */
        port_conf = &vtss_state->l2.vt_port_conf;
        port_conf->used = NULL;
        for (i = VTSS_VLAN_TRANS_FIRST_GROUP_ID; i <= VTSS_VLAN_TRANS_GROUP_MAX_CNT; i++) {
            port_entry = &port_conf->port_list[i - VTSS_VLAN_TRANS_FIRST_GROUP_ID];
            /* Insert in free list */
            port_entry->next = port_conf->free;
            port_conf->free = port_entry;
        }
    }
#endif /* VTSS_FEATURE_VLAN_TRANSLATION */
#if defined(VTSS_FEATURE_IPV4_MC_SIP) || defined(VTSS_FEATURE_IPV6_MC_SIP)
    {
        vtss_ipmc_info_t *ipmc = &vtss_state->l2.ipmc;
        vtss_ipmc_src_t  *src;
        vtss_ipmc_dst_t  *dst;
        
        ipmc->obj.src_max = VTSS_IPMC_SRC_MAX;
        ipmc->obj.dst_max = VTSS_IPMC_DST_MAX;
        
        /* Add IPMC entries to free list */
        ipmc->obj.name = "IPMC";
        for (i = 0; i < VTSS_IPMC_SRC_MAX; i++) {
            src = &ipmc->src_table[i];
            src->next = ipmc->obj.src_free;
            ipmc->obj.src_free = src;
        }
        for (i = 0; i < VTSS_IPMC_DST_MAX; i++) {
            dst = &ipmc->dst_table[i];
            dst->next = ipmc->obj.dst_free;
            ipmc->obj.dst_free = dst;
        }
    }
#endif /* VTSS_FEATURE_IPV4_MC_SIP || VTSS_FEATURE_IPV6_MC_SIP */

    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_vlan_members_get(vtss_state_t *state, 
                                  const vtss_vid_t vid,
                                  BOOL member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_port_no_t    port_no, port_p, mirror_port;
    vtss_vlan_entry_t *vlan_entry;
    vtss_mstp_entry_t *mstp_entry;
    vtss_port_eps_t   *protect;

    VTSS_N("update %d", vid);

    /* Lookup VLAN, MSTP and ERPS entries */
    vlan_entry = &state->l2.vlan_table[vid];
    mstp_entry = &state->l2.mstp_table[vlan_entry->msti];
    
    /* Mirror port and forwarding mode */
    mirror_port = (state->l2.mirror_conf.fwd_enable ? VTSS_PORT_NO_NONE : state->l2.mirror_conf.port_no);

    /* Determine VLAN port members */
    for (port_no = VTSS_PORT_NO_START; port_no < state->port_count; port_no++) {
        member[port_no] = (
            /* Include VLAN member port */
            VTSS_PORT_BF_GET(vlan_entry->member, port_no) && 
            /* Exclude MSTP/ERPS discarding port */
            mstp_entry->state[port_no] == VTSS_STP_STATE_FORWARDING &&
            port_no != mirror_port &&
            vlan_entry->erps_discard_cnt[port_no] == 0);
#if defined(VTSS_FEATURE_VSTAX)
        /* Include Stack port */
        if (state->l2.vstax_port_enabled[port_no])
            member[port_no] = 1;
#endif  /* VTSS_FEATURE_VSTAX */
#if defined(VTSS_FEATURE_NPI)
        /* Exclude NPI port */
        if (state->packet.npi_conf.enable && port_no == state->packet.npi_conf.port_no) {
            member[port_no] = 0;
        }
#endif /* VTSS_FEATURE_NPI */
    }

    /* Consider protections */
    for (port_no = VTSS_PORT_NO_START; port_no < state->port_count; port_no++) {
        protect = &state->l2.port_protect[port_no];
        if ((port_p = protect->conf.port_no) == VTSS_PORT_NO_NONE)
            continue;

        if (protect->conf.type == VTSS_EPS_PORT_1_PLUS_1) {
            /* Port is 1+1 protected, include both ports if one of them is VLAN member */
            if (member[port_no])
                member[port_p] = 1;
            if (member[port_p])
                member[port_no] = 1;
        } else if (member[port_no]) {
            /* Port is 1:1 protected and VLAN member */
            if (protect->selector == VTSS_EPS_SELECTOR_WORKING) {
                /* Working port selected, protection port is not VLAN member */
                member[port_p] = 0;
            } else {
                /* Protection port selected, working port is not VLAN member */
                member[port_p] = 1;
                member[port_no] = 0;
            }
        }
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_vlan_members_set(vtss_state_t *vtss_state, const vtss_vid_t vid)
{
    BOOL member[VTSS_PORT_ARRAY_SIZE];

    VTSS_N("update %d", vid);

    if (vtss_state->l2.vlan_mask_update == NULL)
        return VTSS_RC_ERROR;

    VTSS_RC(vtss_cmn_vlan_members_get(vtss_state, vid, member));

    return vtss_state->l2.vlan_mask_update(vtss_state, vid, member);
}

vtss_rc vtss_cmn_vlan_port_conf_set(vtss_state_t *vtss_state, const vtss_port_no_t port_no)
{
    vtss_rc                rc;
    vtss_vlan_port_conf_t  *conf;
    vtss_vid_t             uvid;
    
    if (vtss_state->l2.vlan_port_conf_update == NULL)
        return VTSS_RC_ERROR;

    conf = &vtss_state->l2.vlan_port_conf[port_no];
    uvid = conf->untagged_vid;
#if defined(VTSS_FEATURE_EVC)
    if (vtss_cmn_evc_port_check(vtss_state)) {
        /* Override UVID for UNI/NNI ports */
        if (vtss_state->evc.port_info[port_no].uni_count)
            conf->untagged_vid = VTSS_VID_ALL;
        else if (vtss_state->evc.port_info[port_no].nni_count)
            conf->untagged_vid = VTSS_VID_NULL;
    }
#endif /* VTSS_FEATURE_EVC */
    
    rc = vtss_state->l2.vlan_port_conf_update(vtss_state, port_no, conf);
    conf->untagged_vid = uvid;
#if defined(VTSS_FEATURE_ES0)
    if (vtss_state->l2.vlan_port_type[port_no] != conf->port_type) {
        /* VLAN port type changed, update ES0 rules */
        vtss_state->l2.vlan_port_type[port_no]= conf->port_type;
        VTSS_RC(vtss_vcap_es0_update(vtss_state, port_no, VTSS_ES0_FLAG_TPID));
    }
#endif /* VTSS_FEATURE_ES0 */
    
    return rc;
}

#if defined(VTSS_FEATURE_VLAN_TX_TAG)
static void vtss_cmn_es0_data_set(vtss_state_t *vtss_state,
                                  vtss_port_no_t port_no, vtss_vid_t vid, vtss_vid_t new_vid, 
                                  BOOL tag_enable, 
                                  vtss_vcap_data_t *data, vtss_es0_entry_t *entry)
{
    vtss_es0_data_t   *es0 = &data->u.es0;
    vtss_es0_key_t    *key = &entry->key;
    vtss_es0_action_t *action = &entry->action;

    /* Use (VID, port) as key */
    vtss_vcap_es0_init(data, entry);
    key->port_no = port_no;
    key->type = VTSS_ES0_TYPE_VID;
    key->data.vid.vid = vid;
    
    if (tag_enable) {
        /* Enable tagging */
        action->tag = VTSS_ES0_TAG_ES0;
        es0->flags = (VTSS_ES0_FLAG_TPID | VTSS_ES0_FLAG_QOS); /* Use port TPID and QoS */
        es0->port_no = port_no;
        action->vid_b = new_vid;
        
#if defined(VTSS_ARCH_SERVAL) || defined(VTSS_ARCH_JAGUAR_2)
        if (vtss_state->arch == VTSS_ARCH_SRVL || vtss_state->arch == VTSS_ARCH_JR2) {
            action->outer_tag.tag = VTSS_ES0_TAG_ES0;
            action->outer_tag.vid.sel = 1;
            action->outer_tag.vid.val = new_vid;
        }
#endif /* VTSS_ARCH_SERVAL/JAGUAR_2 */
    } else {
        action->tag = VTSS_ES0_TAG_NONE;
        action->tpid = VTSS_ES0_TPID_S; /* Needed to overrule port configuration */
    }
}

static vtss_vcap_id_t vtss_tx_tag_vcap_id(vtss_vid_t vid, vtss_port_no_t port_no)
{
    vtss_vcap_id_t id = vid;

    return ((id << 32) + port_no);
}

vtss_rc vtss_cmn_vlan_tx_tag_set(vtss_state_t *vtss_state,
                                 const vtss_vid_t         vid,
                                 const vtss_vlan_tx_tag_t tx_tag[VTSS_PORT_ARRAY_SIZE])
{
    vtss_res_t        res;
    vtss_port_no_t    port_no, i;
    vtss_vlan_entry_t *vlan_entry = &vtss_state->l2.vlan_table[vid];
    vtss_vcap_obj_t   *obj = &vtss_state->vcap.es0.obj;
    vtss_vcap_id_t    id, id_next;
    vtss_vid_t        vid_next;
    vtss_vcap_data_t  data;
    vtss_es0_entry_t  entry;
    BOOL              tag_enable;

    /* Check resource consumption */
    vtss_cmn_res_init(&res);
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (vlan_entry->tx_tag[port_no] == VTSS_VLAN_TX_TAG_PORT) {
            if (tx_tag[port_no] != VTSS_VLAN_TX_TAG_PORT) {
                /* Add new ES0 rule for port */
                res.es0.add++;
            }
        } else if (tx_tag[port_no] == VTSS_VLAN_TX_TAG_PORT) {
            /* Delete old ES0 rule for port */
            res.es0.del++;
        }
    }
    VTSS_RC(vtss_cmn_res_check(vtss_state, &res));

    /* Delete resources */
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (vlan_entry->tx_tag[port_no] != VTSS_VLAN_TX_TAG_PORT && 
            tx_tag[port_no] == VTSS_VLAN_TX_TAG_PORT) {
            id = vtss_tx_tag_vcap_id(vid, port_no);
            VTSS_RC(vtss_vcap_del(vtss_state, obj, VTSS_ES0_USER_TX_TAG, id));
        }
        vlan_entry->tx_tag[port_no] = tx_tag[port_no];
    }

    /* Find next ID */
    id_next = VTSS_VCAP_ID_LAST;
    for (vid_next = (vid + 1); vid_next < VTSS_VIDS; vid_next++) {
        vlan_entry = &vtss_state->l2.vlan_table[vid_next];
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            if (vlan_entry->tx_tag[port_no] != VTSS_VLAN_TX_TAG_PORT) {
                id_next = vtss_tx_tag_vcap_id(vid_next, port_no);
                vid_next = VTSS_VIDS;
                break;
            }
        }
    }

    /* Add/update resources */
    for (i = vtss_state->port_count; i != 0; i--) {
        port_no = (i - 1);
        switch (tx_tag[port_no]) {
        case VTSS_VLAN_TX_TAG_ENABLE:
            tag_enable = 1;
            break;
        case VTSS_VLAN_TX_TAG_DISABLE:
            tag_enable = 0;
            break;
        default:
            continue;
        }
        vtss_cmn_es0_data_set(vtss_state, port_no, vid, vid, tag_enable, &data, &entry);
        id = vtss_tx_tag_vcap_id(vid, port_no);
        VTSS_RC(vtss_vcap_add(vtss_state, obj, VTSS_ES0_USER_TX_TAG, id, id_next, &data, 0));
        id_next = id;
    }
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_VLAN_TX_TAG */

static BOOL vtss_cmn_vlan_enabled(vtss_vlan_entry_t *vlan_entry)
{
    return (vlan_entry->enabled);
}

/* Update all VLANs */
vtss_rc vtss_cmn_vlan_update_all(vtss_state_t *vtss_state)
{
    vtss_vid_t vid;

    for (vid = VTSS_VID_DEFAULT; vid < VTSS_VIDS; vid++)
        VTSS_RC(vtss_cmn_vlan_members_set(vtss_state, vid));
    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_mstp_state_set(vtss_state_t *vtss_state,
                                const vtss_port_no_t   port_no,
                                const vtss_msti_t      msti)
{
    vtss_vid_t        vid;
    vtss_vlan_entry_t *vlan_entry;
    
    /* Update all VLANs mapping to MSTI */
    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        vlan_entry = &vtss_state->l2.vlan_table[vid];
        if (vtss_cmn_vlan_enabled(vlan_entry) && vlan_entry->msti == msti)
            VTSS_RC(vtss_cmn_vlan_members_set(vtss_state, vid));
    }
    
    return VTSS_RC_OK;
}

vtss_rc vtss_cmn_erps_vlan_member_set(vtss_state_t *vtss_state,
                                      const vtss_erpi_t erpi,
                                      const vtss_vid_t  vid)
{
    return vtss_cmn_vlan_members_set(vtss_state, vid);
}

vtss_rc vtss_cmn_erps_port_state_set(vtss_state_t *vtss_state,
                                     const vtss_erpi_t    erpi,
                                     const vtss_port_no_t port_no)
{
    vtss_vid_t        vid;
    vtss_vlan_entry_t *vlan_entry;

    /* Update all VLANs changed by ERPS */
    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        vlan_entry = &vtss_state->l2.vlan_table[vid];
        if (vlan_entry->update) {
            vlan_entry->update = 0;
            if (vtss_cmn_vlan_enabled(vlan_entry))
                VTSS_RC(vtss_cmn_vlan_members_set(vtss_state, vid));
        }
    }
    return VTSS_RC_OK;

}

vtss_rc vtss_cmn_eps_port_set(vtss_state_t *vtss_state,
                              const vtss_port_no_t port_w)
{
    vtss_vid_t vid;

    /* Update all VLANs */
    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        if (vtss_cmn_vlan_enabled(&vtss_state->l2.vlan_table[vid]))
            VTSS_RC(vtss_cmn_vlan_members_set(vtss_state, vid));
    }

    return VTSS_RC_OK;
}

#if defined(VTSS_FEATURE_IPV4_MC_SIP) || defined(VTSS_FEATURE_IPV6_MC_SIP)
u32 vtss_cmn_ip2u32(vtss_ip_addr_internal_t *ip, BOOL ipv6)
{
    u32 addr = 0, i;

    if (ipv6) {
        for (i = 12; i < 16; i++) {
            addr += (ip->ipv6.addr[i] << ((15 - i)*8));
        }
    } else {
        addr = ip->ipv4;
    }
    return addr;
}

/* IPMC source key is (SIP, SSM, VID) */
static u64 vtss_cmn_ip_mc_src_key(vtss_ipmc_src_data_t *src, BOOL ipv6)
{
    u64 key = vtss_cmn_ip2u32(&src->sip, ipv6);

    key = (key << 32);
    if (src->ssm)
        key += (1<<12);
    key += src->vid;
    return key;
}

/* IPMC destination key is (DIP32/DIP23, DIP) */
static u64 vtss_cmn_ip_mc_dst_key(vtss_ipmc_dst_data_t *dst, BOOL ipv6)
{
    u32 dip = vtss_cmn_ip2u32(&dst->dip, ipv6);
    u64 key = (ipv6 ? dip : (dip &  0x007fffff));

    key = (key << 32);
    key += dip;
    return key;
}

static vtss_rc vtss_cmn_ip_mc_add(vtss_state_t *vtss_state, vtss_ipmc_data_t *ipmc)
{
    vtss_ipmc_obj_t *obj = &vtss_state->l2.ipmc.obj;
    vtss_ipmc_src_t *src, *src_prev = NULL, **src_list;
    vtss_ipmc_dst_t *dst = NULL, *dst_prev = NULL;
    vtss_vid_t      fid;
    u64             sip, ipmc_sip = vtss_cmn_ip_mc_src_key(&ipmc->src, ipmc->ipv6);
    u32             dip, ipmc_dip = vtss_cmn_ip_mc_dst_key(&ipmc->dst, ipmc->ipv6);

    /* Search for source entry or the place to insert the new entry */
    src_list = &obj->src_used[ipmc->ipv6];
    for (src = *src_list; src != NULL; src_prev = src, src = src->next) {
        sip = vtss_cmn_ip_mc_src_key(&src->data, ipmc->ipv6);
        if (sip > ipmc_sip) {
            /* Found bigger entry */
            src = NULL;
            break;
        } else if (sip == ipmc_sip) {
            /* Found matching key, save allocated FID */
            ipmc->src.fid = src->data.fid;
            if (!ipmc->ipv6 ||
                memcmp(&src->data.sip.ipv6, &ipmc->src.sip.ipv6, sizeof(vtss_ipv6_t)) == 0) {
                /* Found identical entry */
                break;
            }
        }
    }

    if (src == NULL) {
        /* Check if source entry can be allocated */
        if (obj->src_free == NULL) {
            VTSS_I("no more source entries");
            return VTSS_RC_ERROR;
        }
        ipmc->src_add = 1;
    } else {
        /* Source found, search for destination entry or the place to insert the new entry */
        for (dst = src->dest; dst != NULL; dst_prev = dst, dst = dst->next) {
            dip = vtss_cmn_ip_mc_dst_key(&dst->data, ipmc->ipv6);
            if (dip > ipmc_dip) {
                /* Found bigger entry */
                dst = NULL;
                break;
            } 
            
            if (dip == ipmc_dip && 
                (!ipmc->ipv6 || 
                 memcmp(&dst->data.dip.ipv6, &ipmc->dst.dip.ipv6, sizeof(vtss_ipv6_t)) == 0)) {
                /* Found identical entry */
                memcpy(dst->data.member, ipmc->dst.member, VTSS_PORT_BF_SIZE);
                break;
            }
        }
    }

    if (dst == NULL) {
        /* Check if destination entry can be allocated */
        if (obj->dst_free == NULL) {
            VTSS_I("no more destination entries");
            return VTSS_RC_ERROR;
        }
        ipmc->dst_add = 1;
    }

    /* Check that resources can be added in device */
    fid = ipmc->src.fid;
    VTSS_FUNC_RC(l2.ip_mc_update, ipmc, VTSS_IPMC_CMD_CHECK);

    /* Now allocate resources */
    if (src == NULL) {
        /* Allocate new source entry */
        src = obj->src_free;
        obj->src_free = src->next;
        obj->src_count++;
        if (src_prev == NULL) {
            /* Insert first in list */
            src->next = *src_list;
            *src_list = src;
        } else {
            /* Insert after previous entry */
            src->next = src_prev->next;
            src_prev->next = src;
        }
        src->data = ipmc->src;
        src->dest = NULL;
    }

    if (dst == NULL) {
        /* Allocate new destination entry */
        dst = obj->dst_free;
        obj->dst_free = dst->next;
        obj->dst_count++;
        if (dst_prev == NULL) {
            /* Insert first in list */
            dst->next = src->dest;
            src->dest = dst;
        } else {
            /* Insert after previous entry */
            dst->next = dst_prev->next;
            dst_prev->next = dst;
        }
        dst->data = ipmc->dst;
    }

    /* Add ressources in device */
    ipmc->src.fid = fid;
    VTSS_FUNC_RC(l2.ip_mc_update, ipmc, VTSS_IPMC_CMD_ADD);

    return VTSS_RC_OK;
}

static vtss_rc vtss_cmn_ip_mc_del(vtss_state_t *vtss_state, vtss_ipmc_data_t *ipmc)
{
    vtss_ipmc_obj_t *obj = &vtss_state->l2.ipmc.obj;
    vtss_ipmc_src_t *src, *src_prev = NULL, **src_list;
    vtss_ipmc_dst_t *dst = NULL, *dst_prev = NULL;
    
    /* Search for source entry */
    src_list = &obj->src_used[ipmc->ipv6];
    for (src = *src_list; src != NULL; src_prev = src, src = src->next) {
        if (src->data.vid == ipmc->src.vid && 
            ((!ipmc->ipv6 && ipmc->src.sip.ipv4 == src->data.sip.ipv4) ||
             (ipmc->ipv6 && 
              memcmp(&src->data.sip.ipv6, &ipmc->src.sip.ipv6, sizeof(vtss_ipv6_t)) == 0))) {
            ipmc->src.fid = src->data.fid;
            /* Found entry */
            break;
        }
    }

    if (src == NULL) {
        VTSS_E("IPv%u SIP 0x%08x not found", 
               ipmc->ipv6 ? 6 : 4, vtss_cmn_ip2u32(&ipmc->src.sip, ipmc->ipv6));
        return VTSS_RC_ERROR;
    }

    /* Search for destination entry */
    for (dst = src->dest; dst != NULL; dst_prev = dst, dst = dst->next) {
        if ((!ipmc->ipv6 && dst->data.dip.ipv4 == ipmc->dst.dip.ipv4) ||
            (ipmc->ipv6 && 
             memcmp(&dst->data.dip.ipv6, &ipmc->dst.dip.ipv6, sizeof(vtss_ipv6_t)) == 0)) {
            /* Found entry */
            break;
        }
    }
    

    if (dst == NULL) {
        VTSS_E("IPv%u DIP 0x%08x not found", 
               ipmc->ipv6 ? 6 : 4, vtss_cmn_ip2u32(&ipmc->dst.dip, ipmc->ipv6));
        return VTSS_RC_ERROR;
    }

    /* Move destination entry to free list */
    if (dst_prev == NULL)
        src->dest = dst->next;
    else 
        dst_prev->next = dst->next;
    dst->next = obj->dst_free;
    obj->dst_free = dst;
    obj->dst_count--;

    /* Free source, if it was the last destination */
    if (src->dest == NULL) {
        if (src_prev == NULL)
            *src_list = src->next;
        else
            src_prev->next = src->next;
        src->next = obj->src_free;
        obj->src_free = src;
        obj->src_count--;
        ipmc->src_del = 1;
    }
    ipmc->dst_del = 1;

    /* Delete resources */
    VTSS_FUNC_RC(l2.ip_mc_update, ipmc, VTSS_IPMC_CMD_DEL);
    
    return VTSS_RC_OK;
}

static void vtss_cmn_ipv4_mc_data_init(vtss_ipmc_data_t *ipmc, 
                                       const vtss_vid_t vid,
                                       const vtss_ip_t  sip,
                                       const vtss_ip_t  dip)
{
    memset(ipmc, 0, sizeof(*ipmc));
    ipmc->src.vid = vid;
    ipmc->src.sip.ipv4 = sip;
    ipmc->dst.dip.ipv4 = dip;
    if (sip == 0) {
        /* Zero SIP indicates ASM */
        ipmc->src.fid = vid;
    } else {
        ipmc->src.ssm = 1;
    }
}

vtss_rc vtss_cmn_ipv4_mc_add(vtss_state_t     *vtss_state,
                             const vtss_vid_t vid,
                             const vtss_ip_t  sip,
                             const vtss_ip_t  dip,
                             const BOOL       member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_ipmc_data_t ipmc;
    vtss_port_no_t   port_no;

    vtss_cmn_ipv4_mc_data_init(&ipmc, vid, sip, dip);
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        VTSS_PORT_BF_SET(ipmc.dst.member, port_no, member[port_no]);
    }
    return vtss_cmn_ip_mc_add(vtss_state, &ipmc);
}

vtss_rc vtss_cmn_ipv4_mc_del(vtss_state_t     *vtss_state,
                             const vtss_vid_t vid,
                             const vtss_ip_t  sip,
                             const vtss_ip_t  dip)
{
    vtss_ipmc_data_t ipmc;
    
    vtss_cmn_ipv4_mc_data_init(&ipmc, vid, sip, dip);
    return vtss_cmn_ip_mc_del(vtss_state, &ipmc);
}

static void vtss_cmn_ipv6_mc_data_init(vtss_ipmc_data_t  *ipmc, 
                                       const vtss_vid_t  vid,
                                       const vtss_ipv6_t sip,
                                       const vtss_ipv6_t dip)
{
    vtss_ipv6_t sipv6;
    
    memset(ipmc, 0, sizeof(*ipmc));
    ipmc->ipv6 = 1;
    ipmc->src.vid = vid;
    ipmc->src.sip.ipv6 = sip;
    ipmc->dst.dip.ipv6 = dip;
    memset(&sipv6, 0, sizeof(sipv6));
    if (memcmp(&sip, &sipv6, sizeof(sipv6)) == 0) {
        /* Zero SIP indicates ASM */
        ipmc->src.fid = vid;
    } else {
        ipmc->src.ssm = 1;
    }
}

vtss_rc vtss_cmn_ipv6_mc_add(vtss_state_t      *vtss_state,
                             const vtss_vid_t  vid,
                             const vtss_ipv6_t sip,
                             const vtss_ipv6_t dip,
                             const BOOL        member[VTSS_PORT_ARRAY_SIZE])
{
    vtss_ipmc_data_t ipmc;
    vtss_port_no_t   port_no;

    vtss_cmn_ipv6_mc_data_init(&ipmc, vid, sip, dip);
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        VTSS_PORT_BF_SET(ipmc.dst.member, port_no, member[port_no]);
    }
    return vtss_cmn_ip_mc_add(vtss_state, &ipmc);
}

vtss_rc vtss_cmn_ipv6_mc_del(vtss_state_t      *vtss_state,
                             const vtss_vid_t  vid,
                             const vtss_ipv6_t sip,
                             const vtss_ipv6_t dip)
{
    vtss_ipmc_data_t ipmc;

    vtss_cmn_ipv6_mc_data_init(&ipmc, vid, sip, dip);
    return vtss_cmn_ip_mc_del(vtss_state, &ipmc);
}
#endif /* VTSS_FEATURE_IPV4_MC_SIP || VTSS_FEATURE_IPV6_MC_SIP */

#if defined(VTSS_FEATURE_VCL)
/* Add VCE */
vtss_rc vtss_cmn_vce_add(vtss_state_t *vtss_state, const vtss_vce_id_t vce_id, const vtss_vce_t *const vce)
{
    vtss_vcap_obj_t             *obj = vtss_vcap_is1_obj_get(vtss_state);
    vtss_vcap_user_t            user = VTSS_IS1_USER_VCL;
    vtss_vcap_data_t            data;
    vtss_is1_data_t             *is1 = &data.u.is1;
    vtss_is1_entry_t            entry;
    vtss_is1_action_t           *action = &entry.action;
    vtss_is1_key_t              *key = &entry.key;
    u32                         i;
    vtss_res_chg_t              res_chg;
    vtss_vcap_key_size_t        key_size = VTSS_VCAP_KEY_SIZE_FULL;
    vtss_vcap_range_chk_table_t range_new = vtss_state->vcap.range; /* Make a temporary working copy of the range table */

    /* Check VCE ID */
    if (vce->id == VTSS_VCE_ID_LAST || vce->id == vce_id) {
        VTSS_E("illegal vce id: %u", vce->id);
        return VTSS_RC_ERROR;
    }

    /* Initialize entry data */
    vtss_vcap_is1_init(&data, &entry);

    /* First Lookup */
    is1->lookup = 0;

#if defined(VTSS_ARCH_SERVAL)
    /* For Serval, the default half key is used */
    if (vtss_state->arch == VTSS_ARCH_SRVL) {
        is1->lookup = 1; /* Second lookup */
        key_size = VTSS_VCAP_KEY_SIZE_HALF;
        key->key_type = VTSS_VCAP_KEY_TYPE_NORMAL;
    }
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_2)
    if (vtss_state->arch == VTSS_ARCH_JR2) {
        /* Jaguar-2, use half rule. For revision A, a full row is consumed */
        key->key_type = VTSS_VCAP_KEY_TYPE_NORMAL;
        key_size = (vtss_state->misc.jr2_a ? VTSS_VCAP_KEY_SIZE_FULL : VTSS_VCAP_KEY_SIZE_HALF);
    }
#endif /* VTSS_ARCH_JAGUAR_2 */

    /* Check if main entry exists */
    memset(&res_chg, 0, sizeof(res_chg));
    if (vtss_vcap_lookup(vtss_state, obj, user, vce->id, &data, NULL) == VTSS_RC_OK) {
        is1->entry = &entry; /* NOTE: Restore entry pointer which was overwritten by vtss_vcap_lookup() */
        res_chg.del_key[key_size] = 1;
        /* Free eventually old range checkers */
        VTSS_RC(vtss_vcap_range_free(&range_new, is1->vid_range));
        VTSS_RC(vtss_vcap_range_free(&range_new, is1->dscp_range));
        VTSS_RC(vtss_vcap_range_free(&range_new, is1->sport_range));
        VTSS_RC(vtss_vcap_range_free(&range_new, is1->dport_range));
        is1->vid_range = VTSS_VCAP_RANGE_CHK_NONE;
        is1->dscp_range = VTSS_VCAP_RANGE_CHK_NONE;
        is1->sport_range = VTSS_VCAP_RANGE_CHK_NONE;
        is1->dport_range = VTSS_VCAP_RANGE_CHK_NONE;
    }

    /* Check that the entry can be added */
    res_chg.add_key[key_size] = 1;
    VTSS_RC(vtss_cmn_vcap_res_check(obj, &res_chg));
    data.key_size = key_size;

    /* Copy action data */
    action->vid = vce->action.vid;
    if (vce->action.policy_no != VTSS_ACL_POLICY_NO_NONE) {
        action->pag_enable = TRUE;
        action->pag = vce->action.policy_no;
    }

    /* Copy key data */
    memcpy(key->port_list, vce->key.port_list, sizeof(key->port_list));

    key->mac.dmac_mc = vce->key.mac.dmac_mc;
    key->mac.dmac_bc = vce->key.mac.dmac_bc;
    for (i = 0; i < 6; i++) {
        key->mac.smac.value[i] = vce->key.mac.smac.value[i];
        key->mac.smac.mask[i]  = vce->key.mac.smac.mask[i];
    }

    key->tag.vid.type = VTSS_VCAP_VR_TYPE_VALUE_MASK;
    key->tag.vid.vr.v.value = vce->key.tag.vid.value;
    key->tag.vid.vr.v.mask = vce->key.tag.vid.mask;
    key->tag.pcp    = vce->key.tag.pcp;
    key->tag.dei    = vce->key.tag.dei;
    key->tag.tagged = vce->key.tag.tagged;

    switch (vce->key.type) {
    case VTSS_VCE_TYPE_ANY:
        key->type = VTSS_IS1_TYPE_ANY;
        break;
    case VTSS_VCE_TYPE_ETYPE:
        key->type              = VTSS_IS1_TYPE_ETYPE;
        key->frame.etype.etype = vce->key.frame.etype.etype;
        key->frame.etype.data  = vce->key.frame.etype.data;
        break;
    case VTSS_VCE_TYPE_LLC:
        key->type           = VTSS_IS1_TYPE_LLC;
        key->frame.llc.data = vce->key.frame.llc.data;
        break;
    case VTSS_VCE_TYPE_SNAP:
        key->type            = VTSS_IS1_TYPE_SNAP;
        key->frame.snap.data = vce->key.frame.snap.data;
        break;
    case VTSS_VCE_TYPE_IPV4:
    case VTSS_VCE_TYPE_IPV6:
        if (vce->key.type == VTSS_VCE_TYPE_IPV4) {
            key->type                = VTSS_IS1_TYPE_IPV4;
            key->frame.ipv4.fragment = vce->key.frame.ipv4.fragment;
            key->frame.ipv4.dscp     = vce->key.frame.ipv4.dscp;
            VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->dscp_range, VTSS_VCAP_RANGE_TYPE_DSCP, &key->frame.ipv4.dscp));
            key->frame.ipv4.proto    = vce->key.frame.ipv4.proto;
            key->frame.ipv4.sip      = vce->key.frame.ipv4.sip;
            //key->frame.ipv4.sport    = vce->key.frame.ipv4.sport;
            //VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->sport_range, VTSS_VCAP_RANGE_TYPE_SPORT, &key->frame.ipv4.sport));
            key->frame.ipv4.dport    = vce->key.frame.ipv4.dport;
            VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->dport_range, VTSS_VCAP_RANGE_TYPE_DPORT, &key->frame.ipv4.dport));
        } else {
            key->type                = VTSS_IS1_TYPE_IPV6;
            key->frame.ipv6.dscp     = vce->key.frame.ipv6.dscp;
            VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->dscp_range, VTSS_VCAP_RANGE_TYPE_DSCP, &key->frame.ipv6.dscp));
            key->frame.ipv6.proto    = vce->key.frame.ipv6.proto;
            for (i = 0; i < 4; i++) {
                key->frame.ipv6.sip.value[i + 12] = vce->key.frame.ipv6.sip.value[i];
                key->frame.ipv6.sip.mask[i + 12] = vce->key.frame.ipv6.sip.mask[i];
            }
            //key->frame.ipv6.sport    = vce->key.frame.ipv6.sport;
            //VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->sport_range, VTSS_VCAP_RANGE_TYPE_SPORT, &key->frame.ipv6.sport));
            key->frame.ipv6.dport    = vce->key.frame.ipv6.dport;
            VTSS_RC(vtss_vcap_vr_alloc(&range_new, &is1->dport_range, VTSS_VCAP_RANGE_TYPE_DPORT, &key->frame.ipv6.dport));
        }
        break;
    default:
        VTSS_E("illegal type: %d", vce->key.type);
        return VTSS_RC_ERROR;
    }

    /* Commit range checkers */
    VTSS_RC(vtss_vcap_range_commit(vtss_state, &range_new));

    /* Add main entry */
    VTSS_RC(vtss_vcap_add(vtss_state, obj, user, vce->id, vce_id, &data, 0));
    return VTSS_RC_OK;
}

/* Delete VCE */
vtss_rc vtss_cmn_vce_del(vtss_state_t *vtss_state, const vtss_vce_id_t vce_id)
{
    vtss_vcap_obj_t  *obj = vtss_vcap_is1_obj_get(vtss_state);
    vtss_vcap_user_t user = VTSS_IS1_USER_VCL;
    vtss_vcap_data_t data;
    vtss_is1_data_t  *is1 = &data.u.is1;

    if (vtss_vcap_lookup(vtss_state, obj, user, vce_id, &data, NULL) != VTSS_RC_OK) {
        /* This is possible as add may fail in some situations due to hardware limitation */
        VTSS_D("vce_id: %u not found", vce_id);
        return VTSS_RC_OK;
    }
    
    /* Delete range checkers and main entry */
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->vid_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->dscp_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->sport_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->dport_range));
    VTSS_RC(vtss_vcap_del(vtss_state, obj, user, vce_id));
    
    return VTSS_RC_OK;
}
#endif /* VTSS_FEATURE_VCL */

#if defined(VTSS_FEATURE_VLAN_TRANSLATION)
static void vtss_debug_print_vlan_trans(vtss_state_t *vtss_state,
                                        const vtss_debug_printf_t pr,
                                        const vtss_debug_info_t   *const info)
{
    vtss_vlan_trans_grp2vlan_conf_t conf;
    vtss_vlan_trans_port2grp_conf_t port_conf;
    BOOL                            next = FALSE, first = TRUE;
    pr("VLAN Translation Group Table\n");
    pr("----------------------------\n");
    memset(&conf, 0, sizeof(vtss_vlan_trans_grp2vlan_conf_t));
    while ((vtss_cmn_vlan_trans_group_get(vtss_state, &conf, next)) == VTSS_RC_OK) {
        if (first == TRUE) {
            pr("GroupID  VID   Trans_VID\n");
            pr("-------  ---   ---------\n");
            first = FALSE;
        }
        pr("%-7u  %-4u  %-4u\n", conf.group_id, conf.vid, conf.trans_vid);
        next = TRUE;
    }
    pr("\nVLAN Translation Port Table\n");
    pr("---------------------------\n");
    next = FALSE;
    first = TRUE;
    //vtss_cmn_vlan_trans_port_conf_get(vtss_vlan_trans_port2grp_conf_t *conf, BOOL next);
    memset(&port_conf, 0, sizeof(vtss_vlan_trans_port2grp_conf_t));
    while ((vtss_cmn_vlan_trans_port_conf_get(vtss_state, &port_conf, next)) == VTSS_RC_OK) {
        if (first == TRUE) {
            pr("GroupID  Ports\n");
            pr("-------  -----\n");
            first = FALSE;
        }
        pr("%-7u  ", port_conf.group_id);
        vtss_debug_print_ports(vtss_state, pr, port_conf.ports, 1);
        next = TRUE;
    }

    pr("\n");
#if defined(VTSS_FEATURE_IS1) || defined(VTSS_FEATURE_CLM)
    vtss_vcap_debug_print_is1(vtss_state, pr, info);
#endif /* VTSS_FEATURE_IS1/CLM */
    vtss_vcap_debug_print_es0(vtss_state, pr, info);
}

/* VTE(VLAN Translation Entry) ID */
static vtss_vcap_id_t vtss_vt_is1_vte_id_get(const vtss_vid_t vid, const u16 group_id)
{
    return ((group_id << 12) + vid);
}

/* Add IS1 TCAM entry for the VLAN Translation */
static vtss_rc vtss_vt_is1_entry_add(vtss_state_t *vtss_state,
                                     const u64 vte_id, const vtss_vid_t vid, 
                                     const vtss_vid_t trans_vid, const u8 *ports)
{
    vtss_vcap_obj_t             *obj = vtss_vcap_is1_obj_get(vtss_state);
    vtss_vcap_user_t            user = VTSS_IS1_USER_VLAN;
    vtss_vcap_data_t            data;
    vtss_is1_data_t             *is1 = &data.u.is1;
    vtss_is1_entry_t            entry;
    vtss_is1_action_t           *action = &entry.action;
    vtss_is1_key_t              *key = &entry.key;
    vtss_vcap_key_size_t        key_size = VTSS_VCAP_KEY_SIZE_FULL;
    u32                         i;
    BOOL                        port_list[VTSS_PORT_ARRAY_SIZE];

    VTSS_D("vtss_vt_is1_entry_add - vte_id: 0x%" PRIx64 ", vid: %u, trans_vid: %u", vte_id, vid, trans_vid);
    /* Initialize entry data */
    vtss_vcap_is1_init(&data, &entry);
    
    /* First Lookup */
    is1->lookup = 0;

#if defined(VTSS_ARCH_SERVAL)
    /* For Serval, the default half key is used */
    if (vtss_state->arch == VTSS_ARCH_SRVL) {
        is1->lookup = 1; /* Second lookup */
        key_size = VTSS_VCAP_KEY_SIZE_HALF;
        key->key_type = VTSS_VCAP_KEY_TYPE_NORMAL;
    }
#endif /* VTSS_ARCH_SERVAL */
#if defined(VTSS_ARCH_JAGUAR_2)
    if (vtss_state->arch == VTSS_ARCH_JR2) {
        /* Jaguar-2, use half rule. For revision A, a full row is consumed */
        key->key_type = VTSS_VCAP_KEY_TYPE_NORMAL;
        key_size = (vtss_state->misc.jr2_a ? VTSS_VCAP_KEY_SIZE_FULL : VTSS_VCAP_KEY_SIZE_HALF);
    }
#endif /* VTSS_ARCH_JAGUAR_2 */
    data.key_size = key_size;

    /* Copy action data */
    action->vid = trans_vid;
    /* Convert Port Bit field to Ports */
    for (i = 0; i < VTSS_PORT_ARRAY_SIZE; i++) {
        port_list[i] = ((ports[i/8]) & (1 << (i % 8))) ? TRUE : FALSE;
    }
    /* Copy key data */
    memcpy(key->port_list, port_list, sizeof(key->port_list));
    /* VID is of type value/mask */
    key->tag.vid.type = VTSS_VCAP_VR_TYPE_VALUE_MASK;
    key->tag.vid.vr.v.value = vid;
    key->tag.vid.vr.v.mask = 0xFFFF;
    /* Allow only for tagged frames */
    key->tag.tagged = VTSS_VCAP_BIT_1;
    key->type = VTSS_IS1_TYPE_ANY;

    /* Add main entry */
    VTSS_RC(vtss_vcap_add(vtss_state, obj, user, vte_id, VTSS_VCAP_ID_LAST, &data, 0));
    return VTSS_RC_OK;
}
/* Delete IS1 TCAM entry for the VLAN Translation */
static vtss_rc vtss_vt_is1_entry_del(vtss_state_t *vtss_state, const u64 vte_id)
{
    vtss_vcap_obj_t  *obj = vtss_vcap_is1_obj_get(vtss_state);
    vtss_vcap_user_t user = VTSS_IS1_USER_VLAN;
    vtss_vcap_data_t data;
    vtss_is1_data_t  *is1 = &data.u.is1;

    if (vtss_vcap_lookup(vtss_state, obj, user, vte_id, &data, NULL) != VTSS_RC_OK) {
        VTSS_E("vte_id: %" PRIu64 " not found", vte_id);
        return VTSS_RC_ERROR;
    }
    /* Delete range checkers and main entry */
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->vid_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->dscp_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->sport_range));
    VTSS_RC(vtss_vcap_range_free(&vtss_state->vcap.range, is1->dport_range));
    VTSS_RC(vtss_vcap_del(vtss_state, obj, user, vte_id));
    return VTSS_RC_OK;
}

/* ES0 index derived from VID (11:0 bits) and Port number (64:12 bits) */ 
static vtss_vcap_id_t vtss_vt_es0_vte_id_get(const vtss_vid_t vid, const vtss_port_no_t port_no)
{
    vtss_vcap_id_t id = port_no;

    return ((id << 12) + vid);
}

/* VLAN Translation function to add entry to ES0 */
static vtss_rc vtss_vt_es0_entry_add(vtss_state_t *vtss_state,
                                     const vtss_vid_t trans_vid, const vtss_vid_t vid,
                                     const vtss_port_no_t port_no)
{
    vtss_vcap_data_t data;
    vtss_es0_entry_t entry;
    vtss_vcap_id_t   id;

    id = vtss_vt_es0_vte_id_get(vid, port_no);
    vtss_cmn_es0_data_set(vtss_state, port_no, trans_vid, vid, 1, &data, &entry);

    return vtss_vcap_add(vtss_state, &vtss_state->vcap.es0.obj, VTSS_ES0_USER_VLAN, id, VTSS_VCAP_ID_LAST, &data, 0);
}
/* VLAN Translation function to delete entry to ES0 */
static vtss_rc vtss_vt_es0_entry_del(vtss_state_t *vtss_state,
                                     const vtss_vid_t vid, const vtss_port_no_t port_no)
{
    vtss_vcap_id_t id;

    id = vtss_vt_es0_vte_id_get(vid, port_no);
    return vtss_vcap_del(vtss_state, &vtss_state->vcap.es0.obj, VTSS_ES0_USER_VLAN, id);
}
/* VLAN Translation function to check if group already exists in VLAN Translation list */
static vtss_rc vtss_vlan_trans_group_list_entry_exists(vtss_state_t *vtss_state,
                                                       const u16 group_id, const vtss_vid_t vid, 
                                                       BOOL *exists)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;

    *exists = FALSE;
    list = &vtss_state->l2.vt_trans_conf;
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if ((group_id == tmp->conf.group_id) && (vid == tmp->conf.vid)) {
            *exists = TRUE;
        }
    }
    return VTSS_RC_OK;
}
/* Helper function to calculate number of set bits in a byte */
static u8 bitcount(u8 n)
{
    u8 cnt = 0;
    while (n)
    {
        n &= (n-1);
        cnt++;
    }
    return cnt;
}
/* Add entry to the VLAN Translation group list */
static vtss_rc vtss_vlan_trans_group_list_add(vtss_state_t *vtss_state,
                                              const u16 group_id, const vtss_vid_t vid, 
                                              const vtss_vid_t trans_vid)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp, *prev, *new = NULL;
    BOOL                                update = FALSE;

    list = &vtss_state->l2.vt_trans_conf;
    /* Insert the new node into the used list */
    for (tmp = list->used, prev = NULL; tmp != NULL; prev = tmp, tmp = tmp->next) {
        /* Check to see if the vid is already configured for this group */
        if ((group_id == tmp->conf.group_id) && (vid == tmp->conf.vid)) {
            /* Overwrite the trans_vid */
            tmp->conf.trans_vid = trans_vid;
            update = TRUE;
            break;
        }
        /* List needs to be sorted based on group_id */
        if (group_id > tmp->conf.group_id) {
            break;
        }
    }
    if (update == FALSE) {
        /* Get free node from free list */
        new = list->free;
        if (new == NULL) {  /* No free entry exists */
            return VTSS_RC_ERROR;
        }   
        /* Update the free list */
        list->free = new->next;
        /* Copy the configuration */
        new->conf.group_id = group_id;
        new->conf.vid = vid;
        new->conf.trans_vid = trans_vid;
        new->next = NULL;
        /* Group to VLAN mapping can be added in to the used list in three ways:
           1. Add at the beginning of the list;
           2. Add somewhere in the middle of the list;
           3. Add at the end of the list.
         */
        if (tmp != NULL) { /* This means insertion point found */
            if (prev == NULL) { 
                /* Add at the beginning of the list */
                new->next = list->used;
                list->used = new;
            } else {
                /* Add somewhere in the middle of the list */
                prev->next = new;
                new->next = tmp;
            }
        } else { /* insertion point not found; add at the end of the list */
            if (prev == NULL) { /* used list is empty */
                list->used = new;
            } else {
                prev->next = new;
            } /* if (prev == NULL) */
        } /* if (tmp != NULL) */
    } /* if (update == FALSE) */
    return VTSS_RC_OK;
}
/* Delete entry to the VLAN Translation group list */
static vtss_rc vtss_vlan_trans_group_list_del(vtss_state_t *vtss_state,
                                              const u16 group_id, const vtss_vid_t vid)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp, *prev;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used, prev = NULL; tmp != NULL; prev = tmp, tmp = tmp->next) {
        if ((group_id == tmp->conf.group_id) && (vid == tmp->conf.vid)) {
            break;
        }
    }
    if (tmp != NULL) { /* This means deletion point found */
        if (prev == NULL) { /* Delete the first node */
            /* Remove from the used list */
            list->used = tmp->next;
        } else {
            /* Remove from the used list */
            prev->next = tmp->next;
        }
        /* Add to the free list */
        tmp->next = list->free;
        list->free = tmp;
    }
    return ((tmp != NULL) ? VTSS_RC_OK : VTSS_RC_ERROR);
}
static vtss_rc vtss_vlan_trans_group_list_get(vtss_state_t *vtss_state,
                                              const u16 group_id, const vtss_vid_t vid,
                                              vtss_vlan_trans_grp2vlan_conf_t *conf)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if ((group_id == tmp->conf.group_id) && (vid == tmp->conf.vid)) {
            *conf = tmp->conf;
            break;
        }
    }
    return ((tmp != NULL) ? VTSS_RC_OK : VTSS_RC_ERROR);
}
/* VLAN Translation function to add IS1 and ES0 entries for a group */
static void vtss_vlan_trans_group_hw_entries_add(vtss_state_t *vtss_state,
                                                 const u16 group_id, u8 *ports)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;
    u64                                 vte_id = 0;
    u32                                 i, j;
    vtss_port_no_t                      port_no;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if (group_id == tmp->conf.group_id) {
            vte_id = vtss_vt_is1_vte_id_get(tmp->conf.vid, group_id);
            if ((vtss_vt_is1_entry_add(vtss_state, vte_id, tmp->conf.vid, tmp->conf.trans_vid, ports)) != VTSS_RC_OK) {
                VTSS_D("vtss_vlan_trans_group_hw_entries_add: IS1 entry add failed");
            } /* if ((vtss_vt_is1_entry_add */
            /* Adding ES0 TCAM entry for egress VLAN Translation */
            for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
                for (j = 0; j < 8; j++) {
                    if (ports[i] & (1 << j)) {
                        port_no = (i * 8) + j;
                        if (vtss_vt_es0_entry_add(vtss_state, tmp->conf.trans_vid, tmp->conf.vid, port_no)) {
                            VTSS_D("vtss_vlan_trans_group_hw_entries_add: ES0 entry add failed");
                        } /* if (vtss_vt_es0_entry_add */
                    } /* if (ports[i] */
                } /* for (j = 0; j < 8; j++)  */
            } /* for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) */
        } /* if (group_id == tmp->conf.group_id) */
    } /* for (tmp = list->used, prev = NULL */
}
/* VLAN Translation function to delete IS1 and ES0 entries for a group */
static void vtss_vlan_trans_group_hw_entries_del(vtss_state_t *vtss_state,
                                                 const u16 group_id, u8 *ports)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;
    u64                                 vte_id = 0;
    u32                                 i, j;
    vtss_port_no_t                      port_no;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if (group_id == tmp->conf.group_id) {
            vte_id = vtss_vt_is1_vte_id_get(tmp->conf.vid, group_id);
            if ((vtss_vt_is1_entry_del(vtss_state, vte_id)) != VTSS_RC_OK) {
                VTSS_D("vtss_vlan_trans_group_hw_entries_del: IS1 entry delete failed");
            } /* if ((vtss_vt_is1_entry_del( */
            /* Delete ES0 TCAM entries for egress VLAN Translation */
            for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
                for (j = 0; j < 8; j++) {
                    if (ports[i] & (1 << j)) {
                        port_no = (i * 8) + j;
                        if (vtss_vt_es0_entry_del(vtss_state, tmp->conf.trans_vid, port_no) != VTSS_RC_OK) {
                            VTSS_D("vtss_vlan_trans_group_hw_entries_del: ES0 entry delete failed");
                        } /* if (vtss_vt_es0_entry_del */
                    } /* if (ports[i] & (1 << j)) */
                } /* for (j = 0; j < 8; j++) */
            } /* for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) */
        } /* if (group_id == tmp->conf.group_id) */
    } /* for (tmp = list->used; */
}
/* VLAN Translation helper function to update all the IS1 and ES0 entries based on port list */
static vtss_rc vtss_vlan_trans_group_port_list_update(vtss_state_t *vtss_state,
                                                      const u16 group_id, const u8 *ports, 
                                                      const u8 *new_ports, const BOOL update)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;
    u64                                 vte_id = 0;
    u32                                 i, j;
    vtss_port_no_t                      port_no;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if (group_id == tmp->conf.group_id) {
            vte_id = vtss_vt_is1_vte_id_get(tmp->conf.vid, group_id);
            if (update == TRUE) {
                if ((vtss_vt_is1_entry_del(vtss_state, vte_id)) != VTSS_RC_OK) {
                    VTSS_D("vtss_vlan_trans_group_port_list_update: IS1 entry delete failed");
                } /* if ((vtss_vt_is1_entry_del( */
                /* Delete ES0 TCAM entries for egress VLAN Translation */
                for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
                    for (j = 0; j < 8; j++) {
                        if (ports[i] & (1 << j)) {
                            port_no = (i * 8) + j;
                            if (vtss_vt_es0_entry_del(vtss_state, tmp->conf.trans_vid, port_no)) {
                                VTSS_D("vtss_vlan_trans_group_port_list_update: ES0 entry delete failed");
                            }
                        } /* if (ports[i] */
                    } /* for (j = 0; j < 8; */
                } /* for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE */
            } /* if (update == TRUE */
            if ((vtss_vt_is1_entry_add(vtss_state, vte_id, tmp->conf.vid, tmp->conf.trans_vid, 
                                       new_ports)) != VTSS_RC_OK) {
                VTSS_D("vtss_vlan_trans_group_port_list_update: IS1 entry addition failed");
            } /* if ((vtss_vt_is1_entry_add( */
            /* Adding ES0 TCAM entry for egress VLAN Translation */
            for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
                for (j = 0; j < 8; j++) {
                    if (new_ports[i] & (1 << j)) {
                        port_no = (i * 8) + j;
                        if (vtss_vt_es0_entry_add(vtss_state, tmp->conf.trans_vid, tmp->conf.vid, port_no)) {
                            VTSS_D("vtss_vlan_trans_group_port_list_update: ES0 entry addition failed");
                        } /* if (vtss_vt_es0_entry_add */
                    } /* if (new_ports[i] */
                } /* for (j = 0; j < 8; j++) */
            } /* for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE */
        } /* if (group_id == tmp->conf.group_id) */
    } /* for (tmp = list->used; */
    return VTSS_RC_OK;
}
/* VLAN Translation function to fetch port_list for this group */
static vtss_rc vtss_vlan_trans_port_list_get(vtss_state_t *vtss_state,
                                             const u16 group_id, u8 *ports)
{
    vtss_vlan_trans_port2grp_t          *list;
    vtss_vlan_trans_port2grp_entry_t    *tmp;
    BOOL                                found = FALSE;
    list = &vtss_state->l2.vt_port_conf;
    memset(ports, 0, VTSS_VLAN_TRANS_PORT_BF_SIZE);
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if (tmp->conf.group_id == group_id) {
            memcpy(ports, tmp->conf.ports, VTSS_VLAN_TRANS_PORT_BF_SIZE);
            found = TRUE;
            break;
        }
    }
    return ((found == TRUE) ? VTSS_RC_OK : VTSS_RC_ERROR);
}
static void vtss_vlan_trans_group_trans_cnt(vtss_state_t *vtss_state,
                                            const u16 group_id, u32 *count)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;
    u32                                 cnt = 0;

    list = &vtss_state->l2.vt_trans_conf;
    /* Search used list to find out matching entry */
    for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
        if (group_id == tmp->conf.group_id) {
            cnt++;
        }
    }
    *count = cnt;
}
static vtss_rc vtss_vlan_trans_res_check(vtss_state_t *vtss_state,
                                         const u16 group_id, const u8 *ports)
{
    vtss_vlan_trans_port2grp_t          *port_list;
    vtss_vlan_trans_port2grp_entry_t    *tmp;
    BOOL                                del_entry, grp_found = FALSE;
    u8                                  tmp_ports[VTSS_PORT_BF_SIZE];
    u32                                 i, k, port_cnt, tmp_cnt, grp_cnt = 0;
    u32                                 orig_port_cnt = 0;
    u32                                 is1_add = 0, is1_del = 0, es0_add = 0, es0_del = 0;
    vtss_res_t                          res;

    port_list = &vtss_state->l2.vt_port_conf;
    for (tmp = port_list->used; tmp != NULL; tmp = tmp->next) {
        memcpy(tmp_ports, tmp->conf.ports, VTSS_PORT_BF_SIZE);
        if (group_id != tmp->conf.group_id) {
            port_cnt = 0;
            tmp_cnt = 0;
            del_entry = FALSE;
            for (i = 0; i < VTSS_PORT_BF_SIZE; i++) {
                for (k = 0; k < 8; k++) {
                    if ((ports[i] & (1 << k)) && (tmp->conf.ports[i] & (1 << k))) {
                        port_cnt++;
                    }
                }
                /* Remove the ports */
                tmp_ports[i] &= ~(ports[i]);
                if (tmp_ports[i] == 0) {
                    tmp_cnt++;
                } /* if (tmp_ports[i] == 0) */
            } /* for (i = 0; i < VTSS_PORT_BF_SIZE; */
            /* Check if entry needs to be deleted */
            if (tmp_cnt == VTSS_PORT_BF_SIZE) {
                del_entry = TRUE;
            }
            if (port_cnt != 0) {
                /* Get number of VLAN Translations defined for this group */
                vtss_vlan_trans_group_trans_cnt(vtss_state, tmp->conf.group_id, &grp_cnt);
                if ((del_entry == TRUE) && (grp_cnt != 0)) {
                    is1_del++;
                }
                es0_del += (port_cnt * grp_cnt);
            } /* if (port_cnt != 0) */
        } else {
            grp_found = TRUE;
            orig_port_cnt = 0;
            port_cnt = 0;
            for (i = 0; i < VTSS_PORT_BF_SIZE; i++) {
                orig_port_cnt += bitcount(tmp->conf.ports[i]);
                tmp_ports[i] = tmp->conf.ports[i] | ports[i];
                port_cnt += bitcount(tmp_ports[i]);
            }
            /* Get number of VLAN Translations defined for this group */
            vtss_vlan_trans_group_trans_cnt(vtss_state, group_id, &grp_cnt);
            VTSS_D("grp_cnt = %u, port_cnt = %u, orig_port_cnt = %u", grp_cnt, port_cnt, orig_port_cnt);
            /* IS1 entry will only get updated */
            es0_add += (grp_cnt * (port_cnt - orig_port_cnt));
        } /* if (group_id != tmp->conf.group_id) */
    } /* for (tmp = port_list->used; tmp != NULL */
    if (grp_found == FALSE) {
        port_cnt = 0;
        for (i = 0; i < VTSS_PORT_BF_SIZE; i++) {
            port_cnt += bitcount(ports[i]);
        }
        /* Get number of VLAN Translations defined for this group */
        vtss_vlan_trans_group_trans_cnt(vtss_state, group_id, &grp_cnt);
        is1_add += grp_cnt;
        VTSS_D("grp_cnt = %u, port_cnt = %u", grp_cnt, port_cnt);
        es0_add += grp_cnt * port_cnt;
    }
    VTSS_D("is1_add = %u, is1_del = %u, es0_add = %u, es0_del = %u\n", is1_add, is1_del, es0_add, es0_del);

    /* Resource check based on add/delete count */
    vtss_cmn_res_init(&res);
    if (vtss_state->arch == VTSS_ARCH_JR2) {
        res.clm_c.add = is1_add;
        res.clm_c.del = is1_del;
    } else {
        res.is1.add = is1_add;
        res.is1.del = is1_del;
    }
    res.es0.add = es0_add;
    res.es0.del = es0_del;
    return vtss_cmn_res_check(vtss_state, &res);
}
/* VLAN Translation function to update all the port to group mappings and delete if none 
   of the ports exist for a group */
static vtss_rc vtss_vlan_trans_port_list_del(vtss_state_t *vtss_state, const u8 *ports)
{
    vtss_vlan_trans_port2grp_t          *list;
    vtss_vlan_trans_port2grp_entry_t    *tmp, *prev;
    BOOL                                modified_entry;
    u8                                  orig_ports[VTSS_PORT_BF_SIZE];
    u32                                 i, j;

    list = &vtss_state->l2.vt_port_conf;
    /* Delete all the port to group mappings corresponding to the ports list */
    for (tmp = list->used, prev = NULL; tmp != NULL;) {
        modified_entry = FALSE;
        memcpy(orig_ports, tmp->conf.ports, VTSS_PORT_BF_SIZE);
        for (i = 0; i < VTSS_PORT_BF_SIZE; i++) {
            if (ports[i] & tmp->conf.ports[i]) {
                modified_entry = TRUE;
                /* Remove the ports */
                tmp->conf.ports[i] &= ~(ports[i]);
            } /* if (ports[i] & tmp->conf.ports[i]) */
        } /* for (i = 0; i < VTSS_PORT_BF_SIZE; i++) */
        /* As a result of port deletes, if an entry exists with no ports, delete the entry */
        if (modified_entry == TRUE) {
            vtss_vlan_trans_group_hw_entries_del(vtss_state, tmp->conf.group_id, orig_ports);
            for (i = 0, j = 0; i < VTSS_PORT_BF_SIZE; i++) {
                if (tmp->conf.ports[i] == 0) {
                    j++;
                }
            }
            if (j == VTSS_PORT_BF_SIZE) { /* None of the ports is valid, so delete the entry */
                if (prev == NULL) { /* Delete the first node */
                    /* Remove from the used list */
                    list->used = tmp->next;
                } else {
                    /* Remove from the used list */
                    prev->next = tmp->next;
                }
                /* Add to the free list */
                tmp->next = list->free;
                list->free = tmp;
                /* Update tmp to continue the loop */
                if (prev == NULL) { /* This is the first node in used list */
                    tmp = list->used;
                    continue;
                } else {
                    tmp = prev;
                } /* if (prev == NULL) */
            } else { /* If some ports are valid, we need to add the IS1 and ES0 entries */
                vtss_vlan_trans_group_hw_entries_add(vtss_state, tmp->conf.group_id, tmp->conf.ports);
            } /* if (j == VTSS_PORT_BF_SIZE */
        } /* if (modified_entry == TRUE */
        prev = tmp;
        tmp = tmp->next;
    } /* for (tmp = list->used, */
    return VTSS_RC_OK;
}
/* VLAN Translation function to add VLAN Translation entry into a group */
vtss_rc vtss_cmn_vlan_trans_group_add(vtss_state_t *vtss_state,
                                      const u16 group_id, const vtss_vid_t vid, const vtss_vid_t trans_vid)
{
    u8                                  ports[VTSS_VLAN_TRANS_PORT_BF_SIZE], port_cnt = 0;
    u64                                 vte_id = 0;
    u32                                 i, j;
    vtss_port_no_t                      port_no;
    BOOL                                entry_exist = FALSE, ports_exist = TRUE;
    vtss_res_t                          res;

    memset(ports, 0, VTSS_VLAN_TRANS_PORT_BF_SIZE);
    /* Fetch port_list for this group */
    if ((vtss_vlan_trans_port_list_get(vtss_state, group_id, ports)) != VTSS_RC_OK) {
        ports_exist = FALSE;
        VTSS_D("No Port mapping for this group");
    }
    if (ports_exist == TRUE) {
        for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
            port_cnt += bitcount(ports[i]);
        }
        VTSS_I("VLAN Translation: Port Count = %u", port_cnt);
        VTSS_RC(vtss_vlan_trans_group_list_entry_exists(vtss_state, group_id, vid, &entry_exist));
        /* We need one IS1 entry and port_cnt number of ES0 VLAN Translations. Check for VCAP resources */
        if (entry_exist == FALSE) { /* If an entry already exists, we just need to update the entry */
            vtss_cmn_res_init(&res);
            if (vtss_state->arch == VTSS_ARCH_JR2) {
                res.clm_c.add = 1;
            } else {
                res.is1.add = 1;
            }
            res.es0.add = port_cnt;
            VTSS_RC(vtss_cmn_res_check(vtss_state, &res));
        }
    }
    /* Add to the state list */
    if ((vtss_vlan_trans_group_list_add(vtss_state, group_id, vid, trans_vid)) != VTSS_RC_OK) {
        return VTSS_RC_ERROR;
    }
    if (ports_exist == TRUE) {
        vte_id = vtss_vt_is1_vte_id_get(vid, group_id);
        /* Add IS1 TCAM entry for ingress VLAN Translation */
        if ((vtss_vt_is1_entry_add(vtss_state, vte_id, vid, trans_vid, ports)) != VTSS_RC_OK) {
            return VTSS_RC_ERROR;
        }
        /* Adding ES0 TCAM entry for egress VLAN Translation */
        for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
            for (j = 0; j < 8; j++) {
                if (ports[i] & (1 << j)) {
                    port_no = (i * 8) + j;
                    VTSS_RC(vtss_vt_es0_entry_add(vtss_state, trans_vid, vid, port_no));
                }
            }
        }
    }
    return VTSS_RC_OK;
}
/* VLAN Translation function to delete a VLAN Translation entry from a group */
vtss_rc vtss_cmn_vlan_trans_group_del(vtss_state_t *vtss_state,
                                      const u16 group_id, const vtss_vid_t vid)
{
    u8                                  ports[VTSS_VLAN_TRANS_PORT_BF_SIZE];
    u64                                 vte_id = 0;
    u32                                 i, j;
    vtss_port_no_t                      port_no;
    vtss_vlan_trans_grp2vlan_conf_t     conf;
    BOOL                                ports_exist = TRUE;

    memset(ports, 0, VTSS_VLAN_TRANS_PORT_BF_SIZE);
    /* Fetch port_list for this group */
    if ((vtss_vlan_trans_port_list_get(vtss_state, group_id, ports)) != VTSS_RC_OK) {
        ports_exist = FALSE;
        VTSS_D("No Port mapping for this group");
    }
    if (ports_exist == TRUE) {
        vte_id = vtss_vt_is1_vte_id_get(vid, group_id);
        /* Delete IS1 TCAM entry for ingress VLAN Translation */
        if ((vtss_vt_is1_entry_del(vtss_state, vte_id)) != VTSS_RC_OK) {
            return VTSS_RC_ERROR;
        }
        /* Get trans_vid to delete ES0 entry */
        VTSS_RC(vtss_vlan_trans_group_list_get(vtss_state, group_id, vid, &conf));
        /* Delete ES0 TCAM entries for egress VLAN Translation */
        for (i = 0; i < VTSS_VLAN_TRANS_PORT_BF_SIZE; i++) {
            for (j = 0; j < 8; j++) {
                if (ports[i] & (1 << j)) {
                    port_no = (i * 8) + j;
                    VTSS_RC(vtss_vt_es0_entry_del(vtss_state, vid, port_no));
                }
            }
        }
    }
    /* Delete from the state list */
    if ((vtss_vlan_trans_group_list_del(vtss_state, group_id, vid)) != VTSS_RC_OK) {
        return VTSS_RC_ERROR;
    }
    return VTSS_RC_OK;
}
/* VLAN Translation function to fetch a VLAN Translation entry for a group; next parameter is only valid 
   if both the group_id and vid are valid */
vtss_rc vtss_cmn_vlan_trans_group_get(vtss_state_t *vtss_state,
                                      vtss_vlan_trans_grp2vlan_conf_t *conf, BOOL next)
{
    vtss_vlan_trans_grp2vlan_t          *list;
    vtss_vlan_trans_grp2vlan_entry_t    *tmp;
    BOOL                                found = FALSE, next_entry = FALSE;
    if (conf == NULL) {
        VTSS_E("VLAN Translation: NULL pointer");
        return VTSS_RC_ERROR;
    }
    list = &vtss_state->l2.vt_trans_conf;
    if (list->used == NULL) {
        VTSS_D("Group list is empty");
        return VTSS_RC_ERROR;
    }
    /* If group_id is 0, return first entry */
    if (conf->group_id == VTSS_VLAN_TRANS_NULL_GROUP_ID) {
        *conf = list->used->conf;
        found = TRUE;
    } else {
        for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
            if (next_entry == TRUE) {
                *conf = tmp->conf;
                found = TRUE;
                break;
            }
            if (conf->group_id == tmp->conf.group_id) {
                /* Return first entry if vid is 0 or return matching entry */
                if ((conf->vid == 0) || (conf->vid == tmp->conf.vid)) {
                    if ((next == FALSE) || (conf->vid == 0)) {
                        *conf = tmp->conf;
                        found = TRUE;
                        break;
                    } else {
                        next_entry = TRUE;
                    } /* if (next == FALSE) */
                } /* if ((conf->vid == 0) || (conf->vid == tmp->conf.vid)) */
            } /* if (conf->group_id == tmp->conf.group_id) */
        } /* for (tmp = list->used; tmp != NULL; tmp = tmp->next) */
    } /* if (conf->group_id == VTSS_VLAN_TRANS_NULL_GROUP_ID) */
    return ((found == TRUE) ? VTSS_RC_OK : VTSS_RC_ERROR);
}
/* VLAN Translation function to associate ports to a group. Only one port can be part 
   of one group not multiple groups */
vtss_rc vtss_cmn_vlan_trans_port_conf_set(vtss_state_t *vtss_state,
                                          const vtss_vlan_trans_port2grp_conf_t *conf)
{
    vtss_vlan_trans_port2grp_t          *list;
    vtss_vlan_trans_port2grp_entry_t    *tmp, *prev, *new = NULL;
    u32                                 i;
    BOOL                                update_entry = FALSE;
    u8                                  orig_ports[VTSS_PORT_BF_SIZE];

    if ((vtss_vlan_trans_res_check(vtss_state, conf->group_id, conf->ports)) != VTSS_RC_OK) {
        VTSS_D("VLAN Translation: Not enough resources to add the entry");
        return VTSS_RC_ERROR;
    }
    if ((vtss_vlan_trans_port_list_del(vtss_state, conf->ports)) != VTSS_RC_OK) {
        VTSS_D("VLAN Translation: vtss_vlan_trans_port_list_del failed");
    }
    list = &vtss_state->l2.vt_port_conf;
    /* Insert the new node into the used list */
    for (tmp = list->used, prev = NULL; tmp != NULL; prev = tmp, tmp = tmp->next) {
        if (conf->group_id == tmp->conf.group_id) {
            memcpy(orig_ports, tmp->conf.ports, VTSS_PORT_BF_SIZE);
            for (i = 0; i < VTSS_PORT_BF_SIZE; i++) {
                tmp->conf.ports[i] |= conf->ports[i]; 
            }
            update_entry = TRUE;
            if (vtss_vlan_trans_group_port_list_update(vtss_state, conf->group_id, orig_ports,
                                                       tmp->conf.ports, TRUE) != VTSS_RC_OK)
            {
                VTSS_D("VLAN Translation: vtss_vlan_trans_group_port_list_update failed");
            }
            break;
        }
        /* List needs to be sorted based on group_id */
        if (conf->group_id > tmp->conf.group_id) {
            break;
        }
    }
    if (update_entry == FALSE) {
        /* Get free node from free list */
        new = list->free;
        if (new == NULL) {  /* No free entry exists */
            return VTSS_RC_ERROR;
        }   
        /* Update the free list */
        list->free = new->next;
        /* Copy the configuration */
        new->conf.group_id = conf->group_id;
        memcpy(new->conf.ports, conf->ports, VTSS_PORT_BF_SIZE);
        new->next = NULL;
        /* Port to Group mapping can be added in to the used list in three ways:
           1. Add at the beginning of the list;
           2. Add somewhere in the middle of the list;
           3. Add at the end of the list.
         */
        if (tmp != NULL) { /* This means insertion point found */
            if (prev == NULL) { 
                /* Add at the beginning of the list */
                new->next = list->used;
                list->used = new;
            } else {
                /* Add somewhere in the middle of the list */
                prev->next = new;
                new->next = tmp;
            }
        } else { /* insertion point not found; add at the end of the list */
            if (prev == NULL) { /* used list is empty */
                list->used = new;
            } else {
                prev->next = new;
            }
        }
        memset(orig_ports, 0, VTSS_PORT_BF_SIZE);
        if (vtss_vlan_trans_group_port_list_update(vtss_state, conf->group_id, orig_ports, 
                                                   conf->ports, FALSE) != VTSS_RC_OK)
        {
            VTSS_D("VLAN Translation: vtss_vlan_trans_group_port_list_update failed");
        }
    }
    return VTSS_RC_OK;
}
/* VLAN Translation function to fetch all ports for a group */
vtss_rc vtss_cmn_vlan_trans_port_conf_get(vtss_state_t *vtss_state,
                                          vtss_vlan_trans_port2grp_conf_t *conf, BOOL next)
{
    vtss_vlan_trans_port2grp_t          *list;
    vtss_vlan_trans_port2grp_entry_t    *tmp;
    BOOL                                found = FALSE, next_entry = FALSE;
    if (conf == NULL) {
        VTSS_E("VLAN Translation: NULL pointer");
        return VTSS_RC_ERROR;
    }
    list = &vtss_state->l2.vt_port_conf;
    if (list->used == NULL) {
        VTSS_D("Port list is empty");
        return VTSS_RC_ERROR;
    }
    /* If group_id is 0, return first entry */
    if (conf->group_id == VTSS_VLAN_TRANS_NULL_GROUP_ID) {
        *conf = list->used->conf;
        found = TRUE;
    } else {
        for (tmp = list->used; tmp != NULL; tmp = tmp->next) {
            if (next_entry == TRUE) {
                *conf = tmp->conf;
                found = TRUE;
                break;
            }
            if (conf->group_id == tmp->conf.group_id) {
                if (next == FALSE) {
                    *conf = tmp->conf;
                    found = TRUE;
                    break;
                } else {
                    next_entry = TRUE;
                } /* if (next == FALSE) */
            } /* if (conf->group_id == tmp->conf.group_id) */
        } /* for (tmp = list->used; tmp != NULL; tmp = tmp->next) */
    } /* if (conf->group_id == VTSS_VLAN_TRANS_NULL_GROUP_ID) */
    return ((found == TRUE) ? VTSS_RC_OK : VTSS_RC_ERROR);
}
#endif /* VTSS_FEATURE_VLAN_TRANSLATION */

/* - Debug print --------------------------------------------------- */

#if defined(VTSS_FEATURE_VLAN_COUNTERS)
/* Print counters in two columns */
static void vtss_debug_vlan_cnt(const vtss_debug_printf_t pr,
                                const char *name,
                                vtss_vlan_counter_types_t *cnt)
{
    char buf[80];

    sprintf(buf, "%s Packets:", name);
    pr("%-19s%19" PRIu64 "   ", buf, cnt->frames);
    sprintf(buf, "%s Octets:", name);
    pr("%-19s%19" PRIu64, buf, cnt->bytes);
    pr("\n");
}
#endif /* VTSS_FEATURE_VLAN_COUNTERS */

static void vtss_debug_print_vlan(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    /*lint --e{454, 455} */  // Due to the VTSS_EXIT_ENTER
    vtss_vid_t            vid;
    vtss_vlan_entry_t     *entry;
    BOOL                  header = 1;
    vtss_port_no_t        port_no;
    vtss_vlan_port_conf_t *conf;
    u8                    erps_discard[VTSS_PORT_BF_SIZE];

#if defined(VTSS_FEATURE_VLAN_PORT_V2)
    pr("S-tag Etype: 0x%04x\n\n", vtss_state->l2.vlan_conf.s_etype);
#endif /* VTSS_FEATURE_VLAN_PORT_V1 */
    
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            pr("Port  ");
#if defined(VTSS_FEATURE_VLAN_PORT_V1)
            pr("Aware  S-Tag  ");
#endif /* VTSS_FEATURE_VLAN_PORT_V1 */
#if defined(VTSS_FEATURE_VLAN_PORT_V2)
            pr("Type  ");
#endif /* VTSS_FEATURE_VLAN_PORT_V1 */
#if defined(VTSS_ARCH_SERVAL)
            pr("EType   ");
#endif /* VTSS_ARCH_SERVAL */
            pr("PVID  UVID  Frame  Filter\n");
        }
        conf = &vtss_state->l2.vlan_port_conf[port_no];
        pr("%-4u  ", port_no);
#if defined(VTSS_FEATURE_VLAN_PORT_V1)
        pr("%-5u  %-5u  ", conf->aware, conf->stag);
#endif /* VTSS_FEATURE_VLAN_PORT_V1 */
#if defined(VTSS_FEATURE_VLAN_PORT_V2)
        pr("%-4s  ", 
           conf->port_type == VTSS_VLAN_PORT_TYPE_UNAWARE ? "U" :
           conf->port_type == VTSS_VLAN_PORT_TYPE_C ? "C" :
           conf->port_type == VTSS_VLAN_PORT_TYPE_S ? "S" :
           conf->port_type == VTSS_VLAN_PORT_TYPE_S_CUSTOM ? "S_CU" : "?");
#endif /* VTSS_FEATURE_VLAN_PORT_V2 */
#if defined(VTSS_ARCH_SERVAL)
        pr("0x%04x  ", conf->s_etype);
#endif /* VTSS_ARCH_SERVAL */
        pr("%-4u  %-4u  %-5s  %u\n",
           conf->pvid, conf->untagged_vid,
           conf->frame_type == VTSS_VLAN_FRAME_ALL ? "all" :
           conf->frame_type == VTSS_VLAN_FRAME_TAGGED ? "tag" : "untag",
           conf->ingress_filter);
    }
    if (!header)
        pr("\n");
    header = 1;

    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        entry = &vtss_state->l2.vlan_table[vid];
        if (!entry->enabled && !info->full)
            continue;
        if (header) {
            pr("VID   ");
#if defined(VTSS_FEATURE_VLAN_SVL)
            pr("FID   ");
#endif /* VTSS_FEATURE_VLAN_SVL */
            vtss_debug_print_port_header(vtss_state, pr, "MSTI  Lrn  Mir  Iso  ", 0, 1);
            header = 0;
        }
        pr("%-6u", vid);
#if defined(VTSS_FEATURE_VLAN_SVL)
        pr("%-6u", entry->conf.fid);
#endif /* VTSS_FEATURE_VLAN_SVL */
        pr("%-6u%-5u%-5u%-5u", entry->msti, entry->conf.learning, entry->conf.mirror, entry->isolated);
        vtss_debug_print_ports(vtss_state, pr, entry->member, 0);
        pr(" <- VLAN Members\n");
        pr("%-27s", "");
#if defined(VTSS_FEATURE_VLAN_SVL)
        pr("%-6s", "");
#endif /* VTSS_FEATURE_VLAN_SVL */
        VTSS_PORT_BF_CLR(erps_discard);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
            VTSS_PORT_BF_SET(erps_discard, 
                             port_no, 
                             entry->erps_discard_cnt[port_no] ? 1 : 0);
        }
        vtss_debug_print_ports(vtss_state, pr, erps_discard, 0);
        pr(" <- ERPS Discard\n");

        /* Leave critical region briefly */
        VTSS_EXIT_ENTER();
    }
    if (!header)
        pr("\n");
#if defined(VTSS_FEATURE_VLAN_COUNTERS)
    for (vid = VTSS_VID_NULL; vid < VTSS_VIDS; vid++) {
        vtss_vlan_counters_t counters;

        entry = &vtss_state->l2.vlan_table[vid];
        if ((!entry->enabled && !info->full) || VTSS_FUNC(l2.vlan_counters_get, vid, &counters) != VTSS_RC_OK)
            continue;
        pr("VLAN ID %u Counters:\n\n", vid);
        vtss_debug_vlan_cnt(pr, "Unicast", &counters.rx_vlan_unicast);
        vtss_debug_vlan_cnt(pr, "Multicast", &counters.rx_vlan_multicast);
        vtss_debug_vlan_cnt(pr, "Broadcast", &counters.rx_vlan_broadcast);
        pr("\n");
    }
#endif /* VTSS_FEATURE_VLAN_COUNTERS */
}

static void vtss_debug_print_pvlan(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info)
{
    BOOL            header = 1;
    vtss_port_no_t  port_no;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            pr("Port  Isolation\n");
        }
        pr("%-6u%s\n", port_no, vtss_bool_txt(vtss_state->l2.isolated_port[port_no]));
    }
    if (!header)
        pr("\n");

#if defined(VTSS_FEATURE_PVLAN)
    vtss_debug_print_port_header(vtss_state, pr, "PVLAN  ", 0, 1);
    {
        vtss_pvlan_no_t pvlan_no;

        for (pvlan_no = VTSS_PVLAN_NO_START; pvlan_no < VTSS_PVLAN_NO_END; pvlan_no++) {
            pr("%-7u", pvlan_no);
            vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.pvlan_table[pvlan_no].member, 1);
        }
    }
    pr("\n");

    header = 1;
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            vtss_debug_print_port_header(vtss_state, pr, "APVLAN  ", 0, 1);
        }
        pr("%-6u  ", port_no);
        vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.apvlan_table[port_no], 1);
    }
    if (!header)
        pr("\n");
#endif /* VTSS_FEATURE_PVLAN */
}

void vtss_debug_print_mac_entry(const vtss_debug_printf_t pr,
                                const char *name,
                                BOOL *header,
                                vtss_mac_table_entry_t *entry,
                                u32 pgid)
{
    u8 *p = &entry->vid_mac.mac.addr[0];
    
    if (*header) {
        *header = 0;
        vtss_debug_print_header(pr, name);
        pr("VID   MAC                PGID  CPU  Locked  ");
#if defined(VTSS_FEATURE_VSTAX_V2)
        pr("Remote  UPSID  UPSPN");
#endif /* VTSS_FEATURE_VSTAX_V2 */
        pr("\n");
    }
    pr("%-4u  %02x-%02x-%02x-%02x-%02x-%02x  %-4u  %-3u  %-6u  ",
       entry->vid_mac.vid, p[0], p[1], p[2], p[3], p[4], p[5], 
       pgid, entry->copy_to_cpu, entry->locked);
#if defined(VTSS_FEATURE_VSTAX_V2)
    pr("%-6u  %-5u  %-5u", 
       entry->vstax2.remote_entry, entry->vstax2.upsid, entry->vstax2.upspn);
#endif /* VTSS_FEATURE_VSTAX_V2 */
    pr("\n");
}

static void vtss_debug_print_mac_table(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr,
                                       const vtss_debug_info_t   *const info)
{
    /*lint --e{454, 455} */  // Due to the VTSS_EXIT_ENTER
    vtss_mac_entry_t       *entry;
    vtss_vid_mac_t         vid_mac;
    vtss_mac_table_entry_t mac_entry;
    u8                     *p = &vid_mac.mac.addr[0];
    BOOL                   header = 1;
    u32                    pgid;

    vtss_debug_print_value(pr, "Age time", vtss_state->l2.mac_age_time);
    vtss_debug_print_value(pr, "MAC table size", sizeof(vtss_mac_entry_t)*VTSS_MAC_ADDRS);
    vtss_debug_print_value(pr, "MAC table maximum", vtss_state->l2.mac_table_max);
    vtss_debug_print_value(pr, "MAC table count", vtss_state->l2.mac_table_count);
    pr("\n");

    vtss_debug_print_port_header(vtss_state, pr, "Flood Members  ", 0, 1);
    pr("Unicast        ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.uc_flood, 1);
    pr("Multicast      ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.mc_flood, 1);
    pr("IPv4 MC        ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.ipv4_mc_flood, 1);
    pr("IPv6 MC        ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.ipv6_mc_flood, 1);
    pr("\n");

    /* MAC address table in state */
    for (entry = vtss_state->l2.mac_list_used; entry != NULL; entry = entry->next) {
        if (header) {
            vtss_debug_print_header(pr, "API Entries");
            vtss_debug_print_port_header(vtss_state, pr, "User  VID   MAC                CPU  ", 0, 1);
            header = 0;
        }
        vtss_mach_macl_set(&vid_mac, entry->mach, entry->macl);
        pr("%-6s%-6u%02x-%02x-%02x-%02x-%02x-%02x  %-5u",
           entry->user == VTSS_MAC_USER_NONE ? "None" :
           entry->user == VTSS_MAC_USER_SSM ? "SSM" :
           entry->user == VTSS_MAC_USER_ASM ? "ASM" : "?",
           vid_mac.vid, p[0], p[1], p[2], p[3], p[4], p[5], entry->cpu_copy);
        vtss_debug_print_ports(vtss_state, pr, entry->member, 1);
        VTSS_EXIT_ENTER();
    }
    if (!header)
        pr("\n");

    /* MAC address table in chip */
    header = 1;
    memset(&mac_entry, 0, sizeof(mac_entry));
    while (vtss_state->l2.mac_table_get_next != NULL &&
           vtss_state->l2.mac_table_get_next(vtss_state, &mac_entry, &pgid) == VTSS_RC_OK) {
        vtss_debug_print_mac_entry(pr, "Chip Entries", &header, &mac_entry, pgid);
        VTSS_EXIT_ENTER();
    }
    if (!header)
        pr("\n");
}

static void vtss_debug_print_aggr(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    vtss_port_no_t    port_no;
    u8                member[VTSS_PORT_BF_SIZE];
    vtss_aggr_mode_t  *mode;
    vtss_pgid_entry_t *pgid_entry;
    u32               pgid;
    BOOL              empty;
    vtss_aggr_no_t    aggr_no;

    VTSS_PORT_BF_CLR(member);
    vtss_debug_print_port_header(vtss_state, pr, "LLAG  ", 0, 1);
    for (aggr_no = VTSS_AGGR_NO_START; aggr_no < VTSS_AGGR_NO_END; aggr_no++) {
        pr("%-4u  ", aggr_no);
        VTSS_PORT_BF_CLR(member);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            VTSS_PORT_BF_SET(member, port_no, vtss_state->l2.port_aggr_no[port_no] == aggr_no);
        vtss_debug_print_ports(vtss_state, pr, member, 1);
    }
    pr("\n");

    vtss_debug_print_header(pr, "Mode");
    mode = &vtss_state->l2.aggr_mode;
    vtss_debug_print_value(pr, "SMAC", mode->smac_enable);
    vtss_debug_print_value(pr, "DMAC", mode->dmac_enable);
    vtss_debug_print_value(pr, "SIP/DIP", mode->sip_dip_enable);
    vtss_debug_print_value(pr, "SPORT/DPORT", mode->sport_dport_enable);
    pr("\n");

    vtss_debug_print_port_header(vtss_state, pr, "PGID  Count  Resv  CPU  Queue  ", 0, 1);
    for (pgid = 0; pgid < vtss_state->l2.pgid_count; pgid++) {
        pgid_entry = &vtss_state->l2.pgid_table[pgid];
        empty = pgid_entry->cpu_copy ? 0 : 1;
        for (port_no = VTSS_PORT_NO_START; empty && port_no < vtss_state->port_count; port_no++) {
            if (pgid_entry->member[port_no]) {
                empty = 0;
            }
        }
        if ((pgid_entry->references == 0 && !info->full) || (pgid > 50 && empty && !info->full)) {
            continue;
        }
        pr("%-4u  %-5u  %-4u  %-3u  %-5u  ", pgid, pgid_entry->references, pgid_entry->resv, pgid_entry->cpu_copy, pgid_entry->cpu_queue);

        vtss_debug_print_port_members(vtss_state, pr, pgid_entry->member, 1);
    }
    pr("\n");
    vtss_debug_print_value(pr, "PGID count", vtss_state->l2.pgid_count);
    vtss_debug_print_value(pr, "PGID drop", vtss_state->l2.pgid_drop);
    vtss_debug_print_value(pr, "PGID flood", vtss_state->l2.pgid_flood);
    pr("\n");

    pr("Port  Destination Group\n");
    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        pr("%-6u%u\n", port_no, vtss_state->l2.dgroup_port_conf[port_no].dgroup_no);
    }
    pr("\n");
}

#if defined(VTSS_FEATURE_AGGR_GLAG)
static void vtss_debug_print_glag(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    vtss_glag_no_t glag_no;
    vtss_port_no_t port_no;
    u8             member[VTSS_PORT_BF_SIZE];
    BOOL           header = 1;

    vtss_debug_print_port_header(vtss_state, pr, "GLAG  ", 0, 1);
    for (glag_no = VTSS_GLAG_NO_START; glag_no < VTSS_GLAG_NO_END; glag_no++) {
        pr("%-4u  ", glag_no);
        VTSS_PORT_BF_CLR(member);
        for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++)
            VTSS_PORT_BF_SET(member, port_no, vtss_state->l2.port_glag_no[port_no] == glag_no);
        vtss_debug_print_ports(vtss_state, pr, member, 1);
    }
    pr("\n");

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {        
        glag_no = vtss_state->l2.port_glag_no[port_no];
        if (glag_no == VTSS_GLAG_NO_NONE)
            continue;
        
        if (header) {
            pr("%-8s %-8s\n","Port","GLAG");
            header = 0;
        }
        pr("%-8u %-8u\n", port_no, glag_no);
    }
    if (!header)
        pr("\n");

#if defined(VTSS_FEATURE_VSTAX_V2)
    {
        vtss_glag_conf_t *conf;
        u32              gport;
        
        header = 1;
        for (glag_no = VTSS_GLAG_NO_START; glag_no < VTSS_GLAG_NO_END; glag_no++) {
            for (gport = VTSS_GLAG_PORT_START; gport < VTSS_GLAG_PORT_END; gport++) {
                conf = &vtss_state->l2.glag_conf[glag_no][gport];
                if (conf->entry.upspn == VTSS_PORT_NO_NONE) {
                    break;
                }  
                if (header) {
                    pr("%-8s %-8s %-8s\n","GLAG","UPSID","UPSPN");
                    header = 0;
                }
                pr("%-8u %-8d %-8u\n", glag_no, conf->entry.upsid, conf->entry.upspn);
            }
        }
        if (!header)
            pr("\n");
    }
#endif /* VTSS_FEATURE_VSTAX_V2 */
}
#endif /* VTSS_FEATURE_AGGR_GLAG */

static void vtss_debug_print_stp_state(vtss_state_t *vtss_state,
                                       const vtss_debug_printf_t pr, vtss_stp_state_t *state)
{
    vtss_port_no_t   port_no;
    vtss_stp_state_t s;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        s = state[port_no];
        pr("%s%s",
           port_no == 0 || (port_no & 7) ? "" : ".",
           s == VTSS_STP_STATE_FORWARDING ? "F" : s == VTSS_STP_STATE_LEARNING ? "L" : "D");
    }
    pr("\n");

}

static void vtss_debug_print_stp(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info)
{
    vtss_msti_t msti;

    vtss_debug_print_port_header(vtss_state, pr, "STP   ", 0, 1);
    pr("      ");
    vtss_debug_print_stp_state(vtss_state, pr, vtss_state->l2.stp_state);
    pr("\n");

    vtss_debug_print_port_header(vtss_state, pr, "MSTI  ", 0, 1);
    for (msti = VTSS_MSTI_START; msti < VTSS_MSTI_END; msti++) {
        pr("%-4u  ", msti);
        vtss_debug_print_stp_state(vtss_state, pr, vtss_state->l2.mstp_table[msti].state);
    }
    pr("\n");
}

static void vtss_debug_print_port_none(const vtss_debug_printf_t pr,
                                       const char *name,
                                       vtss_port_no_t port_no)
{
    pr("%s: ", name);
    if (port_no == VTSS_PORT_NO_NONE)
        pr("None");
    else
        pr("%u", port_no);
    pr("\n");
}

static void vtss_debug_print_mirror(vtss_state_t *vtss_state,
                                    const vtss_debug_printf_t pr,
                                    const vtss_debug_info_t   *const info)
{
    vtss_mirror_conf_t *conf = &vtss_state->l2.mirror_conf;

    vtss_debug_print_port_header(vtss_state, pr, "         ", 0, 1);
    pr("Ingress: ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.mirror_ingress, 1);
    pr("Egress : ");
    vtss_debug_print_port_members(vtss_state, pr, vtss_state->l2.mirror_egress, 1);
    pr("\n");
    
    vtss_debug_print_port_none(pr, "Mirror Port      ", conf->port_no);
    pr("Mirror Forwarding: %s\n", vtss_bool_txt(conf->fwd_enable));
#if defined(VTSS_ARCH_JAGUAR_1) || defined(VTSS_ARCH_JAGUAR_2)
    pr("Mirror Tag       : %s\n", 
       conf->tag == VTSS_MIRROR_TAG_NONE ? "None" :
       conf->tag == VTSS_MIRROR_TAG_C ? "C-Tag" :
       conf->tag == VTSS_MIRROR_TAG_S ? "S-Tag" :
       conf->tag == VTSS_MIRROR_TAG_S_CUSTOM ? "S-Custom-Tag" : "?");
    pr("Mirror VID       : %u\n", conf->vid);
    pr("Mirror PCP       : %u\n", conf->pcp);
    pr("Mirror DEI       : %u\n", conf->dei);
#endif /* VTSS_ARCH_JAGUAR_1/JAGUAR_2 */
    pr("\n");
}

static void vtss_debug_print_erps(vtss_state_t *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    vtss_erpi_t       erpi;
    vtss_erps_entry_t *entry;
    int               i;
    vtss_vid_t        vid;

    vtss_debug_print_port_header(vtss_state, pr, "ERPI  ", 0, 0);
    pr("  VLANs\n");
    for (erpi = VTSS_ERPI_START; erpi < VTSS_ERPI_END; erpi++) {
        entry = &vtss_state->l2.erps_table[erpi];
        pr("%-4u  ", erpi);
        vtss_debug_print_ports(vtss_state, pr, entry->port_member, 0);
        pr("  ");
        for (vid = VTSS_VID_NULL, i = 0; vid < VTSS_VIDS; vid++) {
            if (VTSS_BF_GET(entry->vlan_member, vid)) {
                pr("%s%u", i ? "," : "", vid);
                i++;
            }
        }
        pr("%s\n", i ? "" : "-");
    }
    pr("\n");
}

static void vtss_debug_print_eps(vtss_state_t *vtss_state,
                                 const vtss_debug_printf_t pr,
                                 const vtss_debug_info_t   *const info)
{
    vtss_port_no_t  port_no;
    vtss_port_eps_t *eps;
    char            buf[16];
    BOOL            header = 1;

    for (port_no = VTSS_PORT_NO_START; port_no < vtss_state->port_count; port_no++) {
        if (!info->port_list[port_no])
            continue;
        if (header) {
            header = 0;
            pr("Port  Protection  Type  Selector\n");
        }
        eps = &vtss_state->l2.port_protect[port_no];
        if (eps->conf.port_no == VTSS_PORT_NO_NONE)
            strcpy(buf, "None");
        else
            sprintf(buf, "%-4u", eps->conf.port_no);
        pr("%-4u  %-10s  %-4s  %s\n",
           port_no, buf, eps->conf.type == VTSS_EPS_PORT_1_PLUS_1 ? "1+1" : "1:1",
           eps->selector == VTSS_EPS_SELECTOR_WORKING ? "Working" : "Protection");
    }
    if (!header)
        pr("\n");
}

#if defined(VTSS_FEATURE_IPV4_MC_SIP) || defined(VTSS_FEATURE_IPV6_MC_SIP)
static void vtss_debug_print_ipv6_addr(const vtss_debug_printf_t pr, vtss_ipv6_t *ipv6)
{
    int i;

    for (i = 0; i < 16; i++) {
        pr("%02x%s", ipv6->addr[i], (i & 1) && i != 15 ? ":" : "");
    }
}

static void vtss_debug_print_ipmc(vtss_state_t              *vtss_state,
                                  const vtss_debug_printf_t pr,
                                  const vtss_debug_info_t   *const info)
{
    vtss_ipmc_info_t *ipmc = &vtss_state->l2.ipmc;
    vtss_ipmc_src_t  *src;
    vtss_ipmc_dst_t  *dst;
    char             buf[80];
    u32              ipv6, src_free_count = 0, dst_free_count = 0;
    BOOL             header;

    if (!vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_IPMC))
        return;

    for (src = ipmc->obj.src_free; src != NULL; src = src->next) {
        src_free_count++;
    }
    for (dst = ipmc->obj.dst_free; dst != NULL; dst = dst->next) {
        dst_free_count++;
    }

    vtss_debug_print_value(pr, "State size", sizeof(*ipmc));
    vtss_debug_print_value(pr, "Source count", ipmc->obj.src_count);
    vtss_debug_print_value(pr, "Source free", src_free_count);
    vtss_debug_print_value(pr, "Source maximum", ipmc->obj.src_max);
    vtss_debug_print_value(pr, "Destination count", ipmc->obj.dst_count);
    vtss_debug_print_value(pr, "Destination free", dst_free_count);
    vtss_debug_print_value(pr, "Destination maximum", ipmc->obj.dst_max);
    pr("\n");

    /* SIP Table */
    for (ipv6 = 0; ipv6 < 2; ipv6++) {
        sprintf(buf, ipv6 ? "  %-40s" : "  %-11s", "DIP");
        for (src = ipmc->obj.src_used[ipv6]; src != NULL; src = src->next) {
            pr("%-6s%-6s%-6s%s\n", "Type", "VID", "FID", "SIP");
            pr("%-6s%-6u%-6u", src->data.ssm ? "SSM" : "ASM", src->data.vid, src->data.fid);
            if (ipv6) 
                vtss_debug_print_ipv6_addr(pr, &src->data.sip.ipv6);
            else
                pr("0x%08x", src->data.sip.ipv4);
            pr("\n\n");
            header = 1;
            for (dst = src->dest; dst != NULL; dst = dst->next) {
                if (header) {
                    vtss_debug_print_port_header(vtss_state, pr, buf, 0, 1);
                    header = 0;
                }
                pr("  ");
                if (ipv6) 
                    vtss_debug_print_ipv6_addr(pr, &dst->data.dip.ipv6);
                else
                    pr("0x%08x", dst->data.dip.ipv4);
                pr(" ");
                vtss_debug_print_ports(vtss_state, pr, dst->data.member, 1);
            }
            pr("\n");
        }
    }
    
#if defined(VTSS_FEATURE_IS1)
    if (vtss_state->arch == VTSS_ARCH_L26) {
        vtss_vcap_debug_print_is1(vtss_state, pr, info);
    }
#endif /* VTSS_FEATURE_IS1 */    

#if defined(VTSS_FEATURE_IS2)
    if (vtss_state->arch == VTSS_ARCH_JR1) {
        vtss_vcap_debug_print_is2(vtss_state, pr, info);
    }
#endif /* VTSS_FEATURE_IS2 */

    vtss_debug_print_mac_table(vtss_state, pr, info);
}
#endif /* VTSS_FEATURE_IPV4_MC_SIP || VTSS_FEATURE_IPV6_MC_SIP */

#if defined(VTSS_FEATURE_VSTAX)
static void vtss_debug_print_stack(vtss_state_t *vtss_state,
                                   const vtss_debug_printf_t pr,
                                   const vtss_debug_info_t   *const info)
{
    vtss_vstax_conf_t      *conf;
    vtss_vstax_port_conf_t *port_conf;
    vtss_vstax_info_t      *vstax_info = &vtss_state->l2.vstax_info;
    vtss_vstax_chip_info_t *chip_info;
    vtss_chip_no_t         chip_no;
    int                    i;
    char                   buf[64];

    if (!vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_STACK))
       return;
    
    conf = &vtss_state->l2.vstax_conf;
    pr("UPSID_0: %u (%u)\n", conf->upsid_0, vstax_info->upsid[0]);
    pr("UPSID_1: %u (%u)\n", conf->upsid_1, vstax_info->upsid[1]);
    vtss_debug_print_port_none(pr, "Port_0 ", conf->port_0);
    vtss_debug_print_port_none(pr, "Port_1 ", conf->port_1);
#ifdef VTSS_FEATURE_VSTAX_V2
    pr("CMEF   : %s\n", vtss_bool_txt(!conf->cmef_disable));
#endif /* VTSS_FEATURE_VSTAX_V2 */
    pr("\n");
    
    for (chip_no = 0; chip_no < vtss_state->chip_count; chip_no++) {
        sprintf(buf, "Chip %u", chip_no);
        vtss_debug_print_header_underlined(pr, buf, 0);

        chip_info = &vstax_info->chip_info[chip_no];
        vtss_debug_print_reg_header(pr, "Port Masks"); 
        for (i = 0; i < 2; i++) {
            sprintf(buf, "Mask %s", i ? "B" : "A");
            vtss_debug_print_reg(pr, buf, i ? chip_info->mask_b : chip_info->mask_a);
        }
        pr("\nPort  TTL  Mirror\n"); 
        for (i = 0; i < 2; i++) {
            port_conf = &chip_info->port_conf[i];
            pr("%-6s%-5u%u\n", i ? "B" : "A", port_conf->ttl, port_conf->mirror);
        }
        pr("\n");
#if defined(VTSS_FEATURE_VSTAX_V2)
        {
            vtss_vstax_route_entry_t *rt;
            vtss_vstax_upsid_t       upsid;
            
            pr("UPSID  A  B\n");
            for (upsid = VTSS_VSTAX_UPSID_MIN; upsid <= VTSS_VSTAX_UPSID_MAX; upsid++) {
                rt = &chip_info->rt_table.table[upsid];
                pr("%-7u%-3u%u\n", upsid, rt->stack_port_a, rt->stack_port_b);
            }     
            pr("\n");
        }
#endif  /* VTSS_FEATURE_VSTAX_V2 */
    }
    pr("\n");
}
#endif /* VTSS_FEATURE_VSTAX */

void vtss_l2_debug_print(vtss_state_t *vtss_state,
                         const vtss_debug_printf_t pr,
                         const vtss_debug_info_t   *const info)
{
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_VLAN))
        vtss_debug_print_vlan(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_PVLAN))
        vtss_debug_print_pvlan(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_MAC_TABLE))
        vtss_debug_print_mac_table(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_AGGR))
        vtss_debug_print_aggr(vtss_state, pr, info);
#if defined(VTSS_FEATURE_AGGR_GLAG)
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_GLAG))
        vtss_debug_print_glag(vtss_state, pr, info);
#endif /* VTSS_FEATURE_AGGR_GLAG */
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_STP))
        vtss_debug_print_stp(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_MIRROR))
        vtss_debug_print_mirror(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_ERPS))
        vtss_debug_print_erps(vtss_state, pr, info);
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_EPS))
        vtss_debug_print_eps(vtss_state, pr, info);
#if defined(VTSS_FEATURE_VLAN_TRANSLATION)
    if (vtss_debug_group_enabled(pr, info, VTSS_DEBUG_GROUP_VXLAT))
        vtss_debug_print_vlan_trans(vtss_state, pr, info);
#endif /* VTSS_FEATURE_EVC */
#if defined(VTSS_FEATURE_IPV4_MC_SIP) || defined(VTSS_FEATURE_IPV6_MC_SIP)
    vtss_debug_print_ipmc(vtss_state, pr, info);
#endif /* VTSS_FEATURE_IPV4_MC_SIP || VTSS_FEATURE_IPV6_MC_SIP */
#if defined(VTSS_FEATURE_VSTAX)
    vtss_debug_print_stack(vtss_state, pr, info);
#endif /* VTSS_FEATURE_VSTAX */
}

#endif /* VTSS_FEATURE_LAYER2 */
