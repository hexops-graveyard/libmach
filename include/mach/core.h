#ifndef MACH_CORE_H_
#define MACH_CORE_H_

// Note: you may reference e.g. this for how to write a modern C header:
// https://github.com/webgpu-native/webgpu-headers/blob/main/webgpu.h

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

#ifdef __cplusplus
extern "C" {
#endif

typedef uint32_t MachBool;

typedef void (*MachAppInitCallback)(MACH_NULLABLE void* userdata) MACH_FUNCTION_ATTRIBUTE;
typedef MachBool (*MachAppUpdateCallback)(MACH_NULLABLE void* userdata) MACH_FUNCTION_ATTRIBUTE;
typedef MachBool (*MachAppUpdateMainThreadCallback)(MACH_NULLABLE void* userdata) MACH_FUNCTION_ATTRIBUTE;
typedef void (*MachAppDeinitCallback)(MACH_NULLABLE void* userdata) MACH_FUNCTION_ATTRIBUTE;

#if !defined(MACH_SKIP_DECLARATIONS)

/// Called to start the application. The userdata will be passed to each callback.
///
/// * `app_init` will be called on the main thread, and should invoke `machCoreInit`.
/// * `app_deinit` will be called on the main thread.
/// * `app_update` will be called on the render thread, and should invoke `machCorePollEvents`.
///
/// `app_update_main_thread` may be null or a callback to be invoked on the main thread. For details
/// see the Zig code. TODO: this is incomplete, the user needs access to APIs to control main thread
/// execution.
MACH_EXPORT void machAppStart(
    MACH_NULLABLE void* userdata,
    MachAppInitCallback app_init,
    MachAppUpdateCallback app_update,
    MachAppDeinitCallback app_deinit,
    MACH_NULLABLE MachAppUpdateMainThreadCallback app_update_main_thread
) MACH_FUNCTION_ATTRIBUTE;

#endif  // !defined(MACH_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MACH_CORE_H_
