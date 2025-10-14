#include <math.h>
#include "grid_plug.h"


IVector2 screenToGrid(float pxX, float pxY) {
    return (IVector2){
        .x = ceil(pxX/CELL_SIZE), 
        .y = ceil(pxY/CELL_SIZE)
    };
}
Vector2 gridToScreen(int x, int y) {
    return (Vector2){
        .x = x * CELL_SIZE - LEFT_GAP - x+1,
        .y = y * CELL_SIZE - TOP_GAP - y+1
    };
}

void drawGrid(int rows, int cols) {
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = LEFT_GAP + i*(CELL_SIZE - 1);
            float y = LEFT_GAP + j*(CELL_SIZE - 1);
            DrawRectangleLines(x, y, CELL_SIZE, CELL_SIZE, RED);
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
    int rows = ceil(plug.gridQty.x);
    int cols = ceil(plug.gridQty.y);
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            float x = LEFT_GAP + i*(CELL_SIZE - 1);
            float y = LEFT_GAP + j*(CELL_SIZE - 1);
            char coord[128] = {0};
            IVector2 gcoord = screenToGrid(x, y);
            Vector2 scoord = gridToScreen(gcoord.x, gcoord.y);
            sprintf(coord, "  %4.2f, %4.2f\n  (%d, %d)\n  %4.2f, %4.2f", x, y, gcoord.x, gcoord.y, scoord.x, scoord.y);
            DrawText(coord, x, y, 10, GREEN);
        }
    }
}



void plug_init(Plug* plug) {
    plug->gridQty.x = 5;
    plug->gridQty.y = 3;

    plug->showGridCoords = false;
    plug->showMouseCoords = false;

    plug->debugMenu.visible = true;
    plug->debugMenu.pos = (Vector2){ 3*SCREEN_WIDTH/4, 20};
    plug->debugMenu.size = (Vector2){150, 200};

    plug->chkGridCoords.pos = (Vector2){ 0, 0 };
    plug->chkGridCoords.padding = (Vector2){ 5, 25 };
    plug->chkGridCoords.checked = false;

    plug->chkMouseCoords.pos = (Vector2){ 0, 0 };
    plug->chkMouseCoords.padding = (Vector2){ 5, 40 };
    plug->chkMouseCoords.checked = false;
}

void plug_update(Plug* plug) {
    ClearBackground(GRAY);
    drawGrid(plug->gridQty.x, plug->gridQty.y);

    if (plug->showGridCoords) drawGridCoords(*plug);

    // debug menu -----
    if (IsKeyPressed(KEY_M)) plug->debugMenu.visible = !plug->debugMenu.visible;
    if (plug->debugMenu.visible) {
        plug->chkGridCoords.pos = plug->debugMenu.pos;
        plug->chkMouseCoords.pos = plug->debugMenu.pos;
        mg_container(&plug->debugMenu, "Debug");
        mg_checkbox(&plug->chkGridCoords, "Show Grid Coords");
        mg_checkbox(&plug->chkMouseCoords, "Show Mouse Coords");
    }
    plug->showGridCoords = plug->chkGridCoords.checked;
    plug->showMouseCoords = plug->chkMouseCoords.checked;
    // ----------------

    if (plug->showMouseCoords) drawMouseCoords();

    if (IsKeyPressed(KEY_Q)) exit(0);
}
