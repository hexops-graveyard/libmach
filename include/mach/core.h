#ifndef MACH_CORE_H_
#define MACH_CORE_H_

#if defined(MACH_SHARED_LIBRARY)
#    if defined(_WIN32)
#        if defined(MACH_IMPLEMENTATION)
#            define MACH_EXPORT __declspec(dllexport)
#        else
#            define MACH_EXPORT __declspec(dllimport)
#        endif
#    else  // defined(_WIN32)
#        if defined(MACH_IMPLEMENTATION)
#            define MACH_EXPORT __attribute__((visibility("default")))
#        else
#            define MACH_EXPORT
#        endif
#    endif  // defined(_WIN32)
#else       // defined(MACH_SHARED_LIBRARY)
#    define MACH_EXPORT
#endif  // defined(MACH_SHARED_LIBRARY)

#if !defined(MACH_OBJECT_ATTRIBUTE)
#define MACH_OBJECT_ATTRIBUTE
#endif
#if !defined(MACH_ENUM_ATTRIBUTE)
#define MACH_ENUM_ATTRIBUTE
#endif
#if !defined(MACH_STRUCTURE_ATTRIBUTE)
#define MACH_STRUCTURE_ATTRIBUTE
#endif
#if !defined(MACH_FUNCTION_ATTRIBUTE)
#define MACH_FUNCTION_ATTRIBUTE
#endif
#if !defined(MACH_NULLABLE)
#define MACH_NULLABLE
#endif

#include <stdint.h>
#include <stddef.h>

// TODO: implement this section
// see e.g. https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h
// for how to write a modern C API header

#endif  // !defined(MACH_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MACH_CORE_H_
