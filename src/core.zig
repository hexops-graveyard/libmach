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
        // TODO: provide a C API to access a core.pollEvents iterator
        var iter = core.pollEvents();
        while (iter.next()) |event| {
            switch (event) {
                .close => return true,
                else => {},
            }
        }

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
