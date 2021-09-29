/**
 * PLAIN FREAMEWORK PLUGINS ( https://github.com/viticm/plain-plugins )
 * $Id tlbb.h
 * @link https://github.com/viticm/plain-plugins for the canonical source repository
 * @copyright Copyright (c) 2021 viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm( viticm.ti@gmail.com )
 * @date 2021/09/28 17:19
 * @uses The protocol of old tlbb.
 *       protocol: head|body|foot
 *       head: mask(uint16_t 0xAA55)|size(uint16_t)|id(uint8_t)|index(uint16_t)
 *       body: binary[char *]
 *       foot: mask(uint16_t 0x55AA)
 *
 *       empty message length: 9
 */

#ifndef PF_PLUGIN_PROTOCOL_TLBB_H_
#define PF_PLUGIN_PROTOCOL_TLBB_H_

#include "pf/net/protocol/interface.h"

namespace pf_plugin {

namespace protocol {

struct PF_PLUGIN_API TLBBHead {
  uint16_t mask = 0xAA55;
  uint16_t size = 0;
  uint8_t id = 0;
  uint16_t index = 0;
};

struct PF_PLUGIN_API TLBBFoot {
  uint16_t mask = 0x55AA;
};

class PF_PLUGIN_API TLBB : public pf_net::protocol::Interface {

 public:
   TLBB();
   virtual ~TLBB();

 public:
   //处理网络包
   virtual bool command(pf_net::connection::Basic *connection,
                        uint16_t count);
   //处理压缩数据，主要是将压缩的数据解压
   virtual bool compress(pf_net::connection::Basic *connection, 
                         char *uncompress_buffer, 
                         char *compress_buffer);
   virtual bool send(pf_net::connection::Basic *connection,
                     pf_net::packet::Interface *packet);
   virtual size_t header_size() const { return sizeof(TLBBHead); };
   virtual pf_net::packet::Interface *read_packet(pf_net::connection::Basic *); 

};


} // namespace protocol

} // namespace pf_plugin

#endif // PF_PLUGIN_PROTOCOL_TLBB_H_
