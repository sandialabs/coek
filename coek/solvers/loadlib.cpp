
#include <algorithm>
#include <string>
#include <cstdio>
#include <cstring>
#include <cctype>
#include "loadlib.h"

extern "C" libHandle_t loadlib(const char* libname, char* buf, size_t len)
{
#if !defined(_MSC_VER) && !defined(USING_DL)
snprintf(buf, len, "ERROR: loadlib() - cannot load library without the DL library");
return NULL;
#endif

if (libname == NULL) {
    snprintf(buf, len, "ERROR: loadlib() - no library name specified");
    return NULL;
    }

#if defined(_MSC_VER)
libHandle_t h = LoadLibrary(libname);
if (h == NULL) {
    snprintf(buf, len, "ERROR: loadlib() Windows error while loading dynamic library %s, error = %d.\n", libname, GetLastError());
}
#else
libHandle_t h = dlopen(libname, RTLD_NOW);
if (h == NULL) {
    strncpy(buf, dlerror(), len);
    buf[len-1] = 0;
    }
#endif
return h;
}


extern "C" int freelib(libHandle_t handle)
{
int rc=1;

#if defined(_MSC_VER)
rc = FreeLibrary(handle);
rc = !rc;
#elif defined(USING_DL)
rc = dlclose(handle);
#endif

return rc;
}


extern "C" libSymbol_t getsym(libHandle_t handle, const char* symname, char* buf, size_t len)
{
#if defined(_MSC_VER)
libSymbol_t s = GetProcAddress(handle, symname);
if (s == NULL)
    snprintf(buf, len, "ERROR: getsym() - Cannot find symbol %s in dynamic library, error = %d.", symname, GetLastError());
#elif defined(USING_DL)
libSymbol_t s = dlsym (handle, symname);
char* err = dlerror();
if (err) {
    strncpy(buf, err, len);
    buf[len-1]=0;
    return 0;
    }
#else
libSymbol_t s=0;
#endif
return s;
}


extern "C" libSymbol_t getsym_approx(libHandle_t handle, const char* symname, char* buf, size_t len)
{
libSymbol_t s=0;
std::string tmp;
// 1. original
s = getsym(handle, symname, buf, len);
if (s != NULL)
    return s;
// 2. lower_
tmp = symname;
std::transform(tmp.begin(), tmp.end(), tmp.begin(), 
    [](unsigned char c){return std::tolower(c);});
tmp += "_";
s = getsym(handle, tmp.c_str(), buf, len);
if (s != NULL)
    return s;
// 3. upper_
tmp = symname;
std::transform(tmp.begin(), tmp.end(), tmp.begin(), 
    [](unsigned char c){return std::toupper(c);});
tmp += "_";
s = getsym(handle, tmp.c_str(), buf, len);
if (s != NULL)
    return s;
// 4. original_
tmp = symname;
tmp += "_";
s = getsym(handle, tmp.c_str(), buf, len);
if (s != NULL)
    return s;
// 5. lower
tmp = symname;
std::transform(tmp.begin(), tmp.end(), tmp.begin(), 
    [](unsigned char c){return std::tolower(c);});
s = getsym(handle, tmp.c_str(), buf, len);
if (s != NULL)
    return s;
// 6. upper
tmp = symname;
std::transform(tmp.begin(), tmp.end(), tmp.begin(), 
    [](unsigned char c){return std::toupper(c);});
s = getsym(handle, tmp.c_str(), buf, len);
if (s != NULL)
    return s;
return NULL;
}
