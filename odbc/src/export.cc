#include "pf/db/config.h"
#include "pf/plugin/odbc/system.h"
#include "pf/plugin/odbc/export.h"

pf_db::Interface *env_creator() {
  return new pf_plugin::odbc::System();
}

extern "C"
void pfopen_pf_plugin_odbc(void *_params) {
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  if (params.size() <= 0) return;
  auto type = params[0].get<int8_t>();
  register_env_creator_db(type, env_creator);
}
