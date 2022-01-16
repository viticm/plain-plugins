#include "pf/db/config.h"
#include "pf/plugin/odbc/system.h"
#include "pf/engine/kernel.h"
#include "pf/plugin/odbc/export.h"

pf_db::Interface *env_creator() {
  return new pf_plugin::odbc::System();
}

extern "C"
PF_PLUGIN_API void
pfopen_pf_plugin_odbc(pf_engine::Kernel *engine, void *_params) {
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  auto type = (uint8_t)kDBEnvODBC;
  engine->register_db(type, env_creator);
}
