#pragma once

#include <dlfcn.h>
#include <memory>
#include <stdexcept>

template<class T>
class DLClass {
  struct dlmanage {
    typename T::create_t *create{};

    typename T::destroy_t *destroy{};

    void *handle{};

    explicit dlmanage(const std::string &module_name) {
      handle = dlopen(module_name.c_str(), RTLD_LAZY | RTLD_NOW);
      if (handle == nullptr) throw std::runtime_error{dlerror()};

      create = (typename T::create_t *) dlsym(handle, "create");
      if (create == nullptr) throw std::runtime_error{dlerror()};

      destroy = (typename T::destroy_t *) dlsym(handle, "destroy");
      if (destroy == nullptr) throw std::runtime_error{dlerror()};
    }

    ~dlmanage() {
      dlclose(handle);
    }
  };

  std::string path;
  std::shared_ptr<dlmanage> shared;
public:
  [[maybe_unused]] explicit DLClass(std::string path) : path(std::move(path)) {}

  template<typename... Args>
  [[maybe_unused]]
  std::shared_ptr<T> make_obj(Args... args) {
    if (shared == nullptr) shared = std::make_shared<dlmanage>(this->path);
    return std::shared_ptr<T>(shared->create(args...), [&](T *p) { shared->destroy(p); });
  }
};
