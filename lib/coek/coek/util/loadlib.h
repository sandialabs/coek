/*
 inspired by Ipopt LibraryHandler.h
 */
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef _MSC_VER
#include <windows.h>
typedef HINSTANCE libHandle_t;
typedef FARPROC libSymbol_t;

#else
typedef void* libHandle_t;
typedef void* libSymbol_t;
#ifdef USING_DL
#include <unistd.h>
#include <dlfcn.h>
#endif
#endif

libHandle_t loadlib(const char* libname, char* buf, size_t len);
int freelib(libHandle_t handle);
libSymbol_t getsym(libHandle_t handle, const char* symname, char* buf, size_t len);
libSymbol_t getsym_approx(libHandle_t handle, const char* symname, char* buf, size_t len);

#ifdef __cplusplus
}
#endif

