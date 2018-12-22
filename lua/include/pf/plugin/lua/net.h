/**
 * PLAIN FRAMEWORK ( https://github.com/viticm/plainframework )
 * $Id net.h
 * @link https://github.com/viticm/plainframework for the canonical source repository
 * @copyright Copyright (c) 2014- viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm<viticm.ti@gmail.com>
 * @date 2018/11/29 14:59
 * @uses The net plugin lua script functions.
*/
#ifndef PF_PLUGIN_LUA_NET_H_
#define PF_PLUGIN_LUA_NET_H_

#include "pf/plugin/lua/config.h"

#define PF_PLUGIN_LUA_NET_PACKET_STRINGMAX (1024 * 100)

int32_t net_write_int8(lua_State *L);
int32_t net_write_uint8(lua_State *L);
int32_t net_write_int16(lua_State *L);
int32_t net_write_uint16(lua_State *L);
int32_t net_write_int32(lua_State *L);
int32_t net_write_uint32(lua_State *L);
int32_t net_write_int64(lua_State *L);
int32_t net_write_uint64(lua_State *L);
int32_t net_write_string(lua_State *L);
int32_t net_write_float(lua_State *L);
int32_t net_write_double(lua_State *L);

int32_t net_read_int8(lua_State *L);
int32_t net_read_uint8(lua_State *L);
int32_t net_read_int16(lua_State *L);
int32_t net_read_uint16(lua_State *L);
int32_t net_read_int32(lua_State *L);
int32_t net_read_uint32(lua_State *L);
int32_t net_read_int64(lua_State *L);
int32_t net_read_uint64(lua_State *L);
int32_t net_read_string(lua_State *L);
int32_t net_read_float(lua_State *L);
int32_t net_read_double(lua_State *L);
int32_t net_packet_alloc(lua_State *L);

//Use this interface will check your listener and connector name must be different.
//And the packet pointer will remove after this function.
int32_t net_send(lua_State *L);

//Connect by user(return -1 mean not connected).
int32_t net_connect(lua_State *L);

#endif //PF_PLUGIN_LUA_NET_H_
