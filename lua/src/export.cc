#include "pf/plugin/lua/export.h"
#include "pf/plugin/lua/system.h"

pf_script::Interface *env_creator() {
  return new pf_plugin::lua::System();
}

extern "C"
PF_PLUGIN_API void 
pfopen_pf_plugin_lua(void *_params) {
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  if (params.size() <= 0) return;
  auto type = params[0].get<uint8_t>();
  register_env_creator_script(type, env_creator);
}
