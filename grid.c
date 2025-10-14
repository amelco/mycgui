#include <math.h>
#include <stdbool.h>
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define TOP_GAP 50
#define LEFT_GAP 50
#define CELL_SIZE 100

// switches
bool showGridCoords = false;
bool showMouseCoords = true;

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
            if (showGridCoords) {
                char coord[128] = {0};
                IVector2 gcoord = screenToGrid(x, y);
                Vector2 scoord = gridToScreen(gcoord.x, gcoord.y);
                sprintf(coord, "  %4.2f, %4.2f\n  (%d, %d)\n  %4.2f, %4.2f", x, y, gcoord.x, gcoord.y, scoord.x, scoord.y);
                DrawText(coord, x, y, 10, GREEN);
            }
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


Container debugMenu = {
    .visible = true,
    .pos = (Vector2){ 3*SCREEN_WIDTH/4, 20},
    .size = (Vector2){150, 200},
};

Checkbox chkGridCoords = {
    .pos = {0},
    .padding = (Vector2){ 5, 25 },
    .checked = false
};

Checkbox chkMouseCoords = {
    .pos = {0},
    .padding = (Vector2){ 5, 40 },
    .checked = false
};

int main() {
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Test application of mygui.h");
    while (!WindowShouldClose()) {
    BeginDrawing();
        ClearBackground(GRAY);
        drawGrid(5, 3);

        // debug menu -----
        if (IsKeyPressed(KEY_M)) debugMenu.visible = !debugMenu.visible;
        if (debugMenu.visible) {
            chkGridCoords.pos = debugMenu.pos;
            chkMouseCoords.pos = debugMenu.pos;
            mg_container(&debugMenu, "Debug");
            mg_checkbox(&chkGridCoords, "Show Grid Coords");
            mg_checkbox(&chkMouseCoords, "Show Mouse Coords");
        }
        showGridCoords = chkGridCoords.checked;
        showMouseCoords = chkMouseCoords.checked;
        // ----------------

        if (IsKeyPressed(KEY_G)) showGridCoords = !showGridCoords;  // TODO: make a toggle component in mygui
        if (showMouseCoords) drawMouseCoords();
        
        if (IsKeyPressed(KEY_Q)) return 0;
    EndDrawing();
    }
    return 0;
}
