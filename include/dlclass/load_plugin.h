#pragma once

#include <memory>
#include <stdexcept>

// Allow the user to provide their own dlfcn implementation by including it before this file.
# ifndef _DLFCN_H
#   ifdef WIN32
#     include <windef.h>
#     include <errhandlingapi.h>
#     include <libloaderapi.h>
#     include <winbase.h>
#   else
#     include <dlfcn.h>
#   endif
# endif

template<typename T, typename ...A>
static std::shared_ptr<T> load_plugin(const std::string &path, A... args) {

  // Provide a minimal implementation of dlfcn when not provided by the program using the library.
# if defined(WIN32) and not defined(_DLFCN_H)
#   define RTLD_LAZY 0x00001
#   define dlopen(path, mode) (LoadLibraryA(path))
#   define dlclose(handle) (FreeLibrary(reinterpret_cast<HMODULE>(handle)))
#   define dlsym(handle, name) (reinterpret_cast<void *>(GetProcAddress(static_cast<HMODULE>(handle), name)))
# else
  // Clear any existing errors from previous calls.
  dlerror();
# endif

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

#ifdef WIN32
    LPVOID message_buffer;

    FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                  nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                  reinterpret_cast<LPSTR>(&message_buffer), 0, nullptr);

    const auto error = static_cast<char *>(message_buffer);

    LocalFree(message_buffer);

    throw std::runtime_error{error};
#else
    // Turn dlerror() into a runtime error.
    throw std::runtime_error{dlerror()};
#endif
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
