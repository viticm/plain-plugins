#include "pf/net/packet/interface.h"
#include "pf/net/packet/factorymanager.h"
#include "pf/net/packet/dynamic.h"
#include "pf/net/connection/basic.h"
#include "pf/net/connection/manager/basic.h"
#include "pf/net/connection/manager/listener.h"
#include "pf/net/connection/manager/connector.h"
#include "pf/script/config.h"
#include "pf/engine/kernel.h"
#include "pf/plugin/lua/net.h"

using namespace pf_net::packet;

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
  const char *value = lua_tostring(L, 2);
  packet->write_string(value);
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
  lua_pushnumber(L, value);
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
  lua_pushnumber(L, value);
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
  lua_pushnumber(L, value);
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
  lua_pushnumber(L, value);
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
  lua_pushnumber(L, value);
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
  lua_pushnumber(L, value);
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
                  "[script.lua] (net_read_uint64)"
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
  SCRIPT_LUA_CHECKARGC_LEAST(L, 2);
  auto count = lua_gettop(L);
  auto connid = static_cast<uint16_t>(lua_tonumber(L, 1));
  auto pointer = static_cast<int64_t>(lua_tonumber(L, 2));
  std::string name{""}; 
  if (count > 2) name = lua_tostring(L, 3);
  Dynamic *packet = packet_get(pointer);
  if (is_null(packet)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send)"
                  " pointer is null");
    return 0;
  }
  Basic *connection{nullptr};
  if ("default" == name || "" == name) {
    if (ENGINE_POINTER->get_net())
      connection = ENGINE_POINTER->get_net()->get(connid);
  } else {
    auto listener = ENGINE_POINTER->get_listener(name);
    if (!is_null(listener)) {
      connection = listener->get(connid);
    } else {
      connection = ENGINE_POINTER->get_connector(name);
    }
  }
  int32_t result{1};
  if (is_null(connection)) {
    SLOW_ERRORLOG(SCRIPT_MODULENAME,
                  "[script.lua] (net_send)"
                  " can't get the connection: %s|%d",
                  name.c_str(),
                  connid);
    result = 0;
  } else {
    connection->send(packet);
  }
  NET_PACKET_FACTORYMANAGER_POINTER->packet_remove(packet);
  return result;
}
