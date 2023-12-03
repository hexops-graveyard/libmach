const std = @import("std");
const core = @import("mach-core");
const c = @cImport(@cInclude("mach/core.h"));
comptime {
    if (!@import("builtin").is_test) core.AppInterface(CApp);
}

pub const GPUInterface = core.wgpu.dawn.Interface;
pub const DGPUInterface = core.dusk.Impl;

const CApp = struct {
    pub const App = @This();

    userdata: ?*anyopaque,
    app_init: c.MachAppInitCallback,
    app_update: c.MachAppUpdateCallback,
    app_deinit: c.MachAppDeinitCallback,
    app_update_main_thread: c.MachAppUpdateMainThreadCallback,

    pub fn init(app: *App) !void {
        // TODO: provide a C API machCoreinit
        try core.init(.{});
        // TODO: in debug builds, provide a nice error if this is null
        app.app_init.?(app.userdata);
    }

    pub fn deinit(app: *App) void {
        // TODO: in debug builds, provide a nice error if this is null
        app.app_deinit.?(app.userdata);
        core.deinit();
    }

    pub fn updateMainThread(app: *App) !bool {
        if (app.app_update_main_thread) |callback| return callback(app.userdata) > 0;
        return false;
    }

    pub fn update(app: *App) !bool {
        // TODO: in debug builds, provide a nice error if this is null
        return app.app_update.?(app.userdata) > 0;
    }
};

export fn machAppStart(
    userdata: ?*anyopaque,
    app_init: c.MachAppInitCallback,
    app_update: c.MachAppUpdateCallback,
    app_deinit: c.MachAppDeinitCallback,
    app_update_main_thread: c.MachAppUpdateMainThreadCallback,
) void {
    // TODO: error handling
    machAppStartFallible(
        userdata,
        app_init,
        app_update,
        app_deinit,
        app_update_main_thread,
    ) catch unreachable;
}

fn machAppStartFallible(
    userdata: ?*anyopaque,
    app_init: c.MachAppInitCallback,
    app_update: c.MachAppUpdateCallback,
    app_deinit: c.MachAppDeinitCallback,
    app_update_main_thread: c.MachAppUpdateMainThreadCallback,
) !void {
    var gpa = std.heap.GeneralPurposeAllocator(.{}){};
    defer _ = gpa.deinit();
    core.allocator = gpa.allocator();

    // Initialize GPU implementation
    if (comptime core.options.use_wgpu) try core.wgpu.Impl.init(core.allocator, .{});
    if (comptime core.options.use_sysgpu) try core.sysgpu.Impl.init(core.allocator, .{});

    var app: CApp = .{
        .userdata = userdata,
        .app_init = app_init,
        .app_update = app_update,
        .app_deinit = app_deinit,
        .app_update_main_thread = app_update_main_thread,
    };
    try app.init();
    defer app.deinit();
    while (!try core.update(&app)) {}
}

// Global event iterator shared between machCoreEventsPoll and machCoreEventsNext calls.
var iter: ?core.EventIterator = null;
export fn machCoreEventsPoll() void {
    if (iter != null) @panic("machCoreEventsPoll called without consuming all previous events");
    iter = core.pollEvents();
}

export fn machCoreEventsNext(out: *c.MachEvent) c.MachBool {
    if (iter == null) @panic("machCoreEventsNext called without calling machCoreEventsPoll first");

    var event = iter.?.next() orelse {
        iter = null;
        return 0;
    };

    switch (event) {
        .key_press => |v| {
            out.type = c.MachEventType_KeyPress;
            out.value.key_press.key = @intFromEnum(v.key);
            out.value.key_press.mods = @bitCast(v.mods);
        },
        .key_repeat => |v| {
            out.type = c.MachEventType_KeyRepeat;
            out.value.key_repeat.key = @intFromEnum(v.key);
            out.value.key_repeat.mods = @bitCast(v.mods);
        },
        .key_release => |v| {
            out.type = c.MachEventType_KeyRelease;
            out.value.key_release.key = @intFromEnum(v.key);
            out.value.key_release.mods = @bitCast(v.mods);
        },
        .char_input => |v| {
            out.type = c.MachEventType_CharInput;
            out.value.char_input.codepoint = v.codepoint;
        },
        .mouse_motion => |v| {
            out.type = c.MachEventType_MouseMotion;
            out.value.mouse_motion.pos.x = v.pos.x;
            out.value.mouse_motion.pos.y = v.pos.y;
        },
        .mouse_press => |v| {
            out.type = c.MachEventType_MousePress;
            out.value.mouse_press.button = @intFromEnum(v.button);
            out.value.mouse_press.pos.x = v.pos.x;
            out.value.mouse_press.pos.y = v.pos.y;
            out.value.mouse_press.mods = @bitCast(v.mods);
        },
        .mouse_release => |v| {
            out.type = c.MachEventType_MouseRelease;
            out.value.mouse_release.button = @intFromEnum(v.button);
            out.value.mouse_release.pos.x = v.pos.x;
            out.value.mouse_release.pos.y = v.pos.y;
            out.value.mouse_release.mods = @bitCast(v.mods);
        },
        .mouse_scroll => |v| {
            out.type = c.MachEventType_MouseScroll;
            out.value.mouse_scroll.xoffset = v.xoffset;
            out.value.mouse_scroll.yoffset = v.yoffset;
        },
        .joystick_connected => |v| {
            out.type = c.MachEventType_JoystickConnected;
            out.value.joystick_connected = @intFromEnum(v);
        },
        .joystick_disconnected => |v| {
            out.type = c.MachEventType_JoystickDisconnected;
            out.value.joystick_disconnected = @intFromEnum(v);
        },
        .framebuffer_resize => |v| {
            out.type = c.MachEventType_FramebufferResize;
            out.value.framebuffer_resize.width = v.width;
            out.value.framebuffer_resize.height = v.height;
        },
        .focus_gained => out.type = c.MachEventType_FocusGained,
        .focus_lost => out.type = c.MachEventType_FocusLost,
        .close => out.type = c.MachEventType_Close,
    }

    return 1;
}
