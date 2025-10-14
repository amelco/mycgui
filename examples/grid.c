#include <math.h>
#include <stdbool.h>
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOP_GAP 50
#define LEFT_GAP 50
#define CELL_SIZE 100

#include "grid_plug.c"



int main() {
    Plug plug = {0};
    plug_init(&plug);

    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test application of mygui.h");
    while (!WindowShouldClose()) {
        // update state can be done here

        BeginDrawing();
        plug_update(&plug);
        EndDrawing();
    }
    return 0;
}
