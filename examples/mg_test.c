#define MYGUI_IMPLEMENTATION
#include "mygui.h"

int main() {
  Dropdown dd = {0};
  dd.pos = (Vector2){10, 50};
  dd.size = (Vector2){100, 0};
  dd.items = string_split(string_new("primeiro|segundo"), '|');

  Textbox tb = {0};
  tb.pos = (Vector2){10, 100};
  tb.size = (Vector2){100, 0};
  tb.text = string_new("Um texto");

  InitWindow(800, 600, "Test application of mygui.h");
  while (!WindowShouldClose())
  {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    if (mg_button((Vector2){10, 10}, "Ok")) {
      TraceLog(LOG_INFO, "Button pressed!");
    }
    
    mg_dropdown(&dd);
    mg_textbox(&tb);
    EndDrawing();
  }
  stringlist_free(&dd.items);
  string_free(&tb.text);
  return 0;
}
