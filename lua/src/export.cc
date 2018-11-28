#include "pf/plugin/lua/export.h"
#include "pf/plugin/lua/system.h"


/**
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
**/

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
