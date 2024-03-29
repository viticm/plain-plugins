#include "pf/engine/kernel.h"
#include "pf/net/protocol/helpers.h"
#include "pf/plugin/protocol/tlbb.h"
#include "pf/plugin/protocol/export.h"

extern "C"
PF_PLUGIN_API void
pfopen_pf_plugin_protocol(pf_engine::Kernel *engine, void *_params) {
  using namespace pf_plugin::protocol;
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  auto dynamic = params.size() > 0 ? params[0].get<bool>() : false;
  static TLBB tlbb;
  tlbb.set_dynamic(dynamic);
  engine->register_protocol("tlbb", &tlbb);
}
