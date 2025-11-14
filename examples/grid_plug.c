#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define CSTR_IMPLEMENTATION
#include "cstr.h"
#include "grid_plug.h"


IVector2 screenToGrid(float screenX, float screenY, Grid grid) {
    return (IVector2){
        .x = ceil((screenX - grid.pos.x)/(grid.cell_size - 1)), 
        .y = ceil((screenY - grid.pos.y)/(grid.cell_size - 1))
    };
}
Vector2 gridToScreen(int x, int y, Plug plug) {
    return (Vector2){
        .x = ((float)x - 1) * plug.grid.cell_size + plug.grid.pos.x,
        .y = ((float)y - 1) * plug.grid.cell_size + plug.grid.pos.y,
    };
}

int calculateIndexFromGridCoords(int x, int y, Grid grid) {
    return (y * grid.cells_qty.x) + x;
}

void drawGrid(Grid grid) {
    for (int i = 0; i < grid.cells_qty.y; ++i) {
        for (int j = 0; j < grid.cells_qty.x; ++j) {
            float x = grid.pos.x + j*(grid.cell_size - 1);
            float y = grid.pos.y + i*(grid.cell_size - 1);
            int index = calculateIndexFromGridCoords(j, i, grid);
            int red = 255 * grid.cells[index].temperature / 100;
            DrawRectangle(x, y, grid.cell_size, grid.cell_size, (Color){red, 0, 0, 255});
            DrawRectangleLines(x, y, grid.cell_size, grid.cell_size, RED);
        }
    }
}

Cell getCell_f(Vector2 mouseCoords, Plug plug) {
    IVector2 gridCoords = screenToGrid(mouseCoords.x, mouseCoords.y, plug.grid);
    int index = calculateIndexFromGridCoords(gridCoords.x - 1, gridCoords.y - 1, plug.grid);
    return plug.grid.cells[index];
}

Cell *getCell(IVector2 gridCoords, Grid grid) {
    int index = calculateIndexFromGridCoords(gridCoords.x - 1, gridCoords.y - 1, grid);
    return &grid.cells[index];
}

void applySelectedCellProps(Plug* plug) {
    char* endptr = NULL;
    float cellTemperature = strtof(plug->txtCellTemperature.text, &endptr);
    int index = calculateIndexFromGridCoords(plug->grid.selectedCellCoords.x - 1, plug->grid.selectedCellCoords.y - 1, plug->grid);

    if (cellTemperature == 0 && strcmp(plug->txtCellTemperature.text, endptr) == 0) {
        // conversion error
        return;
    }

    if (cellTemperature > 0) {
        plug->grid.cells[index].temperature = cellTemperature;
    }

    plug->grid.cells[index].active = plug->chkCellActive.checked;
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
            IVector2 gcoord = screenToGrid(x, y, plug.grid);
            Vector2 scoord = gridToScreen(gcoord.x, gcoord.y, plug);
            sprintf(coord, "(%d, %d)\n%4.2f, %4.2f", gcoord.x, gcoord.y, scoord.x, scoord.y);
            DrawText(coord, x+gap, y+gap, font_size, GREEN);
        }
    }
}

void plug_init(Plug* plug) {
    int _rows = 3;
    int _cols = 5;
    plug->grid.cells_qty.x = _cols;
    plug->grid.cells_qty.y = _rows;
    plug->grid.pos.x = 10;
    plug->grid.pos.y = 20;
    plug->grid.cell_size = 100;
    plug->grid.size.x = plug->grid.pos.x + (plug->grid.cells_qty.x * plug->grid.cell_size);
    plug->grid.size.y = plug->grid.pos.y + (plug->grid.cells_qty.y * plug->grid.cell_size);
    int size_bytes = _rows * _cols * sizeof(Cell);
    plug->grid.cells = malloc(size_bytes);
    Cell initial_cell = {
        .active = true,
        .temperature = 50,
    };
    for (int i = 0; i < _rows * _cols; i++) {
        memcpy(&plug->grid.cells[i], &initial_cell, sizeof(Cell));
    }

    plug->btnRunSimulation.pos = (Vector2){ plug->grid.pos.x, plug->grid.pos.y + plug->grid.size.y + 15  };
    plug->btnRunSimulation.text = "Run Simulation";
    plug->btnRunSimulation.text_color = BLACK;

    plug->runningSimulation = false;

    
    { // debug Menu & Items
        plug->debugMenu.visible = false;
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
    }

    { // cellPropsWindow & Items
        plug->cellPropsWindow.visible = false;
        plug->cellPropsWindow.pos = (Vector2){ SCREEN_WIDTH/2, 3*SCREEN_HEIGHT/5};
        plug->cellPropsWindow.size = (Vector2){100, 125};

        plug->txtCellTemperature.parent = &plug->cellPropsWindow;
        plug->txtCellTemperature.pos = (Vector2){ 5, 30 };
        plug->txtCellTemperature.size = (Vector2){ 50, MG_FONT_SIZE + 4 };
        plug->txtCellTemperature.text = malloc(10);
        plug->txtCellTemperature.text_color = BLACK;
        sprintf(plug->txtCellTemperature.text, "50");

        plug->btnSetTemperature.parent = &plug->cellPropsWindow;
        plug->btnSetTemperature.pos = (Vector2){ 5, 75 };
        plug->btnSetTemperature.text = "Apply";
        plug->btnSetTemperature.text_color = BLACK;

        plug->chkCellActive.parent = &plug->cellPropsWindow;
        plug->chkCellActive.checked = false;
        plug->chkCellActive.pos = (Vector2){5, 50};
        plug->chkCellActive.text_color = BLACK;
    }

}

void transferHeat(Grid* grid) {
    // TODO: treat the borders properly
    for (int i = 1; i < grid->cells_qty.y-1; ++i) {
        for (int j = 1; j < grid->cells_qty.x-1; ++j) {
            // TODO: get rid of this hack
            i += 1;
            j += 1;
            IVector2 currIndex    = (IVector2){j    , i    };
            IVector2 upIndex      = (IVector2){j    , i - 1};
            IVector2 downIndex    = (IVector2){j    , i + 1};
            IVector2 leftIndex    = (IVector2){j - 1, i    };
            IVector2 rightIndex   = (IVector2){j + 1, i    };
            Cell *currCell = getCell(currIndex, *grid);
            Cell *upCell = getCell(upIndex, *grid);
            Cell *downCell = getCell(downIndex, *grid);
            Cell *leftCell = getCell(leftIndex, *grid);
            Cell *rightCell = getCell(rightIndex, *grid);

            float deltaRight = currCell->temperature - rightCell->temperature;
            if (deltaRight > 0) {
                TraceLog(LOG_INFO, "currCell: %d, %d", i, j);
                TraceLog(LOG_INFO, "  temp:: %f", currCell->temperature);
                rightCell->temperature += 10;
                currCell->temperature -= 10;
            }

            (void) upCell;
            (void) downCell;
            (void) leftCell;
        }
    }
    
}

#define BACKGROUND_COLOR 0x252525FF
void plug_update(Plug* plug) {
    ClearBackground(GetColor(BACKGROUND_COLOR));
    drawGrid(plug->grid);

    if (mg_button(&plug->btnRunSimulation)) {
        plug->runningSimulation = !plug->runningSimulation;
    }

    if (plug->runningSimulation) {
        plug->btnRunSimulation.text = "Stop Simulation";
        DrawText("Running simulation...", 5, 5, 5, RED);

        // simulation things here
        transferHeat(&plug->grid);

        goto nextFrame;
    }
    plug->btnRunSimulation.text = "Start Simulation";

    if (plug->chkGridCoords.checked) drawGridCoords(*plug);

    // check if a cell is selected
    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON) && is_hovered(plug->grid.pos, plug->grid.size)) {
        plug->cellPropsWindow.visible = true;
        Vector2 mouse_pos = GetMousePosition();
        plug->grid.selectedCellCoords = screenToGrid(mouse_pos.x, mouse_pos.y, plug->grid);
        Cell *cell = getCell(plug->grid.selectedCellCoords, plug->grid);
        sprintf(plug->txtCellTemperature.text, "%3.1f", cell->temperature);
        plug->chkCellActive.checked = cell->active;
    }

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
                plug->grid.size.x = plug->grid.pos.x + (plug->grid.cells_qty.x * plug->grid.cell_size);
                plug->grid.size.y = plug->grid.pos.y + (plug->grid.cells_qty.y * plug->grid.cell_size);
            }
        };
    }
    // -------------------
 
    // cellPropsWindow----
    if (IsKeyPressed(KEY_C)) plug->cellPropsWindow.visible = !plug->cellPropsWindow.visible;
    if (plug->cellPropsWindow.visible) {
        mg_container(&plug->cellPropsWindow, "Cell");
        if (mg_textbox(&plug->txtCellTemperature) == KEY_ENTER) {
            applySelectedCellProps(plug);
        }
        if (mg_button(&plug->btnSetTemperature)) {
            applySelectedCellProps(plug);
            plug->txtCellTemperature.active = false;
        }
        if (plug->chkMouseCoords.checked) applySelectedCellProps(plug);
        mg_checkbox(&plug->chkCellActive, "Active");

    }
    // -------------------

    if (plug->chkMouseCoords.checked) drawMouseCoords();
    
nextFrame:
    if (IsKeyPressed(KEY_Q)) exit(0);
    
}
