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

#include "pf/plugin/protocol/config.h"
#include "pf/net/socket/api.h"
#include "pf/net/protocol/interface.h"

namespace pf_plugin {

namespace protocol {

//The net stream header struct need pack one char.
#pragma pack(push,1)
struct PF_PLUGIN_API TLBBHead {
  uint16_t mask = 0xAA55;
  uint16_t size = 0;
  uint8_t id = 0;
  uint16_t index = 0;
};
#pragma pack(pop)

struct PF_PLUGIN_API TLBBFoot {
  uint16_t mask = 0x55AA;
};

inline void tlbb_head_ntoh(TLBBHead &h) {
  h.mask = PF_NTOH(h.mask);
  h.size = PF_NTOH(h.size);
  h.index = PF_NTOH(h.index);
}

inline void tlbb_foot_ntoh(TLBBFoot &f) {
  f.mask = PF_NTOH(f.mask);
}

inline void tlbb_head_hton(TLBBHead &h) {
  h.mask = PF_HTON(h.mask);
  h.size = PF_HTON(h.size);
  h.index = PF_HTON(h.index);
}

inline void tlbb_foot_hton(TLBBFoot &f) {
  f.mask = PF_HTON(f.mask);
}

class PF_PLUGIN_API TLBB : public pf_net::protocol::Interface {

 public:
   TLBB();
   virtual ~TLBB();

 public:
   void set_dynamic(bool flag) { dynamic_ = flag; }

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

 private:
   bool dynamic_ = false;

};

} // namespace protocol

} // namespace pf_plugin

#endif // PF_PLUGIN_PROTOCOL_TLBB_H_
