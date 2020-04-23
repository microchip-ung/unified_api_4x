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


#include "vtss_api.h"
#include "vtss_state.h"
#include "vtss_util.h"

#if defined (VTSS_FEATURE_MPLS)

#include "vtss_mpls_common.h"

// Index chain functions

void vtss_mpls_idxchain_init(vtss_state_t *vtss_state)
{
    vtss_mpls_idxchain_t i;
    for (i = 0; i < VTSS_MPLS_IDXCHAIN_ENTRY_CNT; i++) {
        vtss_state->mpls.idxchain[i].user = -1;             // Debug only
        vtss_state->mpls.idxchain[i].next = i+1;
    }
    vtss_state->mpls.idxchain[VTSS_MPLS_IDXCHAIN_ENTRY_CNT - 1].next = VTSS_MPLS_IDXCHAIN_UNDEFINED;     // Terminate chain
}

vtss_rc vtss_mpls_idxchain_insert_at_head(vtss_state_t *vtss_state,
                                          vtss_mpls_idxchain_t * const    chain,
                                          const vtss_mpls_idxchain_user_t user)
{
    vtss_mpls_idxchain_t new_idx = vtss_state->mpls.idxchain[0].next;

    if (chain == NULL  ||  new_idx == VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        return VTSS_RC_ERROR;
    }

    vtss_state->mpls.idxchain[0].next = vtss_state->mpls.idxchain[new_idx].next;
    vtss_state->mpls.idxchain[new_idx].next = *chain;
    vtss_state->mpls.idxchain[new_idx].user = user;
    *chain = new_idx;

    return VTSS_RC_OK;
}

vtss_rc vtss_mpls_idxchain_append_to_tail(vtss_state_t *vtss_state,
                                          vtss_mpls_idxchain_t * const    chain,
                                          const vtss_mpls_idxchain_user_t user)
{
    vtss_mpls_idxchain_t i;
    vtss_mpls_idxchain_t new_idx = vtss_state->mpls.idxchain[0].next;

    if (chain == NULL  ||  new_idx == VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        return VTSS_RC_ERROR;
    }

    vtss_state->mpls.idxchain[0].next = vtss_state->mpls.idxchain[new_idx].next;

    vtss_state->mpls.idxchain[new_idx].next = VTSS_MPLS_IDXCHAIN_UNDEFINED;
    vtss_state->mpls.idxchain[new_idx].user = user;

    if (*chain == VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        *chain = new_idx;
        return VTSS_RC_OK;
    }

    i = *chain;
    while (vtss_state->mpls.idxchain[i].next != VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        i = vtss_state->mpls.idxchain[i].next;
    }
    vtss_state->mpls.idxchain[i].next = new_idx;

    return VTSS_RC_OK;
}

vtss_rc vtss_mpls_idxchain_remove_first(vtss_state_t *vtss_state,
                                        vtss_mpls_idxchain_t * const    chain,
                                        const vtss_mpls_idxchain_user_t user)
{
    vtss_mpls_idxchain_t i, prev;

    if (chain == NULL) {
        return VTSS_RC_ERROR;
    }

    i = prev = *chain;
    while (i != VTSS_MPLS_IDXCHAIN_UNDEFINED  &&  vtss_state->mpls.idxchain[i].user != user) {
        prev = i;
        i = vtss_state->mpls.idxchain[i].next;
    }

    if (i == VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        // Not found
        return VTSS_RC_ERROR;
    }

    if (i == prev) {
        // Found at head; unlink and set new head
        *chain = vtss_state->mpls.idxchain[i].next;
    } else {
        // Found not at head; just unlink
        vtss_state->mpls.idxchain[prev].next = vtss_state->mpls.idxchain[i].next;
    }

    // Add to free list
    vtss_state->mpls.idxchain[i].user = -1;  // Debug only
    vtss_state->mpls.idxchain[i].next = vtss_state->mpls.idxchain[0].next;
    vtss_state->mpls.idxchain[0].next = i;

    return VTSS_RC_OK;
}

vtss_rc vtss_mpls_idxchain_remove_all(vtss_state_t *vtss_state,
                                      vtss_mpls_idxchain_t * const chain)
{
    if (chain == NULL) {
        return VTSS_RC_ERROR;
    }

    while (*chain != VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        vtss_mpls_idxchain_t nxt = vtss_state->mpls.idxchain[*chain].next;
        vtss_state->mpls.idxchain[*chain].user = -1;
        vtss_state->mpls.idxchain[*chain].next = vtss_state->mpls.idxchain[0].next;
        vtss_state->mpls.idxchain[0].next = *chain;
        *chain = nxt;
    }

    return VTSS_RC_OK;
}

vtss_rc vtss_mpls_idxchain_replace(vtss_state_t *vtss_state,
                                   const vtss_mpls_idxchain_t * const chain,
                                   const vtss_mpls_idxchain_user_t    user_old,
                                   const vtss_mpls_idxchain_user_t    user_new,
                                   const BOOL                         all)
{
    vtss_mpls_idxchain_t i;
    vtss_rc              rc = VTSS_RC_ERROR;

    if (chain == NULL) {
        return VTSS_RC_ERROR;
    }

    i = *chain;
    while (i != VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        if (vtss_state->mpls.idxchain[i].user == user_old) {
            vtss_state->mpls.idxchain[i].user = user_new;
            rc = VTSS_RC_OK;
            if (!all) {
                break;
            }
        }
        i = vtss_state->mpls.idxchain[i].next;
    }

    return rc;
}

BOOL vtss_mpls_idxchain_find(vtss_state_t *vtss_state,
                             const vtss_mpls_idxchain_t * const chain,
                             const vtss_mpls_idxchain_user_t    user)
{
    vtss_mpls_idxchain_t i;

    if (chain == NULL) {
        return FALSE;
    }

    i = *chain;
    while (i != VTSS_MPLS_IDXCHAIN_UNDEFINED  &&  vtss_state->mpls.idxchain[i].user != user) {
        i = vtss_state->mpls.idxchain[i].next;
    }

    return i != VTSS_MPLS_IDXCHAIN_UNDEFINED;
}

BOOL vtss_mpls_idxchain_get_next(vtss_state_t *vtss_state,
                                 vtss_mpls_idxchain_iter_t * const iter,
                                 vtss_mpls_idxchain_user_t * const user)
{
    if (iter == NULL  ||  user == NULL  ||  *iter == VTSS_MPLS_IDXCHAIN_UNDEFINED) {
        return FALSE;
    }

    *user = vtss_state->mpls.idxchain[*iter].user;
    *iter = vtss_state->mpls.idxchain[*iter].next;

    return TRUE;
}

BOOL vtss_mpls_idxchain_get_first(vtss_state_t *vtss_state,
                                  const vtss_mpls_idxchain_t * const chain,
                                  vtss_mpls_idxchain_iter_t  * const iter,
                                  vtss_mpls_idxchain_user_t  * const user)
{
    if (chain == NULL  ||  iter == NULL  ||  user == NULL) {
        return FALSE;
    }

    // Special case for *chain == 0, the free-chain
    *iter = (*chain == 0) ? vtss_state->mpls.idxchain[0].next : *chain;

    return vtss_mpls_idxchain_get_next(vtss_state, iter, user);
}

#endif  /* VTSS_FEATURE_MPLS */
