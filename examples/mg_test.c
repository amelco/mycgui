#include <stdio.h>
#include <dlfcn.h>
#define CSTR_IMPLEMENTATION
#include "cstr.h"
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

#include "mg_test_plug.h"

plug_init_t plug_init = NULL; 
plug_update_t plug_update = NULL; 
plug_end_t plug_end = NULL; 

void plug_load() {
    TraceLog(LOG_INFO, "Hot reloading...");
    void* handle = dlopen("libmg_test.so", RTLD_NOW);
    if (!handle) {
        fprintf(stderr, "Could not load 'libmg_test.so' file: %s", dlerror());
        exit(1);
    }
    plug_init = dlsym(handle, "plug_init");
    if (!plug_init) {
        fprintf(stderr, "Could not load 'plug_init' function: %s", dlerror());
        exit(1);
    }
    plug_update = dlsym(handle, "plug_update");
    if (!plug_update) {
        fprintf(stderr, "Could not load 'plug_update' function: %s", dlerror());
        exit(1);
    }
    plug_end = dlsym(handle, "plug_end");
    if (!plug_end) {
        fprintf(stderr, "Could not load 'plug_end' function: %s", dlerror());
        exit(1);
    }
    TraceLog(LOG_INFO, "Hot reloading successful.");
}

int main() {
    Plug plug = {0};

    plug_load();
    plug_init(&plug);

    InitWindow(800, 600, "Test application of mygui.h");
    while (!WindowShouldClose())
    {
        BeginDrawing();

        plug_update(&plug);

        if (IsKeyPressed(KEY_Q)) return 0;
        if (IsKeyPressed(KEY_H)) plug_load(&plug);
        EndDrawing();
    }
    plug_end(&plug);
    return 0;
}
