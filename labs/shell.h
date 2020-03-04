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
    int fiber_done;
    int fiber_num;
    int fiber_ret;

    //Stuff for the fiber_scheduler

    int curr_fiber = 0; //fiber which is currently executing.
    //Fibonacci uses 2 arguments + 1 return value , The Number,isDone? and ret
    //Factorial uses 2 arguments a+ 1 return value, Number, is done and ret
    //One more to specify the actual function being called
    int arg_ret_list[4 * 5]; //(4* i)th index for ith fiber
    //int exec_q[5]; //list containing which fiber to execute next.
    //int free_q[5]; //queue telling which'th fiber to allocate next
    int fiber_states[5]={0,0,0,0,0}; //list with index i telling th state of fiber i
    //Allowed states 0: DEAD 1: READY 2:RUNNING



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

void int2char(int a,char* ret,int l);
void addToBuffer(shellstate_t&state,const char* str);


