#ifndef SCHEDULER_H
#define SCHEDULER_H

#include "memory.h"
#include "cpu.h"
//#include "scheduler.h"

int schedule(int cycle_num, int process_status);
void next_process();
void new_process(int base, int size);
int ready_q_status();
int get_base();
int get_current_pid();
void remove_process(int pid);
int get_process_count();

#endif

