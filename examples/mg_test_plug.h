#ifndef MG_TEST_PLUG_H
#define MG_TEST_PLUG_H

#define MYGUI_IMPLEMENTATION
#include "mygui.h"

typedef struct {
    Dropdown dd;
    Textbox tb;
} Plug;

typedef void (*plug_init_t)(Plug* plug);
typedef void (*plug_update_t)(Plug* plug );
typedef void (*plug_end_t)(Plug* plug );

#endif //MG_TEST_PLUG_H
