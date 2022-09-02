#include <cassert>
#include <cmath>
#include <memory>

#include <dlclass.hpp>
#include <polygon.hpp>

int main([[maybe_unused]] int argc, [[maybe_unused]] char **argv) {
  auto dlTriangle = new DLClass<Polygon>("./libdlclass_triangle.so");
  std::shared_ptr<Polygon> triangle = dlTriangle->make_obj(3);
  assert(triangle != nullptr);
  delete dlTriangle;
  const auto triangle_area = ceil(triangle->area() * 100.0) / 100.0;
  assert(triangle_area == 7.8);

  auto dlSquare = new DLClass<Polygon>("./libdlclass_square.so");
  std::shared_ptr<Polygon> square = dlSquare->make_obj(5);
  assert(square != nullptr);
  delete dlSquare;
  const auto square_area = ceil(square->area() * 100.0) / 100.0;
  assert(square_area == 25.0);

  return EXIT_SUCCESS;
}
