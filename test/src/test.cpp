#include <cassert>
#include <cstdlib>

#include <dlclass/load_plugin.h>
#include <example.h>

# if WIN32
#   ifdef MSVC
#     define EXAMPLE_LIB_PATH "./example.dll"
#   else
#     define EXAMPLE_LIB_PATH "./libexample.dll"
#   endif
# else
#   define EXAMPLE_LIB_PATH "./libexample.so"
# endif

static void example_plugin_test() {
  auto plugin = load_plugin<Example>(EXAMPLE_LIB_PATH);

  plugin->set_value(10);
  assert(plugin->get_value() == 10);

  plugin->set_value(27);
  assert(plugin->get_value() == 27);
}

static void error_test() try {
  auto plugin = load_plugin<Example>("404.dll");
  assert(plugin == nullptr);
} catch (std::runtime_error &e) {
  auto message = std::string(e.what());

#ifdef WIN32
  assert(message == "The specified module could not be found.\r\n");
#else
  assert(message == "404.dll: cannot open shared object file: No such file or directory");
#endif

}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  example_plugin_test();

  error_test();

  return EXIT_SUCCESS;
}
