/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id config.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2017/06/02 11:00
 * @uses If you want use this library, you need known this:
 *       use protocol::init will have this subsection packet id number
 *         0x0001 - 0xBFFE is the common packet id.
 *         0xBFFF - oxFFFF is the dynamic packet id.
*/
#ifndef PF_PLUGIN_PROTOCOL_CONFIG_H_
#define PF_PLUGIN_PROTOCOL_CONFIG_H_

#include "pf/basic/config.h"
#include "pf/basic/endian.h"
#include "pf/net/packet/factorymanager.h"

namespace pf_plugin {

namespace protocol {

using packet_handlers_t = 
  std::map<uint16_t, pf_net::packet::function_packet_execute>;

}; //namespace protocol

}; //namespace pf_plugin

#ifndef PROTOCOL_PACKET_SIZE_MAX
#define PROTOCOL_PACKET_SIZE_MAX (1024 * 100)
#endif

#ifndef PROTOCOL_LARGEPACKET_SIZE_MAX
#define PROTOCOL_LARGEPACKET_SIZE_MAX (10 * 1024 * 1024)
#endif

#endif //PF_PLUGIN_PROTOCOL_CONFIG_H_
