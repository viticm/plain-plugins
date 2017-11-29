#include "pf/plugin/protocol/large_packet.h"
#include "pf/plugin/protocol/logic_packet.h"
#include "pf/plugin/protocol/basic.h"

namespace pf_plugin {

namespace protocol {

void register_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler) {
  if (get_packet_handlers()[packet_id]) return;
  get_packet_handlers()[packet_id] = handler;
  NET_PACKET_FACTORYMANAGER_POINTER
    ->add_factory(new NormalPacketFactory(packet_id));
}

void register_logic_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler) {
  if (get_packet_handlers()[packet_id]) return;
  get_packet_handlers()[packet_id] = handler;
  NET_PACKET_FACTORYMANAGER_POINTER
    ->add_factory(new LogicPacketFactory(packet_id));
}

void register_large_packet_handler(
    uint16_t packet_id, 
    pf_net::packet::function_packet_execute handler) {
  if (get_packet_handlers()[packet_id]) return;
  get_packet_handlers()[packet_id] = handler;
  NET_PACKET_FACTORYMANAGER_POINTER
    ->add_factory(new LargePacketFactory(packet_id));
}

const uint16_t kDynamicPacketIdBegin{0xBFFF};

static bool __stdcall is_valid_packetid(uint16_t id) {
  return 0 < id && kDynamicPacketIdBegin > id;
}

static bool __stdcall is_valid_dynamic_packet_id(uint16_t id) {
  return id >= kDynamicPacketIdBegin;
}

static bool __stdcall registerfactories() {
  return true;
}

bool init() {
  if (is_null(NET_PACKET_FACTORYMANAGER_POINTER)) {
    std::unique_ptr<pf_net::packet::FactoryManager> 
      temp{new pf_net::packet::FactoryManager()};
    g_packetfactory_manager = std::move(temp);
    if (is_null(NET_PACKET_FACTORYMANAGER_POINTER)) return false;
  }
  NET_PACKET_FACTORYMANAGER_POINTER->set_size(0xFFFF);
  NET_PACKET_FACTORYMANAGER_POINTER
    ->set_function_register_factories(registerfactories);
  NET_PACKET_FACTORYMANAGER_POINTER
    ->set_function_is_valid_packet_id(is_valid_packetid);
  NET_PACKET_FACTORYMANAGER_POINTER
    ->set_function_is_valid_dynamic_packet_id(is_valid_dynamic_packet_id);
  return NET_PACKET_FACTORYMANAGER_POINTER->init();
}

} //namespace protocol

} //namespace pf_plugin
