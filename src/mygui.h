#ifndef MYGUI_H_
#define MYGUI_H_

#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"

#include "cstr.h"

#define MG_FOREGROUND_COLOR LIGHTGRAY
#define MG_BACKGROUND_COLOR (Color){240, 240, 240, 255}
#define MG_TEXT_COLOR       (Color){20,   20,  20, 255}
#define MG_FONT_SIZE 15
#define MG_ARENA_DEFAULT_SIZE 1024 * 1024  // 1 MB

const char allowed_keys[] = {39, 44, 45, 46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 59, 61, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78, 79, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 96,32};

bool is_key_allowed(int k) {
    for (int i = 0; i < (int)sizeof(allowed_keys)/(int)sizeof(allowed_keys[0]); ++i) {
        if (k == allowed_keys[i]) return true;
    }
    return false;
}

// Arena
typedef struct {
    int capacity;
    int size;
    void* memory;
} Arena;
void arena_init(Arena* arena, int size) {
    arena->capacity = size;
    arena->size = 0;
    arena->memory = calloc(size, sizeof(char));
}
void* arena_alloc(Arena* arena, size_t size) {
    if (arena == NULL) arena_init(arena, MG_ARENA_DEFAULT_SIZE);

    // TODO: make it dynamic
    if (arena->size > arena->capacity) {
        TraceLog(LOG_ERROR, "Arena reached its limit. Maybe is a memory leak? Allocate more memory on its initialization.");
        exit(1);
    }
    void* r = arena->memory + size;
    arena->size += size;
    return r;
}
/////////////


typedef struct IVector2 {
    int x;
    int y;
} IVector2;

#define UICOMMOM \
    Vector2 pos;\
    Vector2 size;\
    Color text_color;\
    void* parent;

typedef struct {
    UICOMMOM;
    char* text;
} Button;

typedef struct {
    UICOMMOM;
    bool active;
    int selected_index;
    bool selected_item; 
    char* selected_text;
    CstrList items;
} Dropdown;

typedef struct {
    UICOMMOM;
    bool active;
    int extra_chars;
    char* text;
} Textbox;

typedef struct {
    UICOMMOM;
    bool visible;
} Container;

typedef struct {
    UICOMMOM;
    Vector2 padding;
    bool checked;
} Checkbox;

bool mg_button(Button btn);
void mg_dropdown(Dropdown* dd);
void mg_textbox(Textbox* tb);
void mg_container(Container* cc, const char* title);
void mg_checkbox(Checkbox* chk, const char* text);



///////////////////////////////////////////////////
#ifdef MYGUI_IMPLEMENTATION

bool is_hovered(Vector2 thing_pos, Vector2 thing_size) {
  Vector2 mouse_pos = GetMousePosition();
  return mouse_pos.x >= thing_pos.x && mouse_pos.x <= thing_pos.x+thing_size.x && mouse_pos.y >= thing_pos.y && mouse_pos.y <= thing_pos.y+thing_size.y;
}

bool mg_button(Button btn) {
    Vector2 parent_pos = {0};
    if (btn.parent != NULL) {
        Container* cnt = btn.parent;
        parent_pos.x = cnt->pos.x;
        parent_pos.y = cnt->pos.y;
    }

    int font_size = 15;
    float padding = font_size + 1.1;
    Vector2 size = {
        MeasureText(btn.text, font_size) + padding,
        (15 + padding)
    };

    bool hovered = is_hovered(Vector2Add(parent_pos, btn.pos), size);
    DrawRectangleV(Vector2Add(parent_pos, btn.pos), size, hovered ? GRAY : LIGHTGRAY);

    int x = parent_pos.x + btn.pos.x + padding / 2;
    int y = parent_pos.y + btn.pos.y + padding / 2;
    DrawText(btn.text, x, y, MG_FONT_SIZE, MG_TEXT_COLOR);
    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
    return false;
}

void mg_container(Container* cc, const char* title) {
    int textSize = 20;
    int titleHeight = 20;
    if (cc->visible) {
        DrawRectangleV(cc->pos, cc->size, MG_BACKGROUND_COLOR);

        // title bar
        DrawRectangle(cc->pos.x, cc->pos.y, cc->size.x, titleHeight, BLUE);
        Rectangle closeArea = {
            .x = cc->pos.x + cc->size.x - titleHeight,
            .y = cc->pos.y,
            .width = textSize,
            .height = titleHeight
        };
        DrawText(title, cc->pos.x + 5, cc->pos.y, titleHeight, BLACK);
        DrawText("x", closeArea.x, closeArea.y, closeArea.height, BLACK);

        DrawRectangleLines(cc->pos.x, cc->pos.y, cc->size.x, cc->size.y, BLACK);

        bool closeAreaHovered = is_hovered(
                (Vector2){ .x = closeArea.x, .y = closeArea.y },
                (Vector2){ .x = closeArea.width, .y = closeArea.y }
                );
        if (closeAreaHovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) cc->visible = false;
    }
    bool hovered = is_hovered(cc->pos, cc->size);
    if (hovered && IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        cc->pos.x += GetMouseDelta().x;
        cc->pos.y += GetMouseDelta().y;
    }
}


void mg_checkbox(Checkbox* chk, const char* text) {
    Vector2 parent_pos = {0};
    if (chk->parent != NULL) {
        Container* cnt = chk->parent;
        parent_pos.x = cnt->pos.x;
        parent_pos.y = cnt->pos.y;
    }

    int font_size = 5;
    Vector2 size = {10, 10};

    int x = chk->pos.x + parent_pos.x + chk->padding.x;
    int y = chk->pos.y + parent_pos.y + chk->padding.y;

    bool hovered = is_hovered((Vector2){x, y}, size);

    if (chk->checked) DrawRectangle(x, y, size.x, size.y, GREEN);
    DrawRectangleLines(x, y, size.x, size.y, hovered ? BLUE : BLACK);
    DrawText(text, x + size.x + 5, y, font_size, chk->text_color);
    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) chk->checked = !chk->checked;
}

void mg_dropdown(Dropdown* dd) {
    Vector2 parent_pos = {0};
    if (dd->parent != NULL) {
        Container* cnt = dd->parent;
        parent_pos.x = cnt->pos.x;
        parent_pos.y = cnt->pos.y;
    }

    Vector2 pos = Vector2Add(dd->pos, parent_pos);
    Vector2 size = dd->size;

    // draw box
    DrawRectangleLines(pos.x, pos.y, size.x, MG_FONT_SIZE, GRAY);
    Vector2 p1 = {pos.x + size.x - 15, pos.y + 3};
    Vector2 p2 = {pos.x + size.x - 10, pos.y + 13};
    Vector2 p3 = {pos.x + size.x - 5, pos.y + 3};
    DrawTriangle(p1, p2, p3, DARKGRAY);

    // TODO(Andre): dropdown list must be closed when clicked outside and
    // the selected item should not be changed (if any) when this happens
    bool box_hovered = is_hovered(pos, (Vector2){size.x, MG_FONT_SIZE});
    if (box_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        dd->active = true;
        dd->selected_item = false;
    }

    if (dd->active) {
        for (int i=0; i < dd->items.len; ++i) {
            int x = pos.x;
            int y = pos.y + (i+1) * MG_FONT_SIZE;
            int sx = size.x;
            int sy = MG_FONT_SIZE-1;
            bool hovered = is_hovered((Vector2){x, y}, (Vector2){sx, sy});	
            if (hovered) DrawRectangle(x, y, sx, sy+2, GRAY);

            DrawRectangleLines(x, y, sx, sy+2, GRAY);
            DrawText(dd->items.items[i], x + 5, y, MG_FONT_SIZE, dd->text_color);
            if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                dd->selected_item = true;
                dd->active = false;
                dd->selected_index = i;
                dd->selected_text = dd->items.items[i];
            }
        }
    } else {
        if (dd->selected_item) {
            DrawText(dd->items.items[dd->selected_index], pos.x + 5, pos.y, MG_FONT_SIZE, dd->text_color);
        }
    }
}

void mg_textbox(Textbox* tb) {
    Vector2 parent_pos = {0};
    if (tb->parent != NULL) {
        Container* cnt = tb->parent;
        parent_pos.x = cnt->pos.x;
        parent_pos.y = cnt->pos.y;
    }

    int txt_size = MeasureText(tb->text, MG_FONT_SIZE);
    int folga = 15;
    bool big = (txt_size - tb->size.x + folga) >= 0; // text is bigger than the length of the textbox
    if (big) {
    }
    float tbposx = tb->pos.x;
    float tbposy = tb->pos.y;
    DrawRectangleLines(tbposx + parent_pos.x, tbposy + parent_pos.y, tb->size.x, MG_FONT_SIZE, tb->active ? BLUE : RED);
    DrawText(tb->text, tbposx + parent_pos.x + 5, tbposy + parent_pos.y, MG_FONT_SIZE, tb->text_color);

    bool hovered = is_hovered((Vector2){tbposx + parent_pos.x, tbposy + parent_pos.y}, (Vector2){tb->size.x, MG_FONT_SIZE});
    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        tb->active = true;
    }

    if (tb->active) {
        // blinking cursor
        bool blinktime = ((int)(GetTime() * 5) % 2) > 0;
        DrawLine(tbposx + parent_pos.x + txt_size + 5+1, tbposy + parent_pos.y + 2, tbposx + parent_pos.x + txt_size + 5+1, tbposy + parent_pos.y + MG_FONT_SIZE-2, blinktime ? BLACK : MG_BACKGROUND_COLOR);

        int k = GetKeyPressed();
        int c = GetCharPressed();
        if (k == KEY_ENTER) {
            tb->active = false;
        }
        else if (k == KEY_BACKSPACE) {
            tb->text[strlen(tb->text)-1] = '\0';
        }
        else if (32 <= c && c <= 126) {
            tb->text[strlen(tb->text)] = c;
            tb->text[strlen(tb->text)+1] = '\0';
        }
    }
}




#endif // MYGUI_IMPLEMENTATION

#endif // MYGUI_H

