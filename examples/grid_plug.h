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
    Vector2 pos;
    Vector2 size;
    Cell* cells;
    IVector2 cells_qty;
    int cell_size;
    IVector2 selectedCellCoords;
} Grid;

typedef struct {
    Grid grid;
    
    Container debugMenu;
    Checkbox chkGridCoords;
    Checkbox chkMouseCoords;
    Textbox txtCellSize;
    Button btnApply;

    Container cellPropsWindow;
    Textbox txtCellTemperature;
    Button btnSetTemperature;
    
} Plug;

typedef void (*plug_init_t)(Plug* plug);

typedef void (*plug_update_t)(Plug* plug);


#endif //GRID_PLUG_H
