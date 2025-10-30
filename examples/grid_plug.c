#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CSTR_IMPLEMENTATION
#include "cstr.h"
#include "grid_plug.h"


IVector2 screenToGrid(float screenX, float screenY, Plug plug) {
    return (IVector2){
        .x = ceil((screenX - plug.grid.pos.x)/(plug.grid.cell_size - 1)), 
        .y = ceil((screenY - plug.grid.pos.y)/(plug.grid.cell_size - 1))
    };
}
Vector2 gridToScreen(int x, int y, Plug plug) {
    return (Vector2){
        .x = ((float)x - 1) * plug.grid.cell_size + plug.grid.pos.x,
        .y = ((float)y - 1) * plug.grid.cell_size + plug.grid.pos.y,
    };
}

void drawGrid(Grid grid) {
    // TODO: make a function that gets x and y integers coordinates from the cell and outputs its one-dimensional index: (x * grid.cells_qty.x) + y
    grid.cells[2].temperature = 100;
    grid.cells[8].temperature = 100;
    for (int i = 0; i < grid.cells_qty.y; ++i) {
        for (int j = 0; j < grid.cells_qty.x; ++j) {
            float x = grid.pos.x + j*(grid.cell_size - 1);
            float y = grid.pos.y + i*(grid.cell_size - 1);
            int index = (i * grid.cells_qty.x) + j;
            int red = 255 * grid.cells[index].temperature / 100;
            DrawRectangle(x, y, grid.cell_size, grid.cell_size, (Color){red, 0, 0, 255});
            DrawRectangleLines(x, y, grid.cell_size, grid.cell_size, RED);
        }
    }
}

void drawMouseCoords() {
    const int textHeight = 10;
    Vector2 pos = GetMousePosition();
    char coord[128] = {0};
    sprintf(coord, "%4.2f, %4.2f", pos.x, pos.y);
    DrawText(coord, pos.x, pos.y - textHeight, textHeight, GREEN);
}

void drawGridCoords(Plug plug) {
    int font_size = 10;
    int gap = 3;
    int rows = plug.grid.cells_qty.y;
    int cols = plug.grid.cells_qty.x;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = plug.grid.pos.x + j*(plug.grid.cell_size) + 1;
            float y = plug.grid.pos.y + i*(plug.grid.cell_size) + 1;
            char coord[128] = {0};
            IVector2 gcoord = screenToGrid(x, y, plug);
            Vector2 scoord = gridToScreen(gcoord.x, gcoord.y, plug);
            sprintf(coord, "(%d, %d)\n%4.2f, %4.2f", gcoord.x, gcoord.y, scoord.x, scoord.y);
            DrawText(coord, x+gap, y+gap, font_size, GREEN);
        }
    }
}

void plug_init(Plug* plug) {
    int _rows = 2;
    int _cols = 5;
    plug->grid.cells_qty.x = _cols;
    plug->grid.cells_qty.y = _rows;
    plug->grid.pos.x = 10;
    plug->grid.pos.y = 20;
    plug->grid.cell_size = 100;
    int size_bytes = _rows * _cols * sizeof(Cell);
    plug->grid.cells = malloc(size_bytes);
    Cell initial_cell = {
        .active = true,
        .temperature = 50,
    };
    for (int i = 0; i < _rows * _cols; i++) {
        memcpy(&plug->grid.cells[i], &initial_cell, sizeof(Cell));
    }

    plug->debugMenu.visible = true;
    plug->debugMenu.pos = (Vector2){ 3*SCREEN_WIDTH/4, 20};
    plug->debugMenu.size = (Vector2){150, 200};

    plug->chkGridCoords.parent = &plug->debugMenu; 
    plug->chkGridCoords.pos = (Vector2){ 0, 0 };
    plug->chkGridCoords.padding = (Vector2){ 5, 25 };
    plug->chkGridCoords.checked = false;
    plug->chkGridCoords.text_color = BLACK;
    plug->chkGridCoords.checked = false;

    plug->chkMouseCoords.parent = &plug->debugMenu; 
    plug->chkMouseCoords.pos = (Vector2){ 0, 0 };
    plug->chkMouseCoords.padding = (Vector2){ 5, 40 };
    plug->chkMouseCoords.checked = false;
    plug->chkMouseCoords.text_color = BLACK;
    plug->chkMouseCoords.checked = false;

    plug->txtCellSize.parent = &plug->debugMenu;
    plug->txtCellSize.pos = (Vector2){ 5, 55 };
    plug->txtCellSize.size = (Vector2){ 50, MG_FONT_SIZE + 4 };
    plug->txtCellSize.text = malloc(5);
    plug->txtCellSize.text_color = BLACK;
    sprintf(plug->txtCellSize.text, "%d", plug->grid.cell_size);

    plug->btnApply.parent = &plug->debugMenu;
    plug->btnApply.pos = (Vector2){ 60, 55 };
    plug->btnApply.text = "Apply";
    plug->btnApply.text_color = BLACK;
    
    plug->ddOptions.parent = &plug->debugMenu;
    plug->ddOptions.pos = (Vector2){ 5, 90 };
    plug->ddOptions.size = (Vector2){ 100, MG_FONT_SIZE + 4 };
    plug->ddOptions.items = cstr_split("Item 1|Item 2", '|');
    plug->ddOptions.text_color = BLACK;
}

void plug_update(Plug* plug) {
    ClearBackground(GRAY);
    drawGrid(plug->grid);

    if (plug->chkGridCoords.checked) drawGridCoords(*plug);

    // debug menu -----
    if (IsKeyPressed(KEY_M)) plug->debugMenu.visible = !plug->debugMenu.visible;

    if (plug->debugMenu.visible) {
        mg_container(&plug->debugMenu, "Debug");
        
        mg_checkbox(&plug->chkGridCoords, "Show Grid Coords");
        mg_checkbox(&plug->chkMouseCoords, "Show Mouse Coords");

        mg_textbox(&plug->txtCellSize);
        
        if (mg_button(&plug->btnApply)) {
            int cellSize = atoi(plug->txtCellSize.text);
            if (cellSize > 0) {
                plug->grid.cell_size = cellSize;
            }
        };

        mg_dropdown(&plug->ddOptions);
    }
    // ----------------

    if (plug->chkMouseCoords.checked) drawMouseCoords();

    if (IsKeyPressed(KEY_Q)) exit(0);

}
