#include <example.h>

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

extern "C" [[maybe_unused]] Example* create() {
  return new ExampleImplementation();
}

extern "C" [[maybe_unused]] void destroy(Example *example) {
  delete example;
}
