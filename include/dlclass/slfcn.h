#ifndef SLFCN
#define SLFCN

# if defined(WIN32) and not defined(_DLFCN_H) and not defined(DLFCN_H)
#   include <windef.h>
#   include <errhandlingapi.h>
#   include <libloaderapi.h>
#   include <winbase.h>
# else
#   include <dlfcn.h>
# endif

# ifdef WIN32
#   define SL_HANDLE HMODULE
# else
#   define SL_HANDLE void *
# endif

#include <string>

using std::string;

namespace DLClass {
  static const char *slerror();

  static SL_HANDLE slopen(const string &path);

  static void slclose(SL_HANDLE handle);

  template<typename T>
  static T slsym(SL_HANDLE handle, const string &name);
}

static SL_HANDLE DLClass::slopen(const string &path) {
#if defined(DLFCN_H) or defined (_DLFCN_H)
  // Clear any existing errors from previous calls.
  slerror();

  // Open the library.
  return dlopen(path.c_str(), RTLD_LAZY);
#elif defined(WIN32)
  return LoadLibraryA(path.c_str());
#endif
}

static void DLClass::slclose(SL_HANDLE handle) {
#if defined(DLFCN_H) or defined (_DLFCN_H)
  // Clear any existing errors from previous calls.
  slerror();

  // Close the library.
  dlclose(handle);
#elif defined(WIN32)
  FreeLibrary(handle);
#endif
}

template<typename T>
static T DLClass::slsym(SL_HANDLE handle, const string &name) {
#if defined(DLFCN_H) or defined (_DLFCN_H)
  // Clear any existing errors from previous calls.
  slerror();

  // Find the symbol and cast it to the given type.
  return reinterpret_cast<T>(dlsym(handle, name.c_str()));
#elif defined(WIN32)
  return reinterpret_cast<T>(GetProcAddress(handle, name.c_str()));
#endif
}

static const char *DLClass::slerror() {
#if defined(DLFCN_H) or defined (_DLFCN_H)
  return dlerror();
#elif defined(WIN32)
  LPVOID message_buffer;

  FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                nullptr, GetLastError(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
                reinterpret_cast<LPSTR>(&message_buffer), 0, nullptr);

  const auto error = static_cast<char *>(message_buffer);

  LocalFree(message_buffer);

  return error;
#endif
}

#endif
