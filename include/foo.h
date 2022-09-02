#pragma once

struct IFoo {
  IFoo() = default;

  virtual ~IFoo() = default;

  [[nodiscard]] virtual bool it_works() const = 0;

  [[maybe_unused]] typedef IFoo *create_t();

  [[maybe_unused]] typedef void destroy_t(IFoo *);
};
