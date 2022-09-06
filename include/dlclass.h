#ifndef DLCLASS_H
#define DLCLASS_H

#include <memory>
#include <string>

# ifndef SL_HANDLE
#   ifdef WIN32
#     define SL_HANDLE HMODULE
#   else
#     define SL_HANDLE void *
#   endif
# endif

namespace DLClass {
  template<typename T, typename ...A>
  static std::shared_ptr<T> load_plugin(const std::string &path, A... args);

  static const char *slerror();

  static SL_HANDLE slopen(const std::string &path);

  static void slclose(SL_HANDLE handle);

  template<typename T>
  static T slsym(SL_HANDLE handle, const std::string &name);
}

#endif
