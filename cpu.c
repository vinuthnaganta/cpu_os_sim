#include <stdio.h>
#include <stdlib.h>

#include "memory.h"
#include "cpu.h"

// global CPU registers
int Base = 0, PC = 0, IR0 = 0, IR1 = 0, AC = 0, MAR = 0;

// store the MBR register as a struct as the memory array is made of structs
struct MemCell MBR;

// flag to check if exit instruction is encountered
int not_exit = 1;

void fetch_instruction(int addr) {
	// find instruction in memory and set registers
	int location = mem_address(addr);
	struct MemCell cell = mem_read(location);
	IR0 = cell.op;
	IR1 = cell.arg;
}

void execute_instruction() {
	switch(IR0) {
		case 0:
			not_exit = 0; // set flag as 0 to exit from clock_cycle
			break;
		case 1:
			AC = IR1;	
			break;
		case 2:
			AC = MBR.arg;
			break;
		case 3:
			AC = MAR;
			break;
		case 4:
			MBR.arg = AC;
			break;
		case 5:
			MAR = AC;
			break;
		case 6:
			MBR = mem_read(MAR);
			break;
		case 7:
			mem_write(MAR, MBR);
			break;
		case 8:
			AC += MBR.arg;
			break;
		case 9:
			AC *= MBR.arg;
			break;
		case 10:
			AC = (MBR.arg && AC); // logical AND
			break;
		case 11:
			AC = (MBR.arg || AC); // logical OR
			break;
		case 12:
			if (AC != 0) {
				PC = IR1 - 1; // need move back as PC is at next instr
			}
			break;
		case 13:
			break;
	}
}

int mem_address(int l_addr) {
	return Base + l_addr; // return true memory address
}

int clock_cycle() {
	fetch_instruction(PC);
	execute_instruction();
	PC++;
	if (not_exit == 0) {
		return 0;
	}
	else {
		return 1;
	}
}

struct reg_struct context_switch(struct reg_struct new_vals) {
	struct reg_struct old_vals;
	// save current state
	old_vals.Base = Base;
	old_vals.PC = PC;
	old_vals.IR0 = IR0;
	old_vals.IR1 = IR1;
	old_vals.AC = AC;
	old_vals.MAR = MAR;
	old_vals.MBR = MBR;

	// load new state into regs
	Base = new_vals.Base;
	PC = new_vals.PC;
	IR0 = new_vals.IR0;
	IR1 = new_vals.IR1;
	AC = new_vals.AC;
	MAR = new_vals.MAR;
	MBR = new_vals.MBR;
	
	not_exit = 1; // reset for new process
	return old_vals;
}
