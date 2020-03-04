#include "labs/shell.h"
#include "labs/vgatext.h"


static void getResult(shellstate_t &state);
// void int2char(int a,char* ret,int l);

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

bool isEqual(char a[] , int a1, char b[], int b1){ //b1 is the minimum length of the gold value(b)
  bool started = false,check = true;
  int ind = 0;
  if (b1 < a1){
    return false;
  }
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

void addToBuffer(shellstate_t&state,const char* str){
  int l=0;
  for(;str[l]!=0;){
    state.buffer[state.buffer_end++]=str[l];
    l++;
  }
}

//
// initialize shellstate
//

void keyMapper(uint8_t scankey,char &k){
  if (unsigned(scankey) == 0x01){
    k = 'E';
  }else if (unsigned(scankey) == 0x02){
    k = '1';
  }else if (unsigned(scankey) == 0x03){
    k = '2';
  }else if (unsigned(scankey) == 0x04){
    k = '3';
  }else if (unsigned(scankey) == 0x05){
    k = '4';
  }else if (unsigned(scankey) == 0x06){
    k = '5';
  }else if (unsigned(scankey) == 0x07){
    k = '6';
  }else if (unsigned(scankey) == 0x08){
    k = '7';
  }else if (unsigned(scankey) == 0x09){
    k = '8';
  }else if (unsigned(scankey) == 0xa){
    k = '9';
  }else if (unsigned(scankey) == 0xb){
    k = '0';
  }else if (unsigned(scankey) == 0xc){
    k = '-';
  }else if (unsigned(scankey) == 0xd){
    k = '=';
  }else if (unsigned(scankey) == 0xe){
    k = 'B';
  }else if (unsigned(scankey) == 0xf){
    k = 'T';
  }else if (unsigned(scankey) == 0x10){
    k = 'q';
  }else if (unsigned(scankey) == 0x11){
    k = 'w';
  }else if (unsigned(scankey) == 0x12){
    k = 'e';
  }else if (unsigned(scankey) == 0x13){
    k = 'r';
  }else if (unsigned(scankey) == 0x14){
    k = 't';
  }else if (unsigned(scankey) == 0x15){
    k = 'y';
  }else if (unsigned(scankey) == 0x16){
    k = 'u';
  }else if (unsigned(scankey) == 0x17){
    k = 'i';
  }else if (unsigned(scankey) == 0x18){
    k = 'o';
  }else if (unsigned(scankey) == 0x19){
    k = 'p';
  }else if (unsigned(scankey) == 0x1a){
    k = '[';
  }else if (unsigned(scankey) == 0x1b){
    k = ']';
  }else if (unsigned(scankey) == 0x1c){
    k = '\n';
  }else if (unsigned(scankey) == 0x1d){
    k = 'C';
  }else if (unsigned(scankey) == 0x1e){
    k = 'a';
  }else if (unsigned(scankey) == 0x1f){
    k = 's';
  }else if (unsigned(scankey) == 0x20){
    k = 'd';
  }else if (unsigned(scankey) == 0x21){
    k = 'f';
  }else if (unsigned(scankey) == 0x22){
    k = 'g';
  }else if (unsigned(scankey) == 0x23){
    k = 'h';
  }else if (unsigned(scankey) == 0x24){
    k = 'j';
  }else if (unsigned(scankey) == 0x25){
    k = 'k';
  }else if (unsigned(scankey) == 0x26){
    k = 'l';
  }else if (unsigned(scankey) == 0x27){
    k = ';';
  }else if (unsigned(scankey) == 0x28){
    k = '\'';
  }else if (unsigned(scankey) == 0x29){
    k = ' ';
  }else if (unsigned(scankey) == 0x2a){
    k = '>';
  }else if (unsigned(scankey) == 0x2b){
    k = '\\';
  }else if (unsigned(scankey) == 0x2c){
    k = 'z';
  }else if (unsigned(scankey) == 0x2d){
    k = 'x';
  }else if (unsigned(scankey) == 0x2e){
    k = 'c';
  }else if (unsigned(scankey) == 0x2f){
    k = 'v';
  }else if (unsigned(scankey) == 0x30){
    k = 'b';
  }else if (unsigned(scankey) == 0x31){
    k = 'n';
  }else if (unsigned(scankey) == 0x32){
    k = 'm';
  }else if (unsigned(scankey) == 0x33){
    k = ',';
  }else if (unsigned(scankey) == 0x34){
    k = '.';
  }else if (unsigned(scankey) == 0x35){
    k = '/';
  }else if (unsigned(scankey) == 0x36){
    k = '<';
  // }else if (unsigned(scankey) == 0x37){
  //   k = "";
  }else if (unsigned(scankey) == 0x38){
    k = 'A';
  }else if (unsigned(scankey) == 0x39){
    k = ' '; //space
  }else if (unsigned(scankey) == 0x4b){
    k = 'L';
  }else if (unsigned(scankey) == 0x48){
    k = 'U';
  }else if (unsigned(scankey) == 0x50){
    k = 'D';
  }else if (unsigned(scankey) == 0x4d){
    k = 'R';
  }
}


void shell_init(shellstate_t& state){
  // state.keymap="``1234567890-=+`qwertyuiop[]?`asdfghjkl;'\\``zxcvbnm,./``` ";
  state.buffer[0]='$';
  state.buffer_end = 1;
  state.cursor_time =1;
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
    // char k = (stateinout).keymap[unsigned(scankey)];
    char k;
    keyMapper(scankey,k);
    stateinout.newkey = k;
    if (k != 'B'){
      stateinout.comm_buffer[stateinout.comm_buffer_end++] = k;
      stateinout.comm_buffer[stateinout.comm_buffer_end] ='\0';
    }else{
      if (stateinout.comm_buffer_end>0){
      stateinout.comm_buffer[--stateinout.comm_buffer_end]='\0';
      }else{
      stateinout.comm_buffer[stateinout.comm_buffer_end]='\0';
      }
    }
    // hoh_debug(stateinout.comm_buffer);
    stateinout.kp++;
}





//
// do computation
//
void shell_step(shellstate_t& stateinout){

  if (stateinout.newkey == '\n'){ //Enter pressed
    for ( int i = 0; i< stateinout.comm_buffer_end; i++){
      stateinout.buffer[stateinout.buffer_end] = stateinout.comm_buffer[i];
      stateinout.buffer_end++;
    }
    getResult(stateinout); //adds the answer to the comm_buffer
    if (stateinout.to_clear == true){
      stateinout.buffer_end = 0;
      stateinout.comm_buffer_end = 0;
      stateinout.comm_buffer[0]='\0';
      // stateinout.buffer[0] = '$';
    }else{
    stateinout.buffer[stateinout.buffer_end++] = '$';
    // stateinout.buffer_end+=1;
    stateinout.comm_buffer_end = 0;
    stateinout.comm_buffer[0]='\0';
    stateinout.newkey=' '; //change this value
    }
  }
  int t = 75000;
  if (stateinout.cursor_time<t){
    stateinout.show_cursor = true;
  }else{
    stateinout.show_cursor = false;
  }
  stateinout.cursor_time = (stateinout.cursor_time+1)%(2*t);
}


//
// shellstate --> renderstate
//
void shell_render(const shellstate_t& shell, renderstate_t& render){
  //Taking variables from shell to render
  render.kp = shell.kp;
  render.buffer_end = shell.buffer_end;
  render.comm_buffer_end = shell.comm_buffer_end;
  render.to_clear = shell.to_clear;
  render.coroutine_state = shell.coroutine_state;
  render.fiber_state = shell.fiber_state;
  render.newkey = shell.newkey;

  render.show_cursor = shell.show_cursor;

  //Copying buffers from shell to render
  for (int i = 0; i < shell.buffer_end; i++){
    render.buffer[i] = shell.buffer[i];
  }
  for(int i  = 0; i < shell.comm_buffer_end; i++){
    render.comm_buffer[i] = shell.comm_buffer[i];
  }
  // hoh_debug(render.show_cursor);
  // if (render.show_cursor == true){
  //   render.show_cursor = false;
  // }else{
  //   render.show_cursor = true;
  // }

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

  if (a.show_cursor != b.show_cursor){
    return false;
  }

  if (a.kp == 0 || b.kp == 0 || a.buffer_end==0 || a.coroutine_state != b.coroutine_state || a.fiber_state !=b.fiber_state){
    return false;
  }
  if (a.comm_buffer_end != b.comm_buffer_end){
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

void int2char(int a,char ret[], int l){
  // int e = 1,buff_end=0;
  ret[l]='\0';
  l--;
  while (a !=0){
    int c = a%10; //last digit
    ret[l] = (char) (c+'0');
    a=a/10;
    l--;
  }
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
    if (ch == '\n'){
      writecharxy(x,y,' ',0,2,w,h,vgatext_base);
      y++;
      x=0;
      writecharxy(x,y,' ',0,2,w,h,vgatext_base);
    }else{
      writecharxy(x,y,ch,0,2,w,h,vgatext_base);
      x++;
    }
  }
  for(int i=0; i < state.comm_buffer_end; i++){
    char ch = state.comm_buffer[i];
    writecharxy(x,y,ch,0,2,w,h,vgatext_base);
    x++;
  }
  if (state.newkey == 'B'){ //Backspace
    writecharxy(x,y,' ',0,2,w,h,vgatext_base);
    writecharxy(x+1,y,' ',0,2,w,h,vgatext_base);
  }
  if(state.show_cursor){
    writecharxy(x,y,'_',0,3,w,h,vgatext_base);
  }else{
    writecharxy(x,y,' ',0,3,w,h,vgatext_base);
  }
}

static int whichFunction(char *line, int length){
  char all_fun[10][10] = { "echo","fib","prime","coprime","help","clear","fibprime","fscfib","fscfact","fscprime"}; //all possible functions
  int num_fun = 10;
  for (int i =0; i<num_fun;i++){
    char *c_f = all_fun[i];
    int l=0;
    // char* name="";
    for(; c_f[l] !='\0';){
      l++;
    }
    if (isEqual(line,length,c_f,l) == true){
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
  *fType=-1;

  for (int i=0;i<command_end;i++){
    if (command[i] != ' ' && command[i]!='\n'){
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
    }else if(command[i] =='\n'){
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

  bool ok = false;
  int *fun;
  char args[3][80]={"","",""};
  extractFunctions(state.comm_buffer,state.comm_buffer_end,fun,args);
  
  if (*fun == 0){
    bool start = false;
    bool check_space = false;
    for(int i=0;i<state.comm_buffer_end;i++){
      if (state.comm_buffer[i]=='o' && start == false){
        start=true;
        check_space = true;
        continue;
      }
      if (check_space){
        if (state.comm_buffer[i] != ' '){
          check_space = false;
          state.buffer[state.buffer_end++] = state.comm_buffer[i];
        }
      }else if(start==true){
        state.buffer[state.buffer_end++] = state.comm_buffer[i];
      }
    }
  }else if(*fun == 1){
    ok = true;
    char *number = args[0];
    int l = 0;
    for(;number[l]!='\0';l++);
    int ans = fibo(char2int(number,l));
    if (ans == 0){
      state.buffer[state.buffer_end++]='0';
    }else{
      int temp=ans;
      l=0;
      while(temp!=0){
        temp/=10;
        l++;
      }
      char * ans2 ;
      int2char(ans,ans2,l);
      for(int j=0;j<l;j++){
        state.buffer[state.buffer_end++]=ans2[j];
      }
    }

    // int e = 1;
    // int ans2 = ans;
    // while(ans2!=0){
    //   e*=10;
    //   ans2/=10;
    // }
    // // int l=0;

    // while(ans !=0){
    //   state.buffer[state.buffer_end] = hex2char(ans/e);
    //   state.buffer_end++;
    //   ans%=e;
    //   e/=10;
    // }
  }else if (*fun == 2){
    ok = true;
    char* number = args[0];
    int l=0;
    for(;number[l]!='\0';l++);
    bool ans = prime(char2int(number,l));
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
  }else if(*fun == 3){ //coprime function
    state.coroutine_state=1;
    char* a = args[0];
    int l=0;
    for(;a[l] !='\0';l++);
    state.coroutine_arg=char2int(args[0],l);
  }else if(*fun == 4){ //help
    ok=true;
    char h_s[]="Welcome to the help section\nhelp to get the help section\necho [string]  to echo the string\nfib [num] gets fibonacci for num less than 47\nprime [num] to check if a number is prime\nclear to clear the screen\ncoprime [num] to get check if a number is prime using co-routine\nfibprime [num] to check if a number is prime using fibers\nfscfact [num] to get the factorial mod (num+1) of number using fiber scheduler\nfscfib [num] to get the fibonacci of the num'th number < 47 using fiber scheduler\nfscprime to check if a number is prime using fiber scheduler";
    char* p;
    for (p = h_s; *p !='\0';p++){
      state.buffer[state.buffer_end++] = *p;
    }

  }else if(*fun == 5){ //clear
    ok = true;
    state.to_clear = true;
  }else if(*fun == 6){ //fibprime
    hoh_debug("Fib Prime");
    char *a = args[0];
    int l=0;
    for(;a[l] !='\0';l++);
    state.fiber_state=1;
    state.fiber_num=char2int(args[0],l);
  }else if(*fun == 7){ //fscfib
    hoh_debug("fsc fibonacci");
    char *a = args[0];
      int l=0;
      for(;a[l]!='\0';l++);
      int k=-1;
      for (k=0; k<5;k++){
        if (state.fiber_states[k] == 0){
          break;
        }
      }
      hoh_debug("FSCFIB assigned to fiber number");
      hoh_debug(k);
      state.arg_ret_list[k*4] = (*fun); //The function being called
      state.arg_ret_list[(k * 4)+1]=0; //isDone?
      state.arg_ret_list[(k * 4)+2]=char2int(args[0],l); //The number
      state.arg_ret_list[(k * 4)+3]=0; //Return Value
      state.fiber_states[k] = 1; //Set this to READY
  }else if(*fun == 8){ //fscfact
    hoh_debug("fsc factorial");
    char *a = args[0];
      int l=0;
      for(;a[l]!='\0';l++);
      int k=-1;
      for (k=0; k<5;k++){
        if (state.fiber_states[k] == 0){
          break;
        }
      }
      hoh_debug("FSCFACT assigned to fiber number");
      hoh_debug(k);
      state.arg_ret_list[k*4] = (*fun); //The function being called
      state.arg_ret_list[(k * 4)+1]=0; //isDone?
      state.arg_ret_list[(k * 4)+2]=char2int(args[0],l); //The number
      state.arg_ret_list[(k * 4)+3]=0; //Return Value
      state.fiber_states[k] = 1; //Set this to READY
  }else if(*fun== 9){ //fscprime
    hoh_debug("fsc prime");
    char *a = args[0];
      int l=0;
      for(;a[l]!='\0';l++);
      int k=-1;
      for (k=0; k<5;k++){
        if (state.fiber_states[k] == 0){
          break;
        }
      }
      hoh_debug("FSCPRIME assigned to fiber number");
      hoh_debug(k);
      state.arg_ret_list[k*4] = (*fun); //The function being called
      state.arg_ret_list[(k * 4)+1]=0; //isDone?
      state.arg_ret_list[(k * 4)+2]=char2int(args[0],l); //The number
      state.arg_ret_list[(k * 4)+3]=0; //Return Value
      state.fiber_states[k] = 1; //Set this to READY
  }else{
    state.to_clear=false; //VERY IMPORTANT STATEMENT!! Otherwise won't stop clearing
  }

  if (ok){
    state.buffer[state.buffer_end] = '\n';
    state.buffer_end++;
  }
  state.comm_buffer_end=0;
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
