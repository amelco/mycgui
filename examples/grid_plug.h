#ifndef GRID_PLUG_H
#define GRID_PLUG_H

#include "raylib.h"
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600

typedef struct {
    //IVector2 coords;
    bool active;
    float temperature;
} Cell;

typedef struct {
    Cell* cells;
    Vector2 pos;
    IVector2 cells_qty;
    int cell_size;
} Grid;

typedef struct {
    Grid grid;
    Container debugMenu;
    Checkbox chkGridCoords;
    Checkbox chkMouseCoords;
    Textbox txtCellSize;
    Button btnApply;
    Dropdown ddOptions;
} Plug;

typedef void (*plug_init_t)(Plug* plug);

typedef void (*plug_update_t)(Plug* plug);


#endif //GRID_PLUG_H
