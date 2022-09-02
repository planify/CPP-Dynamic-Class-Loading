#pragma once

#include <memory>
#include <string>
#include <dlfcn.h>
#include <iostream>
#include <utility>

template<class T>
class DLClass {
  class shared_obj {
    bool handle_dlerror(bool close_on_error) {
      if (close_on_error) close_module();

      std::cerr << dlerror() << std::endl;

      return false;
    }

  public:
    typename T::create_t *create{};

    typename T::destroy_t *destroy{};

    void *dll_handle{};

    ~shared_obj() {
      close_module();
    }

    bool open_module(const std::string &module_name) {
      dll_handle = dlopen(module_name.c_str(), RTLD_LAZY);
      if (!dll_handle) return handle_dlerror(false);

      create = (typename T::create_t *) dlsym(dll_handle, "create");
      if (!create) return handle_dlerror(true);

      destroy = (typename T::destroy_t *) dlsym(dll_handle, "destroy");
      if (!destroy) return handle_dlerror(true);

      return true;
    }

    void close_module() {
      if (dll_handle)
        dlclose(dll_handle);

      create = nullptr;
      destroy = nullptr;
      dll_handle = nullptr;
    }
  };

  std::string module;
  std::shared_ptr<shared_obj> shared;
public:
  explicit DLClass(std::string module_name) : module(std::move(module_name)) {
    shared = std::make_shared<shared_obj>();
  }

  ~DLClass() = default;

  template<typename... Args>
  [[maybe_unused]] std::shared_ptr<T> make_obj(Args... args) {
    if ((!shared->create || !shared->destroy) && !shared->open_module(module))
      return std::shared_ptr<T>(NULL);

    std::shared_ptr<shared_obj> my_shared = shared;

    return std::shared_ptr<T>(shared->create(args...), [my_shared](T *p) {
      my_shared->destroy(p);
    });
  }
};
