#pragma once

#include <dlfcn.h>
#include <memory>
#include <stdexcept>

template<typename T, typename ...A>
static std::shared_ptr<T> load_plugin(const std::string &path, A... args) {
  // Clear any existing errors from previous calls.
  dlerror();

  // Load the library, resolving symbols only as their executed by the code.
  auto dlhandle = dlopen(path.c_str(), RTLD_LAZY);

  // Throws an exception if a dlfcn function returns an unexpected NULL value.
  const auto handle_dlerror = [&dlhandle](auto obj) {
    // Do nothing if the object returned by dlfcn is not null.
    if (obj != nullptr) return;

    // Free the reference to the plugin's shared library, so it doesn't stay open after throwing an error, especially
    // if the error is caught and handled elsewhere.
    if (dlhandle) dlclose(dlhandle);

    // Clear the library's handle in case the exception is caught outside this function.
    dlhandle = nullptr;

    // Turn dlerror() into a runtime error.
    throw std::runtime_error{dlerror()};
  };

  // Handle any errors resulting from loading the shared object.
  handle_dlerror(dlhandle);

  // Find the create function.
  const auto create = reinterpret_cast<typename T::create_t *>(dlsym(dlhandle, "create"));
  handle_dlerror(create);

  // Find the destroy function.
  const auto destroy = reinterpret_cast<typename T::destroy_t *>(dlsym(dlhandle, "destroy"));
  handle_dlerror(destroy);

  // Create an instance of the plugin.
  auto instance = create(args...);

  // Create a deleter that calls the plugin's destroy function and frees the reference to the shared object.
  const auto deleter = [dlhandle, destroy](T *instance) {
    if (instance) destroy(instance);
    if (dlhandle) dlclose(dlhandle);
  };

  // Create a smart pointer for the plugin's instance that will call its destructor when it's deleted.
  return std::shared_ptr<T>(instance, deleter);
}
