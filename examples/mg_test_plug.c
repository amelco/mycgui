#include <stdio.h>
#include <stdlib.h>
#define CSTR_IMPLEMENTATION
#include "cstr.h"
#include "mg_test_plug.h"

void plug_init(Plug* plug) {
    plug->dd.pos = (Vector2){10, 50};
    plug->dd.size = (Vector2){100, 0};
    plug->dd.items = cstr_split("first|second", '|');
    plug->dd.active = false;
    plug->dd.selected_index = 0;
    plug->dd.selected_item = false;
    plug->dd.text_color = RAYWHITE;

    plug->tb.pos = (Vector2){10, 100};
    plug->tb.size = (Vector2){100, 0};
    plug->tb.text = malloc(256);
    sprintf(plug->tb.text, "Hello");
    plug->tb.active = false;
    plug->tb.extra_chars = 0;
    plug->tb.text_color = RAYWHITE;
}

void plug_update(Plug* plug) {
    Color bg = GetColor(0x181818FF);
    ClearBackground(bg);

    if (mg_button((Vector2){10, 10}, "Log text", RAYWHITE)) {
        TraceLog(LOG_INFO, plug->tb.text);
    }

    mg_dropdown(&plug->dd);
    mg_textbox(&plug->tb);
}

void plug_end(Plug* plug) {
    cstrlist_free(plug->dd.items);
}
