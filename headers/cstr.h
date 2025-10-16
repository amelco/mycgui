/*
 * C strings helper functions
 */

#ifndef CSTR_H_
#define CSTR_H_

#include <stdlib.h>
#include <string.h>

typedef struct {
    char** items;
    int len;
} CstrList;

CstrList cstr_split(const char* str, const char sep);
void cstrlist_free(CstrList list);

#ifdef CSTR_IMPLEMENTATION

void __add_str_to_list(char** list, int i, int beg, int idx, char* tmp) {
    int n = i - beg + 1;
    list[idx] = malloc(n);
    strncpy(list[idx], tmp, n);
    list[idx][n-1] = '\0';
}

CstrList cstr_split(const char* str, const char sep) {
    char** list = malloc(128 * sizeof(char*)); // TODO: make it dynamic
    int idx = 0;
    
    char tmp[512] = {0};  // TODO: make it dynamic
    int tmp_idx = 0;

    int i=0;
    int beg = 0;
    char c = str[0];
    while (c != '\0') {
        if (c == sep) {
            __add_str_to_list(list, i, beg, idx, tmp);
            idx++;
            beg = i + 1;
            tmp_idx = 0;
            c = str[++i];
            continue;
        }
        tmp[tmp_idx] = c;
        tmp_idx++;

        c = str[++i];
    }
    __add_str_to_list(list, i, beg, idx, tmp);
    
    CstrList l = {
        .items = list,
        .len = idx + 1
    };
    return l;
}

void cstrlist_free(CstrList list) {
    for (int i = 0; i < list.len; ++i) {
        free(list.items[i]);
    }
    free(list.items);
}

#endif //CSTR_IMPLEMENTATION

#endif //CSTR_H_
