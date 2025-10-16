#define CSTR_IMPLEMENTATION
#include "cstr.h"
#define MYGUI_IMPLEMENTATION
#include "mygui.h"

int main() {
  Dropdown dd = {0};
  dd.pos = (Vector2){10, 50};
  dd.size = (Vector2){100, 0};
  dd.items = cstr_split("primeiro|segundo", '|');

  Textbox tb = {0};
  tb.pos = (Vector2){10, 100};
  tb.size = (Vector2){100, 0};
  tb.text = "Um teaxto";

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
    //if (tb.text != NULL) TraceLog(LOG_INFO, tb.text);

    if (IsKeyPressed(KEY_Q)) return 0;
    EndDrawing();
  }
  cstrlist_free(dd.items);
  return 0;
}
