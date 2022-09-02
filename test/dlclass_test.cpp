#include <cassert>
#include <cmath>
#include <memory>

#include <dlclass.h>
#include <polygon.h>
#include <foo.h>

static void triangle_test() {
  auto dlTriangle = new DLClass<Polygon>("./libdlclass_triangle.so");
  std::shared_ptr<Polygon> triangle = dlTriangle->make_obj(3);
  assert(triangle != nullptr);
  delete dlTriangle;
  const auto triangle_area = ceil(triangle->area() * 100.0) / 100.0;
  assert(triangle_area == 7.8);
}

static void square_test() {
  auto dlSquare = new DLClass<Polygon>("./libdlclass_square.so");
  std::shared_ptr<Polygon> square = dlSquare->make_obj(5);
  assert(square != nullptr);
  delete dlSquare;
  const auto square_area = ceil(square->area() * 100.0) / 100.0;
  assert(square_area == 25.0);
}

static void foo_test() {
  auto d = new DLClass<IFoo>("./libdlclass_foo.so");
  std::shared_ptr<IFoo> foo = d->make_obj();
  assert(foo != nullptr);
  delete d;
  assert(foo->it_works());
}

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  triangle_test();

  square_test();

  foo_test();

  return EXIT_SUCCESS;
}
