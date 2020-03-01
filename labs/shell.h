#pragma once
#include "util/config.h"
#include "util/debug.h"

struct shellstate_t{
    char* keymap;
    int kp = 0;
    char comm_buffer[1024]; //buffer of the current command
    char buffer[1024]; //buffer of the whole screen
    char newkey; //The new key
    int buffer_end; //The end int of the buffre
    int comm_buffer_end = 0; //The end int of the command
    bool to_clear = false; //flag to clear or not
    bool show_cursor = true;
    int cursor_time;

    //Stuff for coroutine
    int coroutine_state=0; //dead=0,start=1,ready=2,finished=3
    int coroutine_arg=0; //argument for the coprime function
    bool coroutine_result;

    //Stuff for fiber
    int fiber_state=0;//dead=0,start=1,ready=2,finished=3
    bool fiber_done;
    int fiber_num;
    bool fiber_ret;

};

struct renderstate_t{
    char buffer[1024];
    int kp = 0;
    char comm_buffer[1024];
    char newkey;
    int buffer_end;
    int comm_buffer_end;
    bool to_clear = false;
    bool show_cursor;

    //Stuff for coroutine
    int coroutine_state=0; //dead=0,start=1,ready=2,finished=3
    int coroutine_arg=0; //argument for the coprime function
    bool coroutine_result;

    //Stuff for fiber
    int fiber_state=0;//dead=0,start=1,ready=2,finished=3
    bool fiber_done;
    int fiber_num;
    bool fiber_ret;
};

void shell_init(shellstate_t& state);
void shell_update(uint8_t scankey, shellstate_t& stateinout);
void shell_step(shellstate_t& stateinout);
void shell_render(const shellstate_t& shell, renderstate_t& render);

bool render_eq(const renderstate_t& a, const renderstate_t& b);
void render(const renderstate_t& state, int w, int h, addr_t display_base);

