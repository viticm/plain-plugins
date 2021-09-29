#include "pf/net/protocol/helpers.h"
#include "pf/plugin/protocol/export.h"
#include "pf/plugin/protocol/tlbb.h"


extern "C"
void pfopen_pf_plugin_protocol(void *_params) {
  using namespace pf_plugin::protocol;
  auto params = *cast(const pf_basic::type::variable_array_t *, _params);
  static TLBB tlbb;
  pf_net::protocol::set("tlbb", &tlbb);
}
