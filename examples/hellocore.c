#include <mach/core.h>
#include <stdlib.h>
#include <stdio.h>

void appInit(void* userdata) {
    printf("appInit!\n");
}

MachBool appUpdate(void* userdata) {
    // TODO: add another example `examples/hellocoretriangle.c` which does the same thing as this
    // one, but also renders a triangle using WebGPU

    // Poll for events
    machCoreEventsPoll();

    // Consume each event that was polled, until no more are available.
    MachEvent event;
    while(machCoreEventsNext(&event)) {
        if (event.type == MachEventType_KeyPress) {
            printf("MachEventType_KeyPress: %d\n", event.value.key_press.key);
        } else if (event.type == MachEventType_MousePress) {
            MachMouseButtonEvent press = event.value.mouse_press;
            printf("MachEventType_MousePress: button:%d, pos.x:%f, pos.y:%f\n", press.button, press.pos.x, press.pos.y);
        } else if (event.type == MachEventType_Close) {
            return 1;
        }
    }

    return 0;
}

void appDeinit(void* userdata) {
    printf("appDeinit!\n");
}

// Optional
MachBool appUpdateMainThread(void* userdata) {
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