#ifndef DLCLASS_SLFCN_H
#define DLCLASS_SLFCN_H

#include <string>

// Check for dlfcn implementation. DLFCN_H is used by dlfcn-win32, and _DLFCN_H by glibc. Default to using whatever
// implementation is available before loading this file, use the glibc implementation on Linux, or LoadLibrary on
// Windows.
# if defined(DLFCN_H) or defined(_DLFCN_H)
#   define DLFCN_PRESENT
# else
#   ifdef WIN32
#     include <windef.h>
#     include <errhandlingapi.h>
#     include <libloaderapi.h>
#     include <winbase.h>
#   else
#     include <dlfcn.h>
#     define DLFCN_PRESENT
#   endif
# endif

using std::string;

static SL_HANDLE DLClass::slopen(const string &path) {
#ifdef DLFCN_PRESENT
  // Clear any existing errors from previous calls.
  slerror();

  // Open the library.
  return dlopen(path.c_str(), RTLD_LAZY);
#elif defined(WIN32)
  return LoadLibraryA(path.c_str());
#endif
}

static void DLClass::slclose(SL_HANDLE handle) {
#ifdef DLFCN_PRESENT
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
#ifdef DLFCN_PRESENT
  // Clear any existing errors from previous calls.
  slerror();

  // Find the symbol and cast it to the given type.
  return reinterpret_cast<T>(dlsym(handle, name.c_str()));
#elif defined(WIN32)
  return reinterpret_cast<T>(GetProcAddress(handle, name.c_str()));
#endif
}

static const char *DLClass::slerror() {
#ifdef DLFCN_PRESENT
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
