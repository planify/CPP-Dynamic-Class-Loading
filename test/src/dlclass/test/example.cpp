#include <dlclass/test/example.h>

using DLClass::Test::Example;

namespace DLClass::Test {
  class ExampleImplementation : public Example {
    int value{0};
  public:
    void set_value(int new_value) override {
      value = new_value;
    }

    int get_value() override {
      return value;
    }
  };
}

extern "C" [[maybe_unused]] Example* create() {
  return new DLClass::Test::ExampleImplementation();
}

extern "C" [[maybe_unused]] void destroy(Example *example) {
  delete example;
}
