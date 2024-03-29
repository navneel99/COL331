#include "labs/fiber_scheduler.h"

void fib_sch_prime(addr_t* pmain_stack,addr_t* pf_stack,int* pret,int* isDone,int* fibnum, preempt_t* ppreempt){
  addr_t& main_stack=*pmain_stack;
  addr_t& f_stack=*pf_stack;
  int& ret = *pret;
  int& done = *isDone;
  int& num = *fibnum;
  preempt_t& preempt = *ppreempt;
  int i;
  // hoh_debug("Before loop");
  // hoh_debug(num);
  ret=1;
  for(i=2;i<num;i++){
    hoh_debug(i);
    if(num%i==0){
      done=1;
      ret=0;
      break;
    }else{
      done=0;
    }
  }

  for(;;){
  done=1;
  hoh_debug("here");
  }
}


void fib_sch_factorial(addr_t* pmain_stack,addr_t* pf_stack,int* pret,int* isDone,int* fibnum, preempt_t* ppreempt){
    // void fib_sch_factorial(addr_t* pmain_stack,){
    addr_t& main_stack=*pmain_stack;
    addr_t& f_stack=*pf_stack;
    int& ret = *pret;
    int& done = *isDone;
    int& num = *fibnum;
    preempt_t& preempt = *ppreempt;
    int i;
    
    ret = 1;
    // hoh_debug("Before loop");
    for (i=1;i<=num;i++){
        ret = (ret*(i%(num+1)))%(num+1);
        done=0;
        preempt.doing_yield=0;
        preempt.from_preempt=0;
        stack_saverestore(f_stack,main_stack);
    }
    done=1;
    preempt.doing_yield=0;
    preempt.from_preempt=0;
    stack_saverestore(f_stack,main_stack);
}


void fib_sch_fibonacci(addr_t* pmain_stack,addr_t* pf_stack,int* pret,int* isDone,int* fibnum, preempt_t* ppreempt){
    addr_t& main_stack=*pmain_stack;
    addr_t& f_stack=*pf_stack;
    int& ret = *pret;
    int& done = *isDone;
    int& num = *fibnum;
    preempt_t& preempt = *ppreempt;
    int i;

    if (num==0){
        done = 1;
        ret = 0;
        preempt.doing_yield=0;
        preempt.from_preempt=0;
        stack_saverestore(f_stack,main_stack);
    }else if (num==2 || num==1){
        done = 1;
        ret = 1;
        preempt.doing_yield=0;
        preempt.from_preempt=0;
        stack_saverestore(f_stack,main_stack);
    }else{
        int n1=1, n2=1;
        for (i=2;i<num;i++){
            int fake=0;
            for(int j=0;j<n2;j++){
                fake++;
            }
            int temp = n1+n2;
            n1 = n2;
            n2 = temp;
            done = false;
            preempt.doing_yield=0;
            preempt.from_preempt=0;
            stack_saverestore(f_stack,main_stack);
        }
        done=true;
        ret = n2;
        preempt.doing_yield=0;
        preempt.from_preempt=0;
        stack_saverestore(f_stack,main_stack);
    }
}


int scheduler_output(shellstate_t& state){
    int c_f = state.curr_fiber;
    for(int i =1;i<=5;i++){
        if (state.fiber_states[(c_f+ i)%5] == 1){
            state.curr_fiber=(c_f+i)%5;
            return (c_f+i)%5;
        }
    }
    for(int i =1;i<=5;i++){
        if (state.fiber_states[(c_f+i)%5] == 2){
            state.curr_fiber=(c_f+i)%5;
            return (c_f+i)%5;
        }
    }
    state.curr_fiber=0;
    return 0; //For the time being
}



//
// stackptrs:      Type: addr_t[stackptrs_size].  array of stack pointers (generalizing: main_stack and f_stack)
// stackptrs_size: number of elements in 'stacks'.
//
// arrays:      Type: uint8_t [arrays_size]. array of memory region for stacks (generalizing: f_array)
// arrays_size: size of 'arrays'. equal to stackptrs_size*STACK_SIZE.
//
// Tip: divide arrays into stackptrs_size parts.
// Tip: you may implement a circular buffer using arrays inside shellstate_t
//      if you choose linked lists, create linked linked using arrays in
//      shellstate_t. (use array indexes as next pointers)
// Note: malloc/new is not available at this point.
//
void shell_step_fiber_scheduler(shellstate_t& shellstate, addr_t& main_stack,preempt_t& preempt,addr_t stackptrs[], size_t stackptrs_size, addr_t arrays, size_t arrays_size,dev_lapic_t& lapic){
    //stackptrs will have the number of stacks possible. Will keep 6. 1 for main_stack and 5 possible function invocations.
    //arrays the actual memory region. Will divide 4096 bytes per process
    
    int sz_fr_fiber = arrays_size/stackptrs_size; //equal to STACK_SIZE
    int fiber_to_run = scheduler_output(shellstate); //outputs which fiber index to process
    if (shellstate.fiber_states[fiber_to_run] == 1){
        if (shellstate.arg_ret_list[4*fiber_to_run] == 8){
            stack_init6(stackptrs[fiber_to_run],&arrays[(fiber_to_run)*sz_fr_fiber],sz_fr_fiber,&fib_sch_factorial,&main_stack,&stackptrs[fiber_to_run],&shellstate.arg_ret_list[(4* fiber_to_run)+3],&shellstate.arg_ret_list[(4* fiber_to_run)+1],&shellstate.arg_ret_list[(4* fiber_to_run)+2],&preempt);
        }else if(shellstate.arg_ret_list[4*fiber_to_run] == 7){
            stack_init6(stackptrs[fiber_to_run],&arrays[(fiber_to_run)*sz_fr_fiber],sz_fr_fiber,&fib_sch_fibonacci,&main_stack,&stackptrs[fiber_to_run],&shellstate.arg_ret_list[(4* fiber_to_run)+3],&shellstate.arg_ret_list[(4* fiber_to_run)+1],&shellstate.arg_ret_list[(4* fiber_to_run)+2],&preempt);
        }else{
            stack_init6(stackptrs[fiber_to_run],&arrays[(fiber_to_run)*sz_fr_fiber],sz_fr_fiber,&fib_sch_prime,&main_stack,&stackptrs[fiber_to_run],&shellstate.arg_ret_list[(4* fiber_to_run)+3],&shellstate.arg_ret_list[(4* fiber_to_run)+1],&shellstate.arg_ret_list[(4* fiber_to_run)+2],&preempt);
        }
        shellstate.fiber_states[fiber_to_run]++;
        shellstate.fsc_preempt_return[fiber_to_run]=0;
        shellstate.fsc_preempt_stored_stack[fiber_to_run]=stackptrs[fiber_to_run];
        hoh_debug("Initialized");
    }else if (shellstate.fiber_states[fiber_to_run] == 2){
        hoh_debug("In Running State.");
        // addr_t to_use;

        preempt.doing_yield=1;
        lapic.reset_timer_count(1000000);
        stack_saverestore(main_stack,shellstate.fsc_preempt_stored_stack[fiber_to_run]);
        // if (shellstate.fsc_preempt_return[fiber_to_run]==0){
        //     lapic.reset_timer_count(1000000);
        //     stack_saverestore(main_stack,stackptrs[fiber_to_run]);
        //     hoh_debug("First Iteration.");
        // }else if (shellstate.fsc_preempt_return[fiber_to_run] !=0){
        //     lapic.reset_timer_count(1000000);
        //     stack_saverestore(main_stack,shellstate.fsc_preempt_stored_stack[fiber_to_run]);
        // }
        preempt.doing_yield=1;
        shellstate.fsc_preempt_return[fiber_to_run] = preempt.from_preempt;
        if (shellstate.fsc_preempt_return[fiber_to_run] == 0){
            shellstate.fsc_preempt_stored_stack[fiber_to_run] = stackptrs[fiber_to_run];
        }else{
            shellstate.fsc_preempt_stored_stack[fiber_to_run]= preempt.saved_stack;
        }
        hoh_debug("From Preempt"<<preempt.from_preempt);
        hoh_debug("wow"<<shellstate.arg_ret_list[4*fiber_to_run + 1]);
        hoh_debug("---");
        // shellstate.fsc_preempt_return[fiber_to_run] = preempt.from_preempt;
        // if (preempt.from_preempt == 1){
        // shellstate.fsc_preempt_stored_stack[fiber_to_run] = preempt.saved_stack;
        // }
        // stack_saverestore(stackptrs[0],stackptrs[fiber_to_run+1]);
        if(shellstate.arg_ret_list[4*(fiber_to_run)+1] == 1){ //If Done
            int ans = shellstate.arg_ret_list[4*(fiber_to_run)+3];
            int org_num = shellstate.arg_ret_list[4*(fiber_to_run)+2];
            hoh_debug("----");
            int temp,l,temp2,l2;
            char *c_ans;
            char * c_num;
            const char *ans_p1, *ans_p2, *f_name;
            ans_p1 = "Fiber Scheduler Result : ";
            ans_p2 = " returned: ";
            l2=0;
            temp2=org_num;
            while(temp2 != 0){
                temp2/=10;
                l2++;
            }
            int2char(org_num,c_num,l2);
            shellstate.buffer_end--;
            if (shellstate.arg_ret_list[4*(fiber_to_run)] == 8){
                f_name="fscfact ";
            }else if (shellstate.arg_ret_list[4*(fiber_to_run)] == 7){
                f_name = "fscfib ";
            }else{
                f_name = "fscprime ";
            }
            addToBuffer(shellstate,ans_p1);
            addToBuffer(shellstate,f_name);
            int tmp=0;
            if (l2==0){
                shellstate.buffer[shellstate.buffer_end++]='0';
            }else{
                for(;c_num[tmp]!='\0';){
                    shellstate.buffer[shellstate.buffer_end++]=c_num[tmp];
                    tmp++;
                }
            }

            addToBuffer(shellstate,ans_p2);

            if (f_name == "fscprime "){
                const char* final_ans;
                if (ans==1){
                    final_ans = "Prime";
                }else{
                    final_ans = "Not Prime";
                }
                tmp=0;
                for(;final_ans[tmp]!='\0';){
                    shellstate.buffer[shellstate.buffer_end++]=final_ans[tmp];
                    tmp++;                    
                }
            }else{
                l=0;
                temp=ans;
                while(temp!=0){
                    temp/=10;
                    l++;
                }
                if (l==0){
                    shellstate.buffer[shellstate.buffer_end++]='0';
                }else{
                    int2char(ans,c_ans,l);
                    hoh_debug("------");
                    hoh_debug(ans);
                    hoh_debug(c_ans);
                    hoh_debug("------");
                    tmp=0;
                    for(;c_ans[tmp]!='\0';){
                        shellstate.buffer[shellstate.buffer_end++]=c_ans[tmp];
                        tmp++;
                    }

                }
            }

            shellstate.buffer[shellstate.buffer_end++]='\n';
            shellstate.buffer[shellstate.buffer_end++]='$';

            //Reset the fiber state and the function
            shellstate.fiber_states[fiber_to_run]=0;
            shellstate.arg_ret_list[4*fiber_to_run]=0;
        }

    }
    //insert your code here

}
