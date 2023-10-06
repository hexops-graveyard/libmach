const std = @import("std");
const mach_core = @import("mach_core");

pub fn build(b: *std.Build) void {
    const target = b.standardTargetOptions(.{});
    const optimize = b.standardOptimizeOption(.{});

    const mach_core_dep = b.dependency("mach_core", .{
        .target = target,
        .optimize = optimize,
    });

    const libmachcore = b.addStaticLibrary(.{
        .name = "machcore",
        .root_source_file = .{ .path = "src/core.zig" },
        .target = target,
        .optimize = optimize,
    });
    libmachcore.addIncludePath(.{ .path = "include" });
    libmachcore.addModule("mach-core", mach_core_dep.module("mach-core"));
    libmachcore.installHeadersDirectory("include/mach", "mach");
    @import("mach_core").link(mach_core_dep.builder, libmachcore);
    const install_libmachcore = b.addInstallArtifact(libmachcore, .{});
    b.getInstallStep().dependOn(&install_libmachcore.step);

    const main_tests = b.addTest(.{
        .root_source_file = .{ .path = "src/main.zig" },
        .target = target,
        .optimize = optimize,
    });
    const run_main_tests = b.addRunArtifact(main_tests);

    const test_step = b.step("test", "Run library tests");
    test_step.dependOn(&run_main_tests.step);

    buildHelloCore(
        b,
        target,
        optimize,
        &install_libmachcore.step,
        libmachcore.getEmittedBinDirectory(),
    );
}

pub fn buildHelloCore(
    b: *std.Build,
    target: std.zig.CrossTarget,
    optimize: std.builtin.OptimizeMode,
    install_libmachcore: *std.build.Step,
    libmachcore: std.Build.LazyPath,
) void {
    const exe = b.addExecutable(.{
        .name = "hellocore",
        .root_source_file = .{ .path = "examples/hellocore.c" },
        .target = target,
        .optimize = optimize,
    });
    exe.addIncludePath(.{ .path = "include" });
    exe.linkLibC();

    // Link against libmachcore
    exe.linkSystemLibrary("machcore");
    exe.addLibraryPath(libmachcore);
    exe.step.dependOn(install_libmachcore);

    // TODO: this shouldn't be needed but is right now.
    const mach_core_dep = b.dependency("mach_core", .{
        .target = target,
        .optimize = optimize,
    });
    @import("mach_core").link(mach_core_dep.builder, exe);

    // Install the exe and run it when `zig build run-hellocore` is invoked
    b.installArtifact(exe);
    const run_cmd = b.addRunArtifact(exe);
    run_cmd.step.dependOn(b.getInstallStep());
    const run_step = b.step("run-hellocore", "Run examples/hellocore");
    run_step.dependOn(&run_cmd.step);
}
