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
#if !defined(MACH_UNION_ATTRIBUTE)
#define MACH_UNION_ATTRIBUTE
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

typedef uint8_t MachJoystick;
typedef uint8_t MachKeyMods;

#pragma region Enums
typedef enum MachEventType {
    MachEventType_KeyPress  = 0,
    MachEventType_KeyRepeat = 1,
    MachEventType_KeyRelease = 2,
    MachEventType_CharInput = 3,
    MachEventType_MouseMotion = 4,
    MachEventType_MousePress = 5,
    MachEventType_MouseRelease = 6,
    MachEventType_MouseScroll = 7,
    MachEventType_JoystickConnected = 8,
    MachEventType_JoystickDisconnected = 9,
    MachEventType_FramebufferResize = 10,
    MachEventType_FocusGained = 11,
    MachEventType_FocusLost = 12,
    MachEventType_Close = 13,
    MachEventType_Force32 = 0x7FFFFFFF
} MachEventType MACH_ENUM_ATTRIBUTE;

typedef enum MachKey {
    MachKey_A,
    MachKey_B,
    MachKey_C,
    MachKey_D,
    MachKey_E,
    MachKey_F,
    MachKey_G,
    MachKey_H,
    MachKey_I,
    MachKey_J,
    MachKey_K,
    MachKey_L,
    MachKey_M,
    MachKey_N,
    MachKey_O,
    MachKey_P,
    MachKey_Q,
    MachKey_R,
    MachKey_S,
    MachKey_T,
    MachKey_U,
    MachKey_V,
    MachKey_W,
    MachKey_X,
    MachKey_Y,
    MachKey_Z,

    MachKey_Zero,
    MachKey_One,
    MachKey_Two,
    MachKey_Three,
    MachKey_Four,
    MachKey_Five,
    MachKey_Six,
    MachKey_Seven,
    MachKey_Eight,
    MachKey_Nine,

    MachKey_F1,
    MachKey_F2,
    MachKey_F3,
    MachKey_F4,
    MachKey_F5,
    MachKey_F6,
    MachKey_F7,
    MachKey_F8,
    MachKey_F9,
    MachKey_F10,
    MachKey_F11,
    MachKey_F12,
    MachKey_F13,
    MachKey_F14,
    MachKey_F15,
    MachKey_F16,
    MachKey_F17,
    MachKey_F18,
    MachKey_F19,
    MachKey_F20,
    MachKey_F21,
    MachKey_F22,
    MachKey_F23,
    MachKey_F24,
    MachKey_F25,

    MachKey_KPDivide,
    MachKey_KPMultiply,
    MachKey_KPSubtract,
    MachKey_KPAdd,
    MachKey_KP0,
    MachKey_KP1,
    MachKey_KP2,
    MachKey_KP3,
    MachKey_KP4,
    MachKey_KP5,
    MachKey_KP6,
    MachKey_KP7,
    MachKey_KP8,
    MachKey_KP9,
    MachKey_KPDecimal,
    MachKey_KPEqual,
    MachKey_KPEnter,

    MachKey_Enter,
    MachKey_Escape,
    MachKey_Tab,
    MachKey_LeftShift,
    MachKey_RightShift,
    MachKey_LeftControl,
    MachKey_RightControl,
    MachKey_LeftAlt,
    MachKey_RightAlt,
    MachKey_LeftSuper,
    MachKey_RightSuper,
    MachKey_Menu,
    MachKey_NumLock,
    MachKey_CapsLock,
    MachKey_Print,
    MachKey_ScrollLock,
    MachKey_Pause,
    MachKey_Delete,
    MachKey_Home,
    MachKey_End,
    MachKey_PageUp,
    MachKey_PageDown,
    MachKey_Insert,
    MachKey_Left,
    MachKey_Right,
    MachKey_Up,
    MachKey_Down,
    MachKey_Backspace,
    MachKey_Space,
    MachKey_Minus,
    MachKey_Equal,
    MachKey_LeftBracket,
    MachKey_RightBracket,
    MachKey_Backslash,
    MachKey_Semicolon,
    MachKey_Apostrophe,
    MachKey_Comma,
    MachKey_Period,
    MachKey_Slash,
    MachKey_Grave,

    MachKey_Unknown,
    MachKey_Max = MachKey_Unknown,

    MachKey_Force32 = 0x7FFFFFFF
} MachKey MACH_ENUM_ATTRIBUTE;

typedef enum MachMouseButton {
    MachMouseButton_Left,
    MachMouseButton_Right,
    MachMouseButton_Middle,
    MachMouseButton_Four,
    MachMouseButton_Five,
    MachMouseButton_Six,
    MachMouseButton_Seven,
    MachMouseButton_Eight,

    MachMouseButton_Max = MachMouseButton_Eight,

    MachMouseButton_Force32 = 0x7FFFFFFF
} MachMouseButton MACH_ENUM_ATTRIBUTE;

typedef enum MachKeyModMask {
    MachKeyModMask_Shift = 0x01,
    MachKeyModMask_Control = 0x02,
    MachKeyModMask_Alt = 0x04,
    MachKeyModMask_Super = 0x08,
    MachKeyModMask_CapsLock = 0x10,
    MachKeyModMask_NumLock = 0x20,
} MachKeyModMask MACH_ENUM_ATTRIBUTE;

typedef enum MachJoystickIndex {
    MachJoystickIndex_Zero = 0,
} MachJoystickIndex MACH_ENUM_ATTRIBUTE;
#pragma endregion

#pragma region Structs
typedef struct MachPosition {
    double x;
    double y;
} MachPosition MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachSize {
    uint32_t width;
    uint32_t height;
} MachSize MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachKeyEvent {
    MachKey key;
    MachKeyMods mods;
} MachKeyEvent MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachCharInputEvent {
    uint32_t codepoint;
} MachCharInputEvent MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachMouseMotionEvent {
    MachPosition pos;
} MachMouseMotionEvent MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachMouseButtonEvent {
    MachMouseButton button;
    MachPosition pos;
    MachKeyMods mods;
} MachMouseButtonEvent MACH_STRUCTURE_ATTRIBUTE;

typedef struct MachMouseScrollEvent {
    float xoffset;
    float yoffset;
} MachMouseScrollEvent MACH_STRUCTURE_ATTRIBUTE;

typedef union MachEventValue {
    MachKeyEvent key_press;
    MachKeyEvent key_repeat;
    MachKeyEvent key_release;
    MachCharInputEvent char_input;
    MachMouseMotionEvent mouse_motion;
    MachMouseButtonEvent mouse_press;
    MachMouseButtonEvent mouse_release;
    MachMouseScrollEvent mouse_scroll;
    MachJoystick joystick_connected;
    MachJoystick joystick_disconnected;
    MachSize framebuffer_resize;

    // Events with no payload:
    //
    // focus_gained
    // focus_lost
    // close
} MachEventValue MACH_UNION_ATTRIBUTE;

typedef struct MachEvent {
    MachEventType type;
    MachEventValue value;
} MachEvent MACH_STRUCTURE_ATTRIBUTE;

#pragma endregion

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

/// Polls for new events, populating the internal event iterator. You may then call machCoreEventsNext()
/// to fetch each event from the iterator until no more are available.
MACH_EXPORT void machCoreEventsPoll() MACH_FUNCTION_ATTRIBUTE;

/// Emits the next event in the internal event iterator, returns true if more events are available.
///
/// When machCoreEventsPoll is called, the internal event iterator is populated with events. You may
/// call this function until it returns false and the iterator is empty.
///
/// Calling this function when the iterator is empty and without a prior machCoreEventsPoll call is
/// illegal and will result in a panic.
///
/// Example usage:
///
/// ```
/// // Poll for events
/// machCoreEventsPoll();
///
/// // Consume each event that was polled, until no more are available.
/// MachEvent event;
/// while(machCoreEventsNext(&event)) {
///     if (event.type == MachEventType_KeyPress) {
///          printf("Got keypress: %d\n", event.value.key_press.key);
///     }
/// }
/// ```
MACH_EXPORT MachBool machCoreEventsNext(MachEvent* out) MACH_FUNCTION_ATTRIBUTE;

#endif  // !defined(MACH_SKIP_DECLARATIONS)

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MACH_CORE_H_
