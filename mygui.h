#ifndef MYGUI_H
#define MYGUI_H

#include "raylib.h"

#define STRING_BUILDER_IMPLEMENTATION
#include "string_builder.h"

#define DEFAULT_FOREGROUND_COLOR LIGHTGRAY
#define DEFAULT_BACKGROUND_COLOR (Color){240, 240, 240, 255}
#define DEFAULT_TEXT_COLOR       (Color){20,   20,  20, 255}
#define FONT_SIZE 15

typedef struct IVector2 {
    int x;
    int y;
} IVector2;

typedef struct {
  Vector2 pos;
  Vector2 size;
  bool active;
  int selected_index;
  bool selected_item;
  String selected_text;
  StringList items;
} Dropdown;

typedef struct {
  Vector2 pos;
  Vector2 size;
  bool active;
  int extra_chars;
  String text;
} Textbox;

typedef struct {
  Vector2 pos;
  Vector2 size;
  bool visible;
} Container;

typedef struct {
  Vector2 pos;
  Vector2 padding;
  bool checked;
} Checkbox;

bool mg_button(Vector2 pos, char* text);
void mg_dropdown(Dropdown* dd);
void mg_textbox(Textbox* tb);
void mg_container(Container* cc, const char* title);
void mg_checkbox(Checkbox* chk, const char* text);

#endif // MYGUI_H


///////////////////////////////////////////////////
#ifdef MYGUI_IMPLEMENTATION

bool is_hovered(Vector2 thing_pos, Vector2 thing_size) {
  Vector2 mouse_pos = GetMousePosition();
  return mouse_pos.x >= thing_pos.x && mouse_pos.x <= thing_pos.x+thing_size.x && mouse_pos.y >= thing_pos.y && mouse_pos.y <= thing_pos.y+thing_size.y;
}

bool mg_button(Vector2 pos, char* text) {
  int font_size = 15;
  float padding = font_size + 1.1;
  Vector2 size = {
    MeasureText(text, font_size) + padding,
        (15 + padding)
  };

  bool hovered = is_hovered(pos, size);
  DrawRectangleV(pos, size, hovered ? GRAY : DEFAULT_FOREGROUND_COLOR);

  int x = pos.x + padding / 2;
  int y = pos.y + padding / 2;
  DrawText(text, x, y, FONT_SIZE, DEFAULT_TEXT_COLOR);
  if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) return true;
  return false;
}

void mg_container(Container* cc, const char* title) {
    int textSize = 20;
    int titleHeight = 20;
    if (cc->visible) {
        DrawRectangleV(cc->pos, cc->size, DEFAULT_BACKGROUND_COLOR);

        // title bar
        DrawRectangle(cc->pos.x, cc->pos.y, cc->size.x, titleHeight, RED);
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
    int font_size = 5;
    Vector2 size = {10, 10};

    int x = chk->pos.x + chk->padding.x;
    int y = chk->pos.y + chk->padding.y;

    bool hovered = is_hovered((Vector2){x, y}, size);

    if (chk->checked) DrawRectangle(x, y, size.x, size.y, GREEN);
    DrawRectangleLines(x, y, size.x, size.y, hovered ? BLUE : BLACK);
    DrawText(text, x + size.x + 5, y, font_size, DEFAULT_TEXT_COLOR);
    if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) chk->checked = !chk->checked;
}

void mg_dropdown(Dropdown* dd) {
    Vector2 pos = dd->pos;
    Vector2 size = dd->size;

    // draw box
    DrawRectangleLines(pos.x, pos.y, size.x, FONT_SIZE, GRAY);
    Vector2 p1 = {dd->pos.x + dd->size.x - 15, dd->pos.y + 3};
    Vector2 p2 = {dd->pos.x + dd->size.x - 10, dd->pos.y + 13};
    Vector2 p3 = {dd->pos.x + dd->size.x - 5, dd->pos.y + 3};
    DrawTriangle(p1, p2, p3, DARKGRAY);

    // TODO(Andre): dropdown list must be closed when clicked outside and
    // the selected item should not be changed (if any) when this happens
    bool box_hovered = is_hovered(pos, (Vector2){size.x, FONT_SIZE});
    if (box_hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        dd->active = true;
        dd->selected_item = false;
    }

    if (dd->active) {
        for (int i=0; i < dd->items.size; ++i) {
            int x = pos.x;
            int y = pos.y + (i+1) * FONT_SIZE;
            int sx = size.x;
            int sy = FONT_SIZE-1;
            bool hovered = is_hovered((Vector2){x, y}, (Vector2){sx, sy});	
            if (hovered) DrawRectangle(x, y, sx, sy+2, GRAY);

            DrawRectangleLines(x, y, sx, sy+2, GRAY);
            DrawText(dd->items.items[i].content, x + 5, y, FONT_SIZE, DEFAULT_TEXT_COLOR);
            if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
                dd->selected_item = true;
                dd->active = false;
                dd->selected_index = i;
                dd->selected_text = dd->items.items[i];
            }
        }
    } else {
        if (dd->selected_item) {
            DrawText(dd->items.items[dd->selected_index].content, pos.x + 5, pos.y, FONT_SIZE, DEFAULT_TEXT_COLOR);
        }
    }
}

// TODO(Andre): consertar parte do código que oculta o texto quando ele passa do tamanho da caixa de texto
// TODO(Andre): construir cursor e fazer andar no texto, apagar e adicionar conteúdo.
void mg_textbox(Textbox* tb) {
  int txt_size = MeasureText(tb->text.content, FONT_SIZE);
  int folga = 15;
  bool big = (txt_size - tb->size.x + folga) >= 0;
  if (big) {
  }
  DrawRectangleLines(tb->pos.x, tb->pos.y, tb->size.x, FONT_SIZE, tb->active ? BLUE : GRAY);
  DrawText(tb->text.content, tb->pos.x + 5, tb->pos.y, FONT_SIZE, DEFAULT_TEXT_COLOR);

  bool hovered = is_hovered(tb->pos, tb->size);
  if (hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
    tb->active = true;
  }
  
  if (tb->active) {
    // blinking cursor
    bool blinktime = ((int)(GetTime() * 5) % 2) > 0;
    printf("%d/n", blinktime);
    DrawLine(tb->pos.x + txt_size + 5+1, tb->pos.y+2, tb->pos.x + txt_size + 5+1, tb->pos.y + FONT_SIZE-2, blinktime ? BLACK : DEFAULT_BACKGROUND_COLOR);

    int k = GetKeyPressed();
    if (k == KEY_ENTER)
    {
      tb->active = false;
    }
    else if (k == KEY_BACKSPACE && tb->text.length > 0)
    {
      tb->text = string_substring(tb->text, 0, tb->text.length - 1);
      if (big && tb->extra_chars > 0) tb->extra_chars -=1;
    }
  }
}




#endif // MYGUI_IMPLEMENTATION
