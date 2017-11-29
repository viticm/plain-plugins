/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id basic.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2017/06/02 15:56
 * @uses The basic for protocol plugin.
*/
#ifndef PF_PLUGIN_PROTOCOL_BASIC_H_
#define PF_PLUGIN_PROTOCOL_BASIC_H_

#include "pf_plugin/protocol/config.h"

namespace pf_plugin {

namespace protocol {

/* Net packet factory init, you needn't it when you have your init for it. */
bool init();

/* Get the packet handlers. */
inline packet_handlers_t &get_packet_handlers() {
  static packet_handlers_t var;
  return var;
}

/* Register the normal packet handler. */
PF_PLUGIN_API void register_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler);

/* Register the logic packet handler. */
PF_PLUGIN_API void register_logic_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler);

/* Register the large packet handler. */
PF_PLUGIN_API void register_large_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler);


/* Get the packet handler. */
inline pf_net::packet::function_packet_execute get_packet_handler(
    uint16_t packet_id) {
  return get_packet_handlers()[packet_id];
}

}; //namespace protocol

}; //namespace pf_plugin

#endif //PF_PLUGIN_PROTOCOL_BASIC_H_
