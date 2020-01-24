#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{
    char* keymap;
    int kp = 0;
    char comm_buffer[1024];
    char buffer[1024];
    char newkey;
    int buffer_end;
    int comm_buffer_end = 0;
    bool to_clear = false;
};

struct renderstate_t{
    char buffer[1024];
    int kp = 0;
    char comm_buffer[1024];
    char newkey;
    int buffer_end;
    int comm_buffer_end;
    bool to_clear = false;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

