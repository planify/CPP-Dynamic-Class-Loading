#include <cmath>

#include <polygon.h>

struct Triangle : public Polygon {
  explicit Triangle(int x);

  [[nodiscard]] double area() const override;
};

Triangle::Triangle(int x) : Polygon(x) {}

double Triangle::area() const { return side_length_ * side_length_ * sqrt(3) / 2; }

extern "C" [[maybe_unused]] Polygon *create(int x) { return new Triangle(x); }

extern "C" [[maybe_unused]] void destroy(Polygon *p) { delete p; }
