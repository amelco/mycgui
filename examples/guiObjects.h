#ifndef GUI_OBJECTS_H_
#define GUI_OBJECTS_H_
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
#endif //GUI_OBJECTS_H_
