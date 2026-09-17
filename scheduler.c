#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "cpu.h"
#include "scheduler.h"
#include "smm.h"

int time_quantum = 10;

struct PCB {
	int pid;
	int size;
	int Base;
	int PC;
	int IR0;
	int IR1;
	int AC;
	int MAR;
	struct MemCell MBR;
};

struct PCB process_table[1024];
int process_count = 0;

struct QNode {
	struct PCB *process;
	struct QNode *next;
};

struct QNode *ready_queue = NULL;

int schedule(int cycle_num, int process_status) {
	if (ready_queue == NULL) {
		return 0;
	}
	
	if (process_status == 0) {
		// deallocate the process that was just running
		int pid = ready_queue->process->pid;
		deallocate(pid);
		
		struct QNode *front = ready_queue;
		ready_queue = ready_queue->next;
		free(front);
		
		if (ready_queue == NULL) {
			return 0;
		}
		
		// get the next process (now the head)
		struct PCB *next = ready_queue->process;
		struct reg_struct new;
		
		// load reg vals from process into struct
	        new.Base = next->Base;
		new.PC = next->PC;
		new.IR0 = next->IR0;
		new.IR1 = next->IR1;
		new.AC = next->AC;
		new.MAR = next->MAR;
		new.MBR = next->MBR;
		
		// switch to new process
		context_switch(new);
	}	
	else if (cycle_num % time_quantum == 0) {
		// get the current process
		struct PCB *current = ready_queue->process;
		// find the next process
		struct PCB *next;
		if (ready_queue->next == NULL) {
			next = current;	// if only current process is left, redo that
		}
		else {
			next = ready_queue->next->process;
		}
		
		// save the next process' register vals
		struct reg_struct next_reg;
		next_reg.Base = next->Base;
		next_reg.PC = next->PC;
		next_reg.IR0 = next->IR0;
		next_reg.IR1 = next->IR1;
		next_reg.AC = next->AC;
		next_reg.MAR = next->MAR;
		next_reg.MBR = next->MBR;
		
		// switch the processes + save next process' reg values
		struct reg_struct old_vals = context_switch(next_reg);
		
		// load the old reg values back to current
		current->Base = old_vals.Base;
                current->PC = old_vals.PC;
		current->IR0 = old_vals.IR0;
		current->IR1 = old_vals.IR1;
		current->AC = old_vals.AC;
		current->MAR = old_vals.MAR;
		current->MBR = old_vals.MBR;
		
		// move current process to end
		next_process();
	}
	return 1;
}

void next_process() {
	// if queue is empty
	if (ready_queue == NULL) {
		return;
	}
	// if queue has one element
	if (ready_queue->next == NULL) {
		return;
	}
	// if queue has 2+ elements
	struct QNode *front = ready_queue;
	ready_queue = ready_queue->next;
	
	struct QNode *current = ready_queue;
	while (current->next != NULL) {
		current = current->next; 
	}
	current->next = front;
	front->next = NULL;
}

void new_process(int base, int size) {
	struct PCB *n_p = &process_table[process_count];

	// intialize regs
	n_p->pid = process_count;
	n_p->size = size;
	n_p->Base = base;
	n_p->PC = 0;
	n_p->IR0 = 0;
	n_p->IR1 = 0;
	n_p->AC = 0;
	n_p->MAR = 0;
	struct MemCell cell = {0, 0};
	n_p->MBR = cell;
	
	// add to ready queue
	struct QNode *new_proc = malloc(sizeof(struct QNode));
	new_proc->process = n_p;
	new_proc->next = NULL;

	if (ready_queue == NULL) {
		ready_queue = new_proc;
	}
	else {
		struct QNode *temp = ready_queue;
		while(temp->next != NULL) {
			temp = temp->next;
		}
		temp->next = new_proc;
	}
	process_count++;
}

int ready_q_status() {
	return ready_queue == NULL;
}

int get_base() {
	return ready_queue->process->Base;
}

int get_current_pid() {
	if (ready_queue == NULL) {
		return -1;
	}

	return ready_queue->process->pid;
}

void remove_process(int pid) {
	// if the queue is empty
	if (ready_queue == NULL) {
		return;
	}

	struct QNode *current = ready_queue;
	struct QNode *prev = NULL;

	while (current != NULL) {
		if (current->process->pid == pid) {
			// process to remove is at head
			if (prev == NULL) {
				ready_queue = current->next;
			}
			else {
				// just skip over the process
				prev->next = current->next;
			}
			free(current);
			return;
		}
		prev = current;
		current = current->next;
	}
}

int get_process_count() {
	return process_count;
}
