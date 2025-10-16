#include <stdio.h>
#include <math.h>
#include <stdbool.h>
#include <dlfcn.h>  // (hot reload)
#include <assert.h>
#define MYGUI_IMPLEMENTATION
#include "mygui.h"


#include "grid_plug.h"

void* handle;       // opaque pointer to handle hot reload
Plug plug = {0};    // state of the application
// mapping the types of the function pointers to their implementations names
plug_init_t plug_init = NULL;
plug_update_t plug_update = NULL;

// dynamically loads the functions of the grid_plug.c (hot reload)
void load_plug() {
    if (handle) dlclose(handle);
    handle = dlopen("./bin/libgrid.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Could not load libgrid.so: %s\n", dlerror());
        exit(1);
    }

    plug_init = dlsym(handle, "plug_init");
    assert(plug_init != NULL && "Could not load plug_init function");

    plug_update = dlsym(handle, "plug_update");
    assert(plug_update != NULL && "Could not load plug_update function");
}

int main() {
    load_plug();
    plug_init(&plug);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test application of mygui.h");
    while (!WindowShouldClose()) {
        // update state can be done here

        BeginDrawing();
        plug_update(&plug);
        if (IsKeyPressed(KEY_H)) load_plug();
        if (IsKeyPressed(KEY_R)) {
            load_plug();
            plug_init(&plug);
        }
        EndDrawing();
    }
    return 0;
}
