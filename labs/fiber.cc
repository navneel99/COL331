#include "labs/fiber.h"

void fibprime(addr_t* pmain_stack,addr_t* pf_stack,bool* pret,bool* isDone,int* fibnum){
  addr_t& main_stack=*pmain_stack;
  addr_t& f_stack=*pf_stack;
  bool& ret = *pret;
  bool& done = *isDone;
  int& num = *fibnum;
  int i;
  hoh_debug("Before loop");
  hoh_debug(num);
  ret=true;
  for(i=2;i<num;i++){
    hoh_debug(i);
    if(num%i==0){
      done=true;
      ret=false;
      break;
    }else{
      done=false;
      stack_saverestore(f_stack,main_stack);
    }
  }

  // for(;;){
  // if (ret != false){
  //   ret = true;
  // }
  done=true;
  stack_saverestore(f_stack,main_stack);
  // }
}


void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize){
  // hoh_debug("in shell_step_fiber");
  if (shellstate.fiber_state==0){
  }else if(shellstate.fiber_state==1){
    hoh_debug("Initializing...");
    hoh_debug(shellstate.fiber_num);
    stack_init5(f_stack,&f_array,f_arraysize,&fibprime,&main_stack,&f_stack,&shellstate.fiber_ret,&shellstate.fiber_done,&shellstate.fiber_num);
    hoh_debug("Initialized.");
    shellstate.fiber_state++;
  }else{
    hoh_debug("Before Save_Restore");
    stack_saverestore(main_stack,f_stack);
    hoh_debug("After Context Switch");
    if (shellstate.fiber_done==true){
      shellstate.fiber_state=0;
      bool temp = shellstate.fiber_ret;
      char* ans;
      if (temp){
        ans = "Fiber Result: It is prime.";
      }else{
        ans = "Fiber Result: It is not prime";
      }
      int l=0;
      shellstate.buffer_end--;
      for(;ans[l]!='\0';){
        shellstate.buffer[shellstate.buffer_end++]=ans[l];
        l++;
      };
      shellstate.buffer[shellstate.buffer_end++]='?';
      shellstate.buffer[shellstate.buffer_end++]='$';
    }
  }


  //insert your code here

}

