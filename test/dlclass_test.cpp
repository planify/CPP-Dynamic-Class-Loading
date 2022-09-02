#include <cassert>
#include <cmath>

#include <dlclass.h>
#include <polygon.h>
#include <foo.h>

static void triangle_test() {
  auto plugin = DLClass<Polygon>{"./libdlclass_triangle.so"};
  auto triangle = plugin.make_obj(3);
  assert(triangle->area() > 7.0);
  assert(triangle->area() < 8.0);
}

static void square_test() {
  auto plugin = DLClass<Polygon>{"./libdlclass_square.so"};
  auto square = plugin.make_obj(5);
  assert(square->area() == 25.0);
}

static void foo_test() {
  auto plugin = DLClass<IFoo>{"./libdlclass_foo.so"};
  auto foo = plugin.make_obj();
  assert(foo->it_works());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  square_test();

  triangle_test();

  foo_test();

  return EXIT_SUCCESS;
}
