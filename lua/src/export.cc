#include "pf/plugin/lua/file.h"
#include "pf/plugin/lua/logger.h"
#include "pf/plugin/lua/cache.h"
#include "pf/plugin/lua/export.h"
#include "pf/plugin/lua/system.h"

static const struct luaL_Reg filetable[] = {
  {"opentab", file_opentab},
  {"openini", file_openini},
  {NULL, NULL}
};

static const struct luaL_Reg logtable[] = {
  {"slow", slow_log},
  {"slow_error", slow_errorlog},
  {"slow_warning", slow_warninglog},
  {"slow_debug", slow_debuglog},
  {"slow_write", slow_writelog},
  {"fast", fast_log},
  {"fast_error", fast_errorlog},
  {"fast_warning", fast_warninglog},
  {"fast_debug", fast_debuglog},
  {"fast_write", fast_writelog},
  {NULL, NULL}
};

extern "C"
#if OS_UNIX
int luaopen_libpf_plugin_lua(lua_State* L) {
#elif OS_WIN
PF_PLUGIN_API int luaopen_pf_plugin_lua(lua_State* L) {
#endif
  luaL_register(L, "file", filetable); 
  luaL_register(L, "logger", logtable);
  dcache_register(L);
  return 1;
}

pf_script::Interface *env_creator() {
  return new pf_plugin::lua::System();
}

extern "C"
void pfopen_pf_plugin_lua(void *_params) {
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  if (params.size() <= 0) return;
  auto type = params[0].get<int8_t>();
  register_env_creator_script(type, env_creator);
}
