#include <cassert>
#include <cstdlib>

#include <dlclass/load_plugin.h>
#include <dlclass/test/example.h>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  auto plugin = load_plugin<DLClass::Test::Example>("./libdlclass_example.so");

  plugin->set_value(10);
  assert(plugin->get_value() == 10);

  plugin->set_value(27);
  assert(plugin->get_value() == 27);

  return EXIT_SUCCESS;
}
