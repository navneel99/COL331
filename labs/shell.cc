#include "labs/shell.h"
#include "labs/vgatext.h"


// static char* append(char a[] ,int al, char* b, int bl){
//   // int a1 = sizeof(a);
//   // int a2 = sizeof(b);
//   for(int i = 0; i< bl; i++){
//     a[al] = b[i];
//     al++;
//   }
//   // a[al] = '\0';
//   return a;
// }
static void getResult(shellstate_t &state);

//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  state.keymap="``1234567890-=+`qwertyuiop[]?`asdfghjkl;'\\``zxcvbnm,./``` ";
  state.comm_buffer[0]='$';
}

//
// handle keyboard event.
// key is in scancode format.
// For ex:
// scancode for following keys are:
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | esc |  1 |  2 |  3 |  4 |  5 |  6 |  7 |  8 |  9 |  0 |  - |  = |back|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 01  | 02 | 03 | 04 | 05 | 06 | 07 | 08 | 09 | 0a | 0b | 0c | 0d | 0e |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     | tab |  q |  w |  e |  r |  t |  y |  u |  i |  o |  p |  [ |  ] |entr|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 0f  | 10 | 11 | 12 | 13 | 14 | 15 | 16 | 17 | 18 | 19 | 1a | 1b | 1c |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | keys     |ctrl |  a |  s |  d |  f |  g |  h |  j |  k |  l |  ; |  ' |    |shft|
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//      | scancode | 1d  | 1e | 1f | 20 | 21 | 22 | 23 | 24 | 25 | 26 | 27 | 28 | 29 | 2a |
//      +----------+-----+----+----+----+----+----+----+----+----+----+----+----+----+----+
//
// so and so..
//
// - restrict yourself to: 0-9, a-z, esc, enter, arrows
// - ignore other keys like shift, control keys
// - only handle the keys which you're interested in
// - for example, you may want to handle up(0x48),down(0x50) arrow keys for menu.
//
void shell_update(uint8_t scankey, shellstate_t& stateinout){
    char k = (stateinout).keymap[unsigned(scankey)];
    stateinout.newkey = k;
    stateinout.comm_buffer[stateinout.comm_buffer_end] = k;
    stateinout.comm_buffer_end = stateinout.comm_buffer_end +1;
    stateinout.kp= stateinout.kp + 1;
}





//
// do computation
//
void shell_step(shellstate_t& stateinout){
  if (stateinout.newkey == '?'){ //Enter pressed
    for ( int i = 0; i< stateinout.comm_buffer_end; i++){
      stateinout.buffer[stateinout.buffer_end] = stateinout.comm_buffer[i];
      stateinout.buffer_end++;
    }
    getResult(stateinout); //adds the answer to the comm_buffer
    stateinout.buffer[stateinout.buffer_end] = '$';
    stateinout.buffer_end+=1;
    stateinout.comm_buffer_end = 0;
    stateinout.newkey=' '; //change this value
  }
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){
  render.kp = shell.kp;
  render.buffer_end = shell.buffer_end;
  render.comm_buffer_end = shell.comm_buffer_end;
  for (int i = 0; i < shell.buffer_end; i++){
    render.buffer[i] = shell.buffer[i];
  }
  for(int i  = 0; i < shell.comm_buffer_end; i++){
    render.comm_buffer[i] = shell.comm_buffer[i];
  }
  

  //
  // renderstate. number of keys pressed = shellstate. number of keys pressed
  //
  // renderstate. menu highlighted = shellstate. menu highlighted
  //
  // renderstate. function result = shellstate. output argument
  //
  // etc.
  //
}


//
// compare a and b
//
bool render_eq(const renderstate_t& a, const renderstate_t& b){
  if (a.kp ==0 || b.kp == 0){
    return false;
  }
  if (a.kp == b.kp){
    return true;
  }
  return false;
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base);
static void renderShell(const renderstate_t &state, int w, int h, addr_t vgatext_base);





//
// Given a render state, we need to write it into vgatext buffer
//
void render(const renderstate_t& state, int w, int h, addr_t vgatext_base){
  //Keeps the Counter
  drawnumberindec(76,0, state.kp, 8, 0, 2, w, h, vgatext_base);

  renderShell(state,w,h,vgatext_base);

}


//
//
// helper functions
//
//

static void writecharxy(int x, int y, uint8_t c, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  vgatext::writechar(y*w+x,c,bg,fg,vgatext_base);
}


static void renderShell(const renderstate_t &state, int w, int h, addr_t vgatext_base){
  int y = 1;
  int x = 0;
  for (int i = 0; i < state.buffer_end; i++){
    char ch = state.buffer[i]; 
    if (ch == '?'){
      y++;
      x=0;
      writecharxy(x,y,' ',0,2,w,h,vgatext_base);
    }else{
      writecharxy(x,y,ch,0,2,w,h,vgatext_base);
      x++;
    }
  }
  for(int i  =0; i < state.comm_buffer_end; i++){
    char ch = state.comm_buffer[i];
    writecharxy(x,y,ch,0,2,w,h,vgatext_base);
    x++;
  }
}

static void getResult(shellstate_t &state){
  state.buffer[state.buffer_end] = '?';
  state.buffer_end++;
}


static void fillrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int y=y0;y<y1;y++){
    for(int x=x0;x<x1;x++){
      writecharxy(x,y,0,bg,fg,w,h,vgatext_base);
    }
  }
}

static void drawrect(int x0, int y0, int x1, int y1, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){

  writecharxy(x0,  y0,  0xc9, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y0,  0xbb, bg,fg, w,h,vgatext_base);
  writecharxy(x0,  y1-1,0xc8, bg,fg, w,h,vgatext_base);
  writecharxy(x1-1,y1-1,0xbc, bg,fg, w,h,vgatext_base);

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y0, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int x=x0+1; x+1 < x1; x++){
    writecharxy(x,y1-1, 0xcd, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x0,y, 0xba, bg,fg, w,h,vgatext_base);
  }

  for(int y=y0+1; y+1 < y1; y++){
    writecharxy(x1-1,y, 0xba, bg,fg, w,h,vgatext_base);
  }
}

static void drawtext(int x,int y, const char* str, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  for(int i=0;i<maxw;i++){
    writecharxy(x+i,y,str[i],bg,fg,w,h,vgatext_base);
    if(!str[i]){
      break;
    }
  }
}

static void drawnumberinhex(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=sizeof(uint32_t)*2+1};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%16);
    number=number/16;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}
static void drawnumberindec(int x,int y, uint32_t number, int maxw, uint8_t bg, uint8_t fg, int w, int h, addr_t vgatext_base){
  enum {max=4};
  char a[max];
  for(int i=0;i<max-1;i++){
    a[max-1-i-1]=hex2char(number%10);
    number=number/10;
  }
  a[max-1]='\0';

  drawtext(x,y,a,maxw,bg,fg,w,h,vgatext_base);
}
