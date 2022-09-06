#ifndef DLCLASS
#define DLCLASS

#include <dlclass/slfcn.h>

#include <memory>
#include <stdexcept>

namespace DLClass {
  template<typename T, typename ...A>
  static std::shared_ptr<T> load_plugin(const std::string &path, A... args);
}

template<typename T, typename ...A>
static std::shared_ptr<T> DLClass::load_plugin(const std::string &path, A... args) {
#ifndef WIN32
  // Clear any existing errors from previous calls.
  slerror();
#endif

  // Load the library, resolving symbols only as they are executed by the code.
  auto slhandle = slopen(path);

  // Throws an exception if a dlfcn function returns an unexpected NULL value.
  const auto handle_dlerror = [&slhandle](auto obj) {
    // Do nothing if the object returned by dlfcn is not null.
    if (obj != nullptr) return;

    // Free the reference to the plugin's shared library, so it doesn't stay open after throwing an error, especially
    // if the error is caught and handled elsewhere.
    if (slhandle) slclose(slhandle);

    // Clear the library's handle in case the exception is caught outside this function.
    slhandle = nullptr;

    // Turn dlerror() into a runtime error.
    throw std::runtime_error{slerror()};
  };

  // Handle any errors resulting from loading the shared object.
  handle_dlerror(slhandle);

  // Find the create function.
  const auto create = slsym<typename T::create_t *>(slhandle, "create");
  handle_dlerror(create);

  // Find the destroy function.
  const auto destroy = slsym<typename T::destroy_t *>(slhandle, "destroy");
  handle_dlerror(destroy);

  // Create an instance of the plugin.
  auto instance = create(args...);

  // Create a deleter that calls the plugin's destroy function and frees the reference to the shared object.
  const auto deleter = [slhandle, destroy](T *instance) {
    if (instance) destroy(instance);
    if (slhandle) slclose(slhandle);
  };

  // Create a smart pointer for the plugin's instance that will call its destructor when it's deleted.
  return std::shared_ptr<T>(instance, deleter);
}

#endif
