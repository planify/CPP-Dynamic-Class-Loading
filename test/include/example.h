#pragma once

struct Example {
  Example() = default;

  virtual ~Example() = default;

  virtual int get_value() = 0;

  virtual void set_value(int value) = 0;

  [[maybe_unused]] typedef Example *create_t();

  [[maybe_unused]] typedef void destroy_t(Example *);
};
