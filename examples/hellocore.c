#include <mach/core.h>
#include <stdlib.h>
#include <stdio.h>

void appInit(void* userdata) {
    printf("appInit!\n");
}

MachBool appUpdate(void* userdata) {
    // TODO: call machCorePollEvents here and print events
    // TODO: add another example `examples/hellocoretriangle.c` which does the same thing as this
    // one, but also renders a triangle using WebGPU
    printf("appUpdate!\n");
    return 0;
}

void appDeinit(void* userdata) {
    printf("appDeinit!\n");
}

// Optional
MachBool appUpdateMainThread(void* userdata) {
    printf("appUpdateMainThread!\n");
    return 0;
}

int main() {
    machAppStart(
        NULL, // userdata pointer
        &appInit,
        &appUpdate,
        &appDeinit,
        &appUpdateMainThread // optional; may be NULL
    );
    // TODO: machAppStart should return success/failure
    return 0;
}