/**
 * PLAIN FREAMEWORK PLUGINS ( https://github.com/viticm/plain-plugins )
 * $Id tool.h
 * @link https://github.com/viticm/plain-plugins for the canonical source repository
 * @copyright Copyright (c) 2021 viticm( viticm.ti@gmail.com )
 * @license
 * @user viticm( viticm.ti@gmail.com )
 * @date 2021/12/08 15:50
 * @uses The some useful function tool.
 */

#ifndef PF_PLUGIN_LUA_TOOL_H_
#define PF_PLUGIN_LUA_TOOL_H_

#include "pf/plugin/lua/config.h"

PF_PLUGIN_API int32_t md5(lua_State *L);
PF_PLUGIN_API int32_t split2table(lua_State *L);
PF_PLUGIN_API int32_t split2stack(lua_State *L);
PF_PLUGIN_API int32_t traceback(lua_State *L);
PF_PLUGIN_API int32_t base64_encode(lua_State *L);
PF_PLUGIN_API int32_t base64_decode(lua_State *L);

#endif // PF_PLUGIN_LUA_TOOL_H_
