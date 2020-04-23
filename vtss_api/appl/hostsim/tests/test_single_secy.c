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
#include "vtss_macsec_test_base.h"

/*
 * Test scenario:
 *
 * This is the simplest possible test scenario where a MACsec port is
 * communicating with a single MACsec peer.
 *
 * This test case covers the configuration of the MACsec port, while we pretend
 * that a MACsec peer is connected to the MACsec port.
 *
 *    +-----------+     +-----------+
 *    |MACsec-Peer|<--->|MACsec-port|
 *    +-----------+     +-----------+
 *
 * - One CA is created comprising the MACsec port, and the MACsec peer
 * - 802.1X frames are associated with the uncontrolled port
 * - All other traffic is treated as MACsec traffic
 * - Traffic which is not recognized as MACsec traffic or 802.1X traffic is
 *   dropped.
 * */

// The real content of this example are to be found in base_single_secy.c

int main()
{
    // Create a PHY instance
    vtss_inst_t inst = instance_phy_new();

    vtss_macsec_port_t macsec_port;
    vtss_mac_t         port_macaddress;
    vtss_macsec_sci_t  peer_sci;

    vtss_single_secy_sample_system_create(inst,
                                          &macsec_port,
                                          &port_macaddress,
                                          &peer_sci);

    // TODO, inject traffic here, and verify traffic and counters

    vtss_single_secy_sample_system_destroy(inst,
                                           &macsec_port,
                                           &port_macaddress,
                                           &peer_sci);

    // Delete the PHY instance
    instance_phy_delete(inst);

    return 0;
}

