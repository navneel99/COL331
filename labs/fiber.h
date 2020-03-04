#pragma once

#include "labs/shell.h"
#include "util/fiber.h"



void shell_step_fiber(shellstate_t& shellstate, addr_t& main_stack, addr_t& f_stack, addr_t f_array, uint32_t f_arraysize);

// void fibprime(addr_t* pmain_stack,addr_t* pf_stack,int* pret,int* isDone,int* fibnum);

