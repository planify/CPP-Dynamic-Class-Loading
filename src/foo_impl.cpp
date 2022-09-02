#include <foo.h>

struct Foo : public IFoo {
  [[nodiscard]] bool it_works() const override {
    return true;
  };
};

extern "C" [[maybe_unused]] Foo *create() { return new Foo(); }

extern "C" [[maybe_unused]] void destroy(Foo *i) { delete i; }
