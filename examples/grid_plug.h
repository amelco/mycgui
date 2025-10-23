#ifndef GRID_PLUG_H
#define GRID_PLUG_H

#include "raylib.h"
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOP_GAP 50
#define LEFT_GAP 50

// State to be stored and transefered between reloads.
// In case any of these structs change, hotreload will not work
//  and another full compilation needs to be done.
typedef struct {
    Vector2 gridQty;
    int cell_size;
    Container debugMenu;
    Checkbox chkGridCoords;
    Checkbox chkMouseCoords;
    Textbox txtCellSize;
    Button btnApply;
} Plug;

typedef void (*plug_init_t)(Plug* plug);

typedef void (*plug_update_t)(Plug* plug);


#endif //GRID_PLUG_H
