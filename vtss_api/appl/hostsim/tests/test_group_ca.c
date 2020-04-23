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
#include "vtss_macsec_api.h"
#include "vtss_macsec_test_base.h"
#include "vtss_macsec_emu_base.h"

// byte order is handled by the API
#define ETHERTYPE_IEEE_802_1_X 0x888E

/*
 * Test scenario:
 *
 * This is an example on how to configure the MACsec port for handling two
 * macsec peers connected to a single MACsec PHY.
 *
 * This test case covers the configuration of the MACsec port, while we pretend
 * that a two MACsec peer is connected to the MACsec port through a shared
 * media:
 *
 *    +------------+
 *    |MACsec-Peer1|<---+
 *    +------------+    |   +-----------+
 *                      *-->|MACsec-port|
 *    +------------+    |   +-----------+
 *    |MACsec-Peer2|<---+
 *    +------------+
 *
 * - One Group-CA is created comprising the MACsec port, and the two MACsec
 *   peers
 * - 802.1X frames are associated with the uncontrolled port
 * - All other traffic is treated as MACsec traffic
 * - Traffic which is not recognized as MACsec traffic or 802.1X traffic is
 *   dropped.
 * */

int main()
{
    // Create a PHY instance
    vtss_inst_t inst = instance_phy_new();

    // A set of constants used through the program
    const vtss_port_no_t           macsec_physical_port = 0;
    const vtss_macsec_port_t       macsec_port = {
        .port_no = macsec_physical_port,
    };
    const vtss_mac_t peer1_macaddress = { .addr = {0, 0, 0, 0, 0, 1}};
    const vtss_mac_t peer2_macaddress = { .addr = {0, 0, 0, 0, 0, 2}};
    const vtss_mac_t port_macaddress  = { .addr = {0, 0, 0, 0, 0, 3}};

    { // Initialize the physical PHY to process MACsec traffic
        // Initialize the MACsec block
        vtss_macsec_init_t init_data = { .enable = TRUE };
        VTSS_RC_TEST(vtss_macsec_init_set(inst, macsec_physical_port,
                                          &init_data));

        // Use the default rules to drop all non-matched traffic
        vtss_macsec_default_action_policy_t default_action_policy;
        memset(&default_action_policy, 0, sizeof default_action_policy);
        VTSS_RC_TEST(vtss_macsec_default_action_set(inst, macsec_physical_port,
                                                    &default_action_policy));
    }

    { // create a new MACsec SecY
        // Start by adding a SecY
        vtss_macsec_secy_conf_t macsec_port_conf = {
            .validate_frames        = VTSS_MACSEC_VALIDATE_FRAMES_STRICT,
            .replay_protect         = TRUE,
            .replay_window          = 0,
            .protect_frames         = TRUE,
            .always_include_sci     = FALSE,
            .use_es                 = TRUE,
            .use_scb                = FALSE,
            .current_cipher_suite   = VTSS_MACSEC_CIPHER_SUITE_GCM_AES_128,
            .confidentiality_offset = 0,
            .mac_addr               = port_macaddress
        };
        VTSS_RC_TEST(vtss_macsec_secy_conf_add(inst, macsec_port,
                                               &macsec_port_conf));

        // No encapsulation is used. Just match everything, but keep the
        // priority lower than the rule matching traffic for the uncontrolled
        // port. In other words, if it is not control traffic, then it should
        // be considered MACsec traffic.
        vtss_macsec_match_pattern_t pattern_ctrl = {
            .priority  = VTSS_MACSEC_MATCH_PRIORITY_LOW
        };

        // Associate the pattern with the controlled MACsec port
        VTSS_RC_TEST(vtss_macsec_pattern_set(inst, macsec_port,
                                             VTSS_MACSEC_DIRECTION_INGRESS,
                                             VTSS_MACSEC_MATCH_ACTION_CONTROLLED_PORT,
                                             &pattern_ctrl));
        VTSS_RC_TEST(vtss_macsec_pattern_set(inst, macsec_port,
                                             VTSS_MACSEC_DIRECTION_EGRESS,
                                             VTSS_MACSEC_MATCH_ACTION_CONTROLLED_PORT,
                                             &pattern_ctrl));

        // All traffic which is not vlan traffic, and have ether-type 0x888E
        // must be associated with the controlled port.
        vtss_macsec_match_pattern_t pattern_unctrl = {
            .match        = VTSS_MACSEC_MATCH_HAS_VLAN |
                            VTSS_MACSEC_MATCH_ETYPE,
            .has_vlan_tag = 0,
            .etype        = ETHERTYPE_IEEE_802_1_X,
            .priority     = VTSS_MACSEC_MATCH_PRIORITY_HIGH
        };

        // Associate the pattern with the uncontrolled MACsec port
        VTSS_RC_TEST(vtss_macsec_pattern_set(inst, macsec_port,
                                             VTSS_MACSEC_DIRECTION_INGRESS,
                                             VTSS_MACSEC_MATCH_ACTION_UNCONTROLLED_PORT,
                                             &pattern_unctrl));
        VTSS_RC_TEST(vtss_macsec_pattern_set(inst, macsec_port,
                                             VTSS_MACSEC_DIRECTION_EGRESS,
                                             VTSS_MACSEC_MATCH_ACTION_UNCONTROLLED_PORT,
                                             &pattern_unctrl));
    }

    { // Add secure TX channels and a secure association
        // Add a new TX secure channel
        VTSS_RC_TEST(vtss_macsec_tx_sc_set(inst, macsec_port));

        // create a zero-key for 128bit cipher suite
        vtss_macsec_sak_t sak_tx_sa_0 = { .len = 16 };

        // Update the hash key in the SAK before the key is installed in HW
        VTSS_RC_TEST(sak_update_hash_key(&sak_tx_sa_0));

        // install the key in HW on the egress side
        VTSS_RC_TEST(vtss_macsec_tx_sa_set(inst, macsec_port,
                                           0,    // associations number
                                           1024, // next_pn,
                                           TRUE, // confidentiality,
                                           &sak_tx_sa_0));
    }

    { // Add MACsec peer1
        // Add a new RX secure channel
        vtss_macsec_sci_t sci_rx = { .mac_addr = peer1_macaddress };
        VTSS_RC_TEST(vtss_macsec_rx_sc_add(inst, macsec_port, &sci_rx));

        // Add two RX SA's
        vtss_macsec_sak_t sak_rx_sa_0 = { .len = 16 };
        vtss_macsec_sak_t sak_rx_sa_1 = { .len = 16 };

        // Update the hash key in the SAK before the key is installed in HW
        VTSS_RC_TEST(sak_update_hash_key(&sak_rx_sa_0));
        VTSS_RC_TEST(sak_update_hash_key(&sak_rx_sa_1));

        VTSS_RC_TEST(vtss_macsec_rx_sa_set(inst, macsec_port,
                                           &sci_rx, // identify which SC the SA belongs to
                                           0,       // associations number
                                           0,       // lowest_pn,
                                           &sak_rx_sa_0));

        VTSS_RC_TEST(vtss_macsec_rx_sa_set(inst, macsec_port,
                                           &sci_rx, // identify which SC the SA belongs to
                                           1,       // associations number
                                           1023,    // lowest_pn,
                                           &sak_rx_sa_1));
    }

    { // Add MACsec peer2
        // Add a new RX secure channel
        vtss_macsec_sci_t sci_rx = { .mac_addr = peer2_macaddress };
        VTSS_RC_TEST(vtss_macsec_rx_sc_add(inst, macsec_port, &sci_rx));

        // Add two RX SA's
        vtss_macsec_sak_t sak_rx_sa_0 = { .len = 16 };
        vtss_macsec_sak_t sak_rx_sa_1 = { .len = 16 };

        // Update the hash key in the SAK before the key is installed in HW
        sak_update_hash_key(&sak_rx_sa_0);
        sak_update_hash_key(&sak_rx_sa_1);

        // Install keys in HW
        VTSS_RC_TEST(vtss_macsec_rx_sa_set(inst, macsec_port,
                                           &sci_rx, // identify which SC the SA belongs to
                                           0,       // associations number
                                           0,       // lowest_pn,
                                           &sak_rx_sa_0));

        VTSS_RC_TEST(vtss_macsec_rx_sa_set(inst, macsec_port,
                                           &sci_rx, // identify which SC the SA belongs to
                                           1,       // associations number
                                           1023,    // lowest_pn,
                                           &sak_rx_sa_1));
    }

    // TODO, inject traffic here, and verify traffic and counters

    { // clean up
        vtss_macsec_sci_t sci1_rx = { .mac_addr = peer1_macaddress };
        vtss_macsec_sci_t sci2_rx = { .mac_addr = peer2_macaddress };

        // Delete peers
        VTSS_RC_TEST(vtss_macsec_rx_sa_disable(inst, macsec_port, &sci1_rx, 1));
        VTSS_RC_TEST(vtss_macsec_rx_sa_disable(inst, macsec_port, &sci1_rx, 0));

        VTSS_RC_TEST(vtss_macsec_rx_sa_del(inst, macsec_port, &sci1_rx, 1));
        VTSS_RC_TEST(vtss_macsec_rx_sa_del(inst, macsec_port, &sci1_rx, 0));
        VTSS_RC_TEST(vtss_macsec_rx_sc_del(inst, macsec_port, &sci1_rx));

        VTSS_RC_TEST(vtss_macsec_rx_sa_disable(inst, macsec_port, &sci2_rx, 1));
        VTSS_RC_TEST(vtss_macsec_rx_sa_disable(inst, macsec_port, &sci2_rx, 0));

        VTSS_RC_TEST(vtss_macsec_rx_sa_del(inst, macsec_port, &sci2_rx, 1));
        VTSS_RC_TEST(vtss_macsec_rx_sa_del(inst, macsec_port, &sci2_rx, 0));
        VTSS_RC_TEST(vtss_macsec_rx_sc_del(inst, macsec_port, &sci2_rx));

        // Delete TX SA and SC
        VTSS_RC_TEST(vtss_macsec_tx_sa_del(inst, macsec_port, 0));
        VTSS_RC_TEST(vtss_macsec_tx_sc_del(inst, macsec_port));

        // Delete SecY
        VTSS_RC_TEST(vtss_macsec_secy_conf_del(inst, macsec_port));

        // Disable the MACsec block
        vtss_macsec_init_t deinit_data = { .enable = FALSE };
        VTSS_RC_TEST(vtss_macsec_init_set(inst, macsec_physical_port,
                                          &deinit_data));
    }

    // Delete the PHY instance
    instance_phy_delete(inst);

    return 0;
}


