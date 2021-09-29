#include "pf/net/packet/interface.h"
#include "pf/net/packet/factorymanager.h"
#include "pf/net/packet/dynamic.h"
#include "pf/net/packet/register_connection_name.h"
#include "pf/net/packet/routing_request.h"
#include "pf/net/connection/basic.h"
#include "pf/net/connection/manager/basic.h"
#include "pf/net/connection/manager/listener.h"
#include "pf/net/connection/manager/connector.h"
#include "pf/script/config.h"
#include "pf/engine/kernel.h"
#include "pf/plugin/lua/net.h"

using namespace pf_net::packet;

#ifndef lua_tostringex
#define lua_tostringex(L,n) (lua_isstring((L),(n)) ? lua_tostring((L),(n)) : "")
#endif

//compatibility different version.
#ifndef lua_rawlen
#if LUA_VERSION_NUM == 502
#define lua_rawlen(L,i) lua_len(L, (i))
#elif LUA_VERSION_NUM <= 501
#define lua_rawlen(L,i) lua_strlen(L, (i))
#endif
#endif

#define packet_get(pointer) \
  (dynamic_cast<Dynamic *>(NET_PACKET_FACTORYMANAGER_POINTER->packet_get(pointer)))

int32_t net_write_int8(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_int8)"
                  " pointer is null");
    return 0;
  }
  int8_t value = static_cast<int8_t>(lua_tointeger(L , 2));
  packet->write_int8(value);
  return 0;
}

int32_t net_write_uint8(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_uint8)"
                  " pointer is null");
    return 0;
  }
  uint8_t value = static_cast<uint8_t>(lua_tointeger(L , 2));
  packet->write_uint8(value);
  return 0;
}

int32_t net_write_int16(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_int16)"
                  " pointer is null");
    return 0;
  }
  int16_t value = static_cast<int16_t>(lua_tointeger(L , 2));
  packet->write_uint16(value);
  return 0;
}

int32_t net_write_uint16(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_uint16)"
                  " pointer is null");
    return 0;
  }
  uint16_t value = static_cast<uint16_t>(lua_tointeger(L , 2));
  packet->write_uint16(value);
  return 0;
}

int32_t net_write_int32(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_int32)"
                  " pointer is null");
    return 0;
  }
  int32_t value = static_cast<int32_t>(lua_tointeger(L , 2));
  packet->write_int32(value);
  return 0;
}

int32_t net_write_uint32(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_uint32)"
                  " pointer is null");
    return 0;
  }
  uint32_t value = static_cast<uint32_t>(lua_tointeger(L , 2));
  packet->write_uint32(value);
  return 0;
}

int32_t net_write_int64(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_int64)"
                  " pointer is null");
    return 0;
  }
  int64_t value = static_cast<int64_t>(lua_tonumber(L , 2));
  packet->write_int64(value);
  return 0;
}

int32_t net_write_uint64(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_uint64)"
                  " pointer is null");
    return 0;
  }
  uint64_t value = static_cast<uint64_t>(lua_tonumber(L , 2));
  packet->write_uint64(value);
  return 0;
}

int32_t net_write_string(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_string)"
                  " pointer is null");
    return 0;
  }
  std::string value = lua_tostringex(L, 2);
  packet->write_string(value.c_str());
  return 0;
}

int32_t net_write_float(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_float)"
                  " pointer is null");
    return 0;
  }
  float value = static_cast<float>(lua_tonumber(L, 2));
  packet->write_float(value);
  return 0;
}

int32_t net_write_double(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_dobule)"
                  " pointer is null");
    return 0;
  }
  double value = lua_tonumber(L, 2);
  packet->write_double(value);
  return 0;
}

int32_t net_write_bytes(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 2);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_write_bytes)"
                  " pointer is null");
    return 0;
  }
  auto value = (const unsigned char *)lua_tostring(L, 2);
  size_t size = lua_rawlen(L, 2);
  packet->write_bytes(value, size);
  return 0;
}

int32_t net_read_int8(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_int8)"
                  " pointer is null");
    return 0;
  }
  int8_t value = packet->read_int8();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_uint8(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_uint8)"
                  " pointer is null");
    return 0;
  }
  uint8_t value = packet->read_uint8();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_int16(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_int16)"
                  " pointer is null");
    return 0;
  }
  int16_t value = packet->read_int16();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_uint16(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_uint16)"
                  " pointer is null");
    return 0;
  }
  uint16_t value = packet->read_uint16();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_int32(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_int32)"
                  " pointer is null");
    return 0;
  }
  int32_t value = packet->read_int32();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_uint32(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_uint32)"
                  " pointer is null");
    return 0;
  }
  uint32_t value = packet->read_uint32();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_read_int64(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_int64)"
                  " pointer is null");
    return 0;
  }
  int64_t value = packet->read_int64();
  lua_pushnumber(L, static_cast<lua_Number>(value));
  return 1;
}

int32_t net_read_uint64(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_uint64)"
                  " pointer is null");
    return 0;
  }
  uint64_t value = packet->read_uint64();
  lua_pushnumber(L, static_cast<lua_Number>(value));
  return 1;
}

int32_t net_read_string(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_string)"
                  " pointer is null");
    return 0;
  }
  char value[PF_PLUGIN_LUA_NET_PACKET_STRINGMAX]{0};
  packet->read_string(value, sizeof(value) - 1);
  lua_pushstring(L, value);
  return 1;
}

int32_t net_read_float(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_float)"
                  " pointer is null");
    return 0;
  }
  float value = packet->read_float();
  lua_pushnumber(L, value);
  return 1;
}

int32_t net_read_double(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_double)"
                  " pointer is null");
    return 0;
  }
  double value = packet->read_double();
  lua_pushnumber(L, value);
  return 1;
}

int32_t net_read_bytes(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_string)"
                  " pointer is null");
    return 0;
  }
  unsigned char value[PF_PLUGIN_LUA_NET_PACKET_STRINGMAX]{0};
  auto size = packet->read_bytes(value, sizeof(value) - 1);
  lua_pushlstring(L, (const char *)value, size);
  return 1;
}


int32_t net_read_id(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  int64_t pointer = static_cast<int64_t>(lua_tonumber(L, 1));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_read_id)"
                  " pointer is null");
    return 0;
  }
  auto value = packet->get_id();
  lua_pushinteger(L, value);
  return 1;
}

int32_t net_packet_alloc(lua_State *L) {
  SCRIPT_LUA_CHECKARGC(L, 1);
  uint16_t packetid = static_cast<uint16_t>(lua_tointeger(L, 1));
  if (!NET_PACKET_FACTORYMANAGER_POINTER->is_valid_dynamic_packet_id(packetid)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_packet_alloc)"
                  " packet id is invalid: %d, not a dynamic id",
                  packetid);
    return 0;
  }
  Interface *packet = NET_PACKET_FACTORYMANAGER_POINTER->packet_create(packetid);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_packet_alloc)"
                  " create packet failed");
    return 0;
  }
  int64_t pointer = POINTER_TOINT64(packet);
  lua_pushnumber(L, static_cast<lua_Number>(pointer));
  return 1;
}

int32_t net_send(lua_State *L) {
  using namespace pf_net::connection;
  SCRIPT_LUA_CHECKARGC_LEAST(L, 3);
  std::string manager_name = lua_tostringex(L, 1);
  manager::Basic *manager{nullptr};
  if ("listener" == manager_name) {
    if (lua_isnil(L, 4)) {
     SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send)"
                  " can't get the listener name");
    } else {
      std::string listener_name = lua_tostringex(L, 4);
      manager = ENGINE_POINTER->get_listener(listener_name);
    }
  } else if ("connector" == manager_name) {
    manager = ENGINE_POINTER->get_connector();
  } else {
    if (ENGINE_POINTER->get_net()) {
      manager = ENGINE_POINTER->get_net();
    }
  }
  Basic *connection{nullptr};
  if (!is_null(manager)) {
    if (lua_isnumber(L, 2)) {
      connection = manager->get(static_cast<uint16_t>(lua_tonumber(L, 2)));
    } else {
      std::cout << "get connection: " << lua_tostringex(L, 2) << std::endl;;
      connection = manager->get(lua_tostringex(L, 2));
    }
  }
  auto pointer = static_cast<int64_t>(lua_tonumber(L, 3));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send) packet"
                  " pointer is null");
    return 0;
  }
  int32_t result{1};
  if (is_null(connection) || connection->is_disconnect()) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send)"
                  " can't get the connection or disconnect: %s|%s|%s",
                  manager_name.c_str(),
                  lua_tostringex(L, 2),
                  lua_tostringex(L, 4));
    result = 0;
  } else {
    lua_pushboolean(L, connection->send(packet));
  }
  NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
  return result;
}

int32_t net_conn_name(lua_State *L) {
  using namespace pf_net::connection;
  SCRIPT_LUA_CHECKARGC_LEAST(L, 2);
  std::string manager_name = lua_tostringex(L, 1);
  auto connid = static_cast<uint16_t>(lua_tonumber(L, 2));
  std::string name{""}; 
  Basic *connection{nullptr};
  int32_t result{0};
  manager::Basic *manager{nullptr};
  if ("listener" == manager_name) {
    if (lua_isnil(L, 4)) {
     SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send)"
                  " can't get the listener name");
    } else {
      std::string listener_name = lua_tostringex(L, 4);
      manager = ENGINE_POINTER->get_listener(listener_name);
    }
  } else if ("connector" == manager_name) {
    manager = ENGINE_POINTER->get_connector();
  } else {
    if (ENGINE_POINTER->get_net()) {
      manager = ENGINE_POINTER->get_net();
    }
  }

  if (!is_null(manager)) connection = manager->get(connid);
  if (!lua_isnil(L, 3) && !is_null(connection)) { //Need set name.
    name = lua_tostringex(L, 3);
    //Register name.
    connection->set_name(name);
    pf_net::packet::RegisterConnectionName regname;
    regname.set_name(name);
    connection->send(&regname);
    manager->set_connection_name(connection->get_id(), name);
  }
  if (!is_null(connection)) name = connection->name();
  if (name != "") {
    result = 1;
    lua_pushstring(L, name.c_str());
  }
  return result;
}

int32_t net_connect(lua_State *L) {
  SCRIPT_LUA_CHECKARGC_LEAST(L, 1);
  auto count = lua_gettop(L);
  pf_net::connection::Basic *connection{nullptr};
  std::string name = lua_tostringex(L, 1);
  if (1 == count) {
    connection = ENGINE_POINTER->connect(name);
  } else if (count >= 3) {
    std::string ip = lua_tostringex(L, 2);
    auto port = static_cast<uint16_t>(lua_tointeger(L, 3));
    auto userclient = static_cast<bool>(lua_toboolean(L, 4));
    std::string encrypt_str = lua_tostringex(L, 5);
    if (userclient) {
      connection = ENGINE_POINTER->connect(name, ip, port, encrypt_str);
    } else {
      connection = ENGINE_POINTER->default_connect(name, ip, port, encrypt_str);
    }
  }
  if (is_null(connection)) {
    lua_pushinteger(L, -1);
  } else {
    lua_pushinteger(L, connection->get_id());
  }
  return 1;
}

int32_t net_disconnect(lua_State *L) {
  using namespace pf_net::connection;
  SCRIPT_LUA_CHECKARGC_LEAST(L, 2);
  auto count = lua_gettop(L);
  std::string manager_name = lua_tostringex(L, 1);
  auto connid = static_cast<uint16_t>(lua_tonumber(L, 2));
  manager::Basic *manager{nullptr};
  pf_net::connection::Basic *connection{nullptr};
  if ("listener" == manager_name) {
    if (count <= 2) {
     SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_disconnect)"
                  " can't get the listener name");
    } else {
      std::string listener_name = lua_tostringex(L, 3);
      manager = ENGINE_POINTER->get_listener(listener_name);
    }
  } else if ("connector" == manager_name) {
    manager = ENGINE_POINTER->get_connector();
  } else {
    if (ENGINE_POINTER->get_net()) {
      manager = ENGINE_POINTER->get_net();
    }
  }
  if (!is_null(manager)) connection = manager->get(connid);
  bool result{false};
  if (!is_null(connection) && !connection->is_disconnect()) {
    connection->set_disconnect();
    result = true;
  }
  lua_pushboolean(L, result);
  return 1;
}

int32_t net_routing_request(lua_State *L) {
  using namespace pf_net::connection;
  SCRIPT_LUA_CHECKARGC(L, 6);
  std::string manager_name = lua_tostringex(L, 1);
  auto connid = static_cast<uint16_t>(lua_tonumber(L, 2));
  manager::Basic *manager{nullptr};
  pf_net::connection::Basic *connection{nullptr};
  if ("listener" == manager_name) {
    std::string listener_name = lua_tostringex(L, 3);
    manager = ENGINE_POINTER->get_listener(listener_name);
  } else if ("connector" == manager_name) {
    manager = ENGINE_POINTER->get_connector();
  } else {
    if (ENGINE_POINTER->get_net()) {
      manager = ENGINE_POINTER->get_net();
    }
  }
  if (!is_null(manager)) connection = manager->get(connid);
  bool result{false};
  if (!is_null(connection)) {
    std::string destination = lua_tostringex(L, 4);
    std::string aim_name = lua_tostringex(L, 5);
    uint16_t aim_id = static_cast<uint16_t>(lua_tonumber(L, 6));
    pf_net::packet::RoutingRequest routing_request;
    routing_request.set_destination(destination);
    routing_request.set_aim_name(aim_name);
    routing_request.set_aim_id(aim_id);
    result = connection->send(&routing_request);
  }
  lua_pushboolean(L, result);
  return 1;
}

int32_t net_routing(lua_State *L) {
  using namespace pf_net::connection;
  SCRIPT_LUA_CHECKARGC(L, 6);
  std::string manager_name = lua_tostring(L, 1);
  manager::Basic *manager{nullptr};
  pf_net::connection::Basic *connection{nullptr};
  if ("listener" == manager_name) {
    std::string listener_name = lua_tostringex(L, 3);
    manager = ENGINE_POINTER->get_listener(listener_name);
  } else if ("connector" == manager_name) {
    manager = ENGINE_POINTER->get_connector();
  } else {
    if (ENGINE_POINTER->get_net()) {
      manager = ENGINE_POINTER->get_net();
    }
  }
  if (!is_null(manager)) {
    if (lua_isnumber(L, 2)) {
      connection = manager->get(static_cast<uint16_t>(lua_tonumber(L, 2)));
    } else {
      connection = manager->get(lua_tostringex(L, 2));
    }
  }

  auto pointer = static_cast<int64_t>(lua_tonumber(L, 5));
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_routing) packet"
                  " pointer is null");
    return 0;
  }
  bool result{false};
  if (!is_null(connection)) {
    std::string aim_name = lua_tostringex(L, 4);
    std::string destination = lua_tostringex(L, 6);
    result = connection->routing(aim_name, packet, destination);
  }
  lua_pushboolean(L, result);
  NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
  return 1;
}
