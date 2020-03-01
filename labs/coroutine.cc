#include "labs/coroutine.h"


void coprime(coroutine_t &f_coro,f_t* f_locals,bool* ret, bool *isDone){
    coroutine_t &cor = f_coro;
    bool *done = isDone;
    bool *fret = ret;
    int &i = f_locals -> i;
    int &num = f_locals-> num;
    
    h_begin(f_coro);

    for (i=2;i<num;i++){
        // hoh_debug(i);
        if (num%i==0){
            *fret = false; //Not a prime
            *done = true;
            h_end(cor);
        }else{
            *done = false;
            h_yield(cor);
        }
    }
    // hoh_debug("outside");
    *done = true;
    *fret = true;
    h_end(cor);
}



void shell_step_coroutine(shellstate_t& shellstate, coroutine_t& f_coro, f_t& f_locals){
    if (shellstate.coroutine_state==0){ //dead
    }else if(shellstate.coroutine_state==1){ //start state
        coroutine_reset(f_coro);
        f_locals.num = shellstate.coroutine_arg;
        shellstate.coroutine_state=2;
    }else if(shellstate.coroutine_state == 2){ //ready_state
        bool isDone;
        bool ret;
        coprime(f_coro,&f_locals,&ret,&isDone);
        if (isDone){
            shellstate.coroutine_result=ret;
            shellstate.coroutine_state=0;
            const char*ans;
            int l=0;
            if (ret){
                ans = "Coroutine Result: It is prime";
            }else{
                ans = "Coroutine Result: It is not prime";
            }
            shellstate.buffer_end--; //removing the last $
            while(ans[l] !='\0'){
                shellstate.buffer[shellstate.buffer_end++]=ans[l];
                l++;
            }
            shellstate.buffer[shellstate.buffer_end++]='\n';
            shellstate.buffer[shellstate.buffer_end++]='$';

            // for(;ans[l]!='\0';l++);
        }
        //will call function and check if isDone??
    }

    //insert your code here

}


