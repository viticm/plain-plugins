/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id logic_packet.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2017/06/03 15:38
 * @uses your description
*/
#ifndef PF_PLUGIN_PROTOCOL_LOGIC_PACKET_H_
#define PF_PLUGIN_PROTOCOL_LOGIC_PACKET_H_

#include "pf/plugin/protocol/basic.h"

namespace pf_plugin {

namespace protocol {


class LogicPacket : public pf_net::packet::Interface {

 public:
   LogicPacket(uint16_t id) : id_{id}, logic_id_{0}, option_{0}, str_{""} {}
   virtual ~LogicPacket() {}

 public:
   virtual bool read(pf_net::stream::Input &istream) {
     istream >> logic_id_ >> option_ >> str_;
     return true;
   }
   virtual bool write(pf_net::stream::Output &ostream) {
     ostream << logic_id_ << option_ << str_;
     return true;
   }
   virtual uint32_t execute(pf_net::connection::Basic *connection) {
     auto func = get_packet_handler(id_); 
     if (is_null(func)) return kPacketExecuteStatusError;
     return func(connection, this);
   }
   uint16_t get_id() const { return id_; }
   void set_logic_id(uint8_t logic_id) {
     logic_id_ = logic_id;
   }
   uint8_t get_logic_id() const {
     return logic_id_;
   }
   void set_option(uint8_t option) {
     option_ = option;
   }
   uint8_t get_option() const {
     return option_;
   }
   virtual uint32_t size() const { 
     return sizeof(uint8_t) + sizeof(uint8_t) +
            sizeof(uint32_t) + 
            static_cast<uint32_t>(str_.size()); 
   }
   void set_str(const std::string &str) {
     str_ = str;
   }
   const char *get_str() {
     return str_.c_str();
   }

 private:
   uint16_t id_;
   uint8_t logic_id_;
   uint8_t option_;
   std::string str_;

};

class LogicPacketFactory : public pf_net::packet::Factory {

 public:
   LogicPacketFactory(uint16_t id) : id_{id} {}
   virtual ~LogicPacketFactory() {}

 public:
   virtual pf_net::packet::Interface *packet_create() {
     return new LogicPacket(id_);
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

#endif //PF_PLUGIN_PROTOCOL_LOGIC_PACKET_H_
