#include <polygon.h>

struct Square : public Polygon {
  explicit Square(int x) : Polygon(x) {}

  [[nodiscard]] double area() const override {
    return side_length_ * side_length_;
  }
};

extern "C" [[maybe_unused]] Polygon *create(int x) { return new Square(x); }

extern "C" [[maybe_unused]] void destroy(Polygon *p) { delete p; }
