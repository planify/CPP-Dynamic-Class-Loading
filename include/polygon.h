#pragma once

class Polygon {
protected:
  double side_length_;

public:
  explicit Polygon(int length) : side_length_(length) {}

  virtual ~Polygon() = default;

  [[maybe_unused]] void set_side_length(double side_length) { side_length_ = side_length; }

  [[nodiscard]] virtual double area() const = 0;

  [[maybe_unused]] typedef Polygon * create_t(int);

  [[maybe_unused]] typedef void destroy_t(Polygon *);
};
