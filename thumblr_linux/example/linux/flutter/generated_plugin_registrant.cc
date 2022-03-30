//
//  Generated file. Do not edit.
//

// clang-format off

#include "generated_plugin_registrant.h"

#include <thumblr_linux/thumblr_linux_plugin.h>

void fl_register_plugins(FlPluginRegistry* registry) {
  g_autoptr(FlPluginRegistrar) thumblr_linux_registrar =
      fl_plugin_registry_get_registrar_for_plugin(registry, "ThumblrLinuxPlugin");
  thumblr_linux_plugin_register_with_registrar(thumblr_linux_registrar);
}
