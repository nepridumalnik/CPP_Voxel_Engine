#pragma once

#if defined(_MSC_VER)
// Microsoft
#define EXPORT __declspec(dllexport)
#define IMPORT __declspec(dllimport)
#elif defined(__GNUC__)
// GCC
#define EXPORT __attribute__((visibility("default")))
#define IMPORT
#else
#define EXPORT
#define IMPORT
#pragma warning Unknown dynamic link import / export semantics
#endif

#define DISABLE_COPY_MOVE_CTRS(ClassName)                                                          \
    ClassName(ClassName &&) = delete;                                                              \
    ClassName(const ClassName &) = delete;                                                         \
    ClassName &operator=(ClassName &&) = delete;                                                   \
    void operator=(const ClassName &) = delete
