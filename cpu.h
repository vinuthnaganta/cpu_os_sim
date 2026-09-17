#ifndef CPU_H
#define CPU_H

#include "memory.h"

struct reg_struct {
	int Base;
	int PC;
	int IR0;
	int IR1;
	int AC;
	int MAR;
	struct MemCell MBR;
};


void fetch_instruction(int addr);
void execute_instruction();
int mem_address(int l_addr);
int clock_cycle();
struct reg_struct context_switch(struct reg_struct new_vals);

#endif
