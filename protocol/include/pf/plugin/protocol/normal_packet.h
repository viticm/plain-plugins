/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id normal_packet.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2017/06/03 15:08
 * @uses your description
*/
#ifndef PF_PLUGIN_PROTOCOL_NORMAL_PACKET_H_
#define PF_PLUGIN_PROTOCOL_NORMAL_PACKET_H_

#include "pf/plugin/protocol/basic.h"

namespace pf_plugin {

namespace protocol {


class NormalPacket : public pf_net::packet::Interface {

 public:
   NormalPacket(uint16_t id) : id_{id}, str_{""} {}
   virtual ~NormalPacket() {}

 public:
   virtual bool read(pf_net::stream::Input &istream) {
     istream >> str_;
     return true;
   }
   virtual bool write(pf_net::stream::Output &ostream) {
     ostream << str_;
     return true;
   }
   virtual uint32_t execute(pf_net::connection::Basic *connection) {
     auto func = get_packet_handler(id_); 
     if (is_null(func)) return kPacketExecuteStatusError;
     return func(connection, this);
   }
   uint16_t get_id() const { return id_; }
   virtual uint32_t size() const { 
     return sizeof(uint32_t) + static_cast<uint32_t>(str_.size()); 
   }
   void set_str(const std::string &str) {
     str_ = str;
   }
   const char *get_str() {
     return str_.c_str();
   }

 private:
   uint16_t id_;
   std::string str_;

};

class NormalPacketFactory : public pf_net::packet::Factory {

 public:
   NormalPacketFactory(uint16_t id) : id_{id} {}
   virtual ~NormalPacketFactory() {}

 public:
   virtual pf_net::packet::Interface *packet_create() {
     return new NormalPacket(id_);
   }
   uint16_t packet_id() const {
     return id_;
   }
   virtual uint32_t packet_max_size() const {
     return PROTOCOL_PACKET_SIZE_MAX;
   };

 private:
   uint16_t id_;

};

}; //namespace protocol

}; //namespace pf_plugin

#endif //PF_PLUGIN_PROTOCOL_NORMAL_PACKET_H_
