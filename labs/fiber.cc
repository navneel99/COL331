#include "labs/fiber.h"

void fibprime(addr_t* pmain_stack,addr_t* pf_stack,bool* pret,bool* isDone,int* fibnum,preempt_t* ppreempt){
  addr_t& main_stack=*pmain_stack;
  addr_t& f_stack=*pf_stack;
  bool& ret = *pret;
  bool& done = *isDone;
  int& num = *fibnum;
  preempt_t& preempt = *ppreempt;
  int i;
  ret=true;

  for(i=2;i<num;i++){
    hoh_debug("i is "<<i);
    if(num%i==0){
      done=true;
      ret=false;
      break;
    }else{
      done=false;
      preempt.doing_yield=0;
      preempt.from_preempt=0;
      stack_saverestore(f_stack,main_stack);
    }
  }
  hoh_debug("WAA");
  for(;;){
  hoh_debug("here.");
  done=true;
  preempt.doing_yield=0;
  preempt.from_preempt=0;
  stack_saverestore(f_stack,main_stack);

  }
}


void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, preempt_t& preempt,addr_t& f_stack, addr_t f_array, uint32_t f_arraysize, dev_lapic_t& lapic){
  if (shellstate.fiber_state==0){
  }else if(shellstate.fiber_state==1){
    stack_init6(f_stack,f_array,f_arraysize,&fibprime,&main_stack,&f_stack,&shellstate.fiber_ret,&shellstate.fiber_done,&shellstate.fiber_num,&preempt);
    shellstate.retFromPreempt=0;
    shellstate.fiber_state++;
    preempt.doing_yield=1;
  }else{
    if (shellstate.retFromPreempt==0){
      // shellstate.retFromPreempt=true;
      lapic.reset_timer_count(10000000);
      stack_saverestore(main_stack,f_stack);
    }else if(shellstate.retFromPreempt!=0){
      lapic.reset_timer_count(10000000);
      stack_saverestore(main_stack,preempt.saved_stack);
    }
    preempt.doing_yield=1;
    shellstate.retFromPreempt = preempt.from_preempt;
    hoh_debug("Preempt returnFromPreempt value"<<preempt.from_preempt<<" "<<shellstate.retFromPreempt);
    // fibprime(&main_stack,&f_stack,&shellstate.fiber_ret,&shellstate.fiber_done,&shellstate.fiber_num);
    if (shellstate.fiber_done==true){
      shellstate.fiber_state=0;
      // shellstate.onlyOnce=true;
      shellstate.fiber_done=false;
      bool temp = shellstate.fiber_ret;
      const char* ans;
      if (temp){
        ans = "Fiber Result: It is prime.";
      }else{
        ans = "Fiber Result: It is not prime";
      }
      shellstate.buffer_end--;
      addToBuffer(shellstate,ans);

      shellstate.buffer[shellstate.buffer_end++]='\n';
      shellstate.buffer[shellstate.buffer_end++]='$';
    }
  }


  //insert your code here

}

