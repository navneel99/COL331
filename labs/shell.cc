#include "labs/shell.h"
#include "labs/vgatext.h"


static void getResult(shellstate_t &state);

// bool isEqual(char a[] , int a1, char b[]){
//   bool started = false,check = true;
//   int ind = 0;
//   for (int j = 0 ; j < a1; j++){
//     if (a[j] == ' ' and started == false){
//       ind = 0;
//       check  = false;
//       continue;
//     }else if (a[j] == b[ind]){
//       ind++;
//       started  = true;
//       check = true;
//     }else if (a[j] != b[ind]){
//       check = false;
//       break;
//     }
//   }
//   return check;
// }

bool isEqual(char a[] , int a1, char b[], int b1){
  bool started = false,check = true;
  int ind = 0;
  for (int j = 0 ; j < b1; j++){
    if (a[j] == b[j]){
      continue;
    }else{
      check = false;
      break;
    }
  }
  return check;
}

//
// initialize shellstate
//
void shell_init(shellstate_t& state){
  state.keymap="``1234567890-=+`qwertyuiop[]?`asdfghjkl;'\\``zxcvbnm,./``` ";
  state.buffer[0]='$';
  state.buffer_end = 1;
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
  stateinout.to_clear = false;
  if (stateinout.newkey == '?'){ //Enter pressed
    for ( int i = 0; i< stateinout.comm_buffer_end; i++){
      stateinout.buffer[stateinout.buffer_end] = stateinout.comm_buffer[i];
      stateinout.buffer_end++;
    }
    getResult(stateinout); //adds the answer to the comm_buffer
    if (stateinout.to_clear == true){
      stateinout.buffer_end = 0;
      stateinout.comm_buffer_end = 0;
      // stateinout.buffer[0] = '$';
    }else{
    stateinout.buffer[stateinout.buffer_end] = '$';
    stateinout.buffer_end+=1;
    stateinout.comm_buffer_end = 0;
    stateinout.newkey=' '; //change this value
    }
  }
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){
  render.kp = shell.kp;
  render.buffer_end = shell.buffer_end;
  render.comm_buffer_end = shell.comm_buffer_end;
  render.to_clear = shell.to_clear;
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
  
  if (a.kp == 0 || b.kp == 0 || a.buffer_end==0){
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
  if (state.to_clear == true){
    fillrect(0,0,80,25,0,2,w,h,vgatext_base);
  }
  drawnumberindec(76,0, state.kp, 8, 0, 2, w, h, vgatext_base);
  renderShell(state,w,h,vgatext_base);
}


int char2int(char num[], int l ){
  int e= 1;
  for(int j = 0; j<l-1;j++){
    e*=10;
  }
  int n = 0;
  for(int j = 0; j < l; j++){
    n+= (( (int)num[j] - '0') * e);
    e/=10;

  }

  return n;
}

int fibo(int a){
  int m = m>2?m:2;
  int t[3];
  t[0] = 0;
  t[1] = 1;
  t[2] = 1;
  if (a<3){
    return t[a];
  }
  for (int k = 3; k<=a;k++){
    int tmp = t[1]+t[2];
    t[0] = t[1];
    t[1] = t[2];
    t[2] = tmp;
    // t[k] = t[k-1]+t[k-2];
  }
  return t[2];
}

bool prime(int n){
  
  for (int j = 2; j <n;j++){
    if (n%j == 0){
      return false;
    }
  }
  return true;
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

static int whichFunction(char *line, int length){
  char all_fun[6][10] = { "echo","fib","prime","fake","help","clear" }; //all possible functions
  int num_fun = 6;
  // hoh_debug("Currently in whichFunction");
  for (int i =0; i<num_fun;i++){
    char *c_f = all_fun[i];
    int l=0;
    char* name="";
    for(; c_f[l] !='\0';){
      l++;
    }
    if (isEqual(line,length,c_f,l) == true){
      // hoh_debug("isEqual function returns");
      // hoh_debug(i);
      return i;
    }
  }

  return -1;
}

static void extractFunctions(char *command,int command_end, int *fType, char arguments[3][80]){
  char line[80]="";
  int line_end=0;
  bool start_flag = false; //has the program non empty input?
  bool func_flag=false; // is what we just read a function?
  bool arg_flag=false;
  int arg_num=0;
  char curr_arg[80]="";
  int arg_l=0;

  for (int i=0;i<command_end;i++){
    if (command[i] != ' ' && command[i]!='?'){
      if (func_flag == false && start_flag==true){
        arg_flag=true;
      }
      if (arg_flag){
        curr_arg[arg_l++]=command[i];
        curr_arg[arg_l]='\0';
      }else{
        line[line_end++]=command[i];
      }
      if (start_flag == false){
        func_flag = true; //only for the first time;
      }
      start_flag=true;

    }else if(command[i] == ' '){
      if (start_flag){
        if (func_flag){
          *fType = whichFunction(line,line_end);
          func_flag = false;
          line_end=0;
        }else{
          arguments[arg_num][arg_l]='\0';
          for(int j=0;j<arg_l;j++){
          arguments[arg_num][j] = curr_arg[j];
          }
          arg_num++;
          arg_l=0;
          curr_arg[arg_l]='\0';
        }
      }
    }else if(command[i] =='?'){
      if (func_flag){
          *fType = whichFunction(line,line_end);
      }
      if (arg_flag && arg_l !=0){
        for(int j=0;j<arg_l;j++){
          arguments[arg_num][j] = curr_arg[j];
        }
        arg_num=0;
        curr_arg[0]='\0';
        arg_l=0;
      }
      break;
    }
  }
}

static void getResult(shellstate_t &state){
  char line[80]="";
  int line_end = 0;
  bool ok = false;
  int *fun;
  char args[3][80]={"","",""};
  extractFunctions(state.comm_buffer,state.comm_buffer_end,fun,args);
  // hoh_debug("On Enter Press");
  // hoh_debug(*fun);
  // hoh_debug(args[0]);
  // hoh_debug(args[1]);
  // hoh_debug("Function over.");
  for ( int i = 0; i < state.comm_buffer_end; i++){
    if (state.comm_buffer[i] == ' '){
        if (isEqual(line,line_end,"echo",4)){
          bool s = false;
          for(int j = i ; j < state.comm_buffer_end; j++){
            if (state.comm_buffer[j] ==' ' && s == false){
              continue;
            }else{
              s = true;
              state.buffer[state.buffer_end] = state.comm_buffer[j];
              state.buffer_end++;
            }
          }
          // hoh_debug("echo");
          break;
        }else if (isEqual(line, line_end,"fib",3)){
          ok = true;
          char num[10]; 
          int n = 0;
          bool s = false;
          for ( int j = i+1; j<state.comm_buffer_end; j++){
            if (state.comm_buffer[j] != ' ' && state.comm_buffer[j] != '?'){
              s = true;
              num[n] = state.comm_buffer[j];
              n++;
            }else if (s == true){
              break;
            }else{
              continue;
            }
          }
          int ans = fibo(char2int(num,n));
          int e = 1;
          int ans2 = ans;
          while(ans2!=0){
            e*=10;
            ans2/=10;
          }
          while(ans !=0){
            state.buffer[state.buffer_end] = hex2char(ans/e);
            state.buffer_end++;
            ans%=e;
            e/=10;
          }
        }else if (isEqual(line, line_end, "prime",5 )){
          ok = true;
          char num[20]; 
          int n = 0;
          bool s = false;
          for ( int j = i+1; j<state.comm_buffer_end; j++){
            if (state.comm_buffer[j] != ' ' && state.comm_buffer[j] != '?'){
              s = true;
              num[n] = state.comm_buffer[j];
              n++;
            }else if (s == true){
              break;
            }else{
              continue;
            }
          }
          bool ans = prime(char2int(num,n));
          if (ans){
            char pr[13] = "It is prime.";
            for (int j = 0; j < 13; j++){
              state.buffer[state.buffer_end] = pr[j];
              state.buffer_end++;
            }
          }else{
            char pr[10] = "Not Prime";
            for (int j = 0; j < 10; j++){
              state.buffer[state.buffer_end] = pr[j];
              state.buffer_end++;
            }

          }

        }/*else if (isEqual(line,line_end,"fake",4)){
          bool start_flag = false;
          for(int j = i+1; j< state.comm_buffer_end;j++){
            if (state.comm_buffer[j] != ' ')
          }
        }*/
      }else if(isEqual(line,line_end,"clear",5)){
        ok = true;
        state.to_clear = true;
        break;
      }else if (isEqual(line,line_end, "help",4)){
            char h_s[]="Welcome to the help section?help to get the help section?echo [string]  to echo the string?fib [num] gets fibonacci for num less than 25?prime [num] to check if a number is prime?clear to clear the screen?";
            char* p;
            for (p = h_s; *p !='\0';p++){
              state.buffer[state.buffer_end++] = *p;
              // state.buffer_end++;
            }
            break;
      }else{
        line[i] = state.comm_buffer[i];
      }
  }
  if (ok){
    state.buffer[state.buffer_end] = '?';
    state.buffer_end++;
  }  
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
