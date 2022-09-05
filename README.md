# load_plugin

Header-only library for loading shared objects at runtime with the intention of creating a plugin system.

## Basic Usage

### Interface

```c++
// File: plugin.h
struct IPlugin {
  typedef IPlugin *create_t();
  typedef void destroy_t(IPlugin *);
  virtual IPlugin() = default;
  virtual ~IPlugin() = default;
};
```

### Implementation

```c++
// File: plugin.cpp
struct Plugin : public IPlugin {
  void do_something();
};

extern "C" IPlugin* create() { return new Plugin(); }

extern "C" void destroy(IPlugin *i) { delete i; }
```

### Using

```c++
#include <dlclass/load_plugin.h>
#include <plugin.h>

int main() {
  auto plugin = load_plugin<IPlugin>("libplugin.so");

  plugin->do_something();

  return 0;
}
```
