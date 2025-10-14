#ifndef GUI_OBJECTS+H+
#define GUI_OBJECTS+H+
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
#endif //GUI_OBJECTS+H+
