#include <iostream>

#include <polygon.h>

struct Square : public Polygon {
  explicit Square(int x);

  ~Square() override;

  [[nodiscard]] double area() const override;
};

Square::Square(int x) : Polygon(x) {}

double Square::area() const { return side_length_ * side_length_; }

Square::~Square() {
  std::cerr << "~Square()" << std::endl;
}

extern "C" [[maybe_unused]] Polygon *create(int x) { return new Square(x); }

extern "C" [[maybe_unused]] void destroy(Polygon *p) { delete p; }
