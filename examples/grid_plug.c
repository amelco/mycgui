#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CSTR_IMPLEMENTATION
#include "cstr.h"
#include "grid_plug.h"


IVector2 screenToGrid(float screenX, float screenY, Plug plug) {
    return (IVector2){
        .x = ceil((screenX - plug.gridPos.x)/(plug.cell_size - 1)), 
        .y = ceil((screenY - plug.gridPos.y)/(plug.cell_size - 1))
    };
}
Vector2 gridToScreen(int x, int y, Plug plug) {
    return (Vector2){
        .x = ((float)x - 1) * plug.cell_size + plug.gridPos.x,
        .y = ((float)y - 1) * plug.cell_size + plug.gridPos.y,
    };
}

void drawGrid(int rows, int cols, Plug plug) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = plug.gridPos.x + i*(plug.cell_size - 1);
            float y = plug.gridPos.y + j*(plug.cell_size - 1);
            DrawRectangleLines(x, y, plug.cell_size, plug.cell_size, RED);
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
    int rows = plug.gridQty.y;
    int cols = plug.gridQty.x;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = plug.gridPos.x + j*(plug.cell_size) + 1;
            float y = plug.gridPos.y + i*(plug.cell_size) + 1;
            char coord[128] = {0};
            IVector2 gcoord = screenToGrid(x, y, plug);
            Vector2 scoord = gridToScreen(gcoord.x, gcoord.y, plug);
            sprintf(coord, "(%d, %d)\n%4.2f, %4.2f", gcoord.x, gcoord.y, scoord.x, scoord.y);
            DrawText(coord, x+gap, y+gap, font_size, GREEN);
        }
    }
}

void plug_init(Plug* plug) {
    plug->gridPos.x = 10;
    plug->gridPos.y = 20;

    plug->gridQty.x = 5;
    plug->gridQty.y = 2;
    plug->cell_size = 100;

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
    sprintf(plug->txtCellSize.text, "%d", plug->cell_size);

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
    drawGrid(plug->gridQty.x, plug->gridQty.y, *plug);

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
                plug->cell_size = cellSize;
            }
        };

        mg_dropdown(&plug->ddOptions);
    }
    // ----------------

    if (plug->chkMouseCoords.checked) drawMouseCoords();

    if (IsKeyPressed(KEY_Q)) exit(0);

}
