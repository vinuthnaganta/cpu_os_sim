#include <stdio.h>

#include "memory.h"
#include "scheduler.h"
#include "smm.h"

struct MemCell memory[1024];

struct MemCell mem_read(int addr) {
	int pid = get_current_pid();
	
	if (is_allowed_address(pid, addr) == 0) {
		printf("error in memory: pid %d not allowed to read at address %d\n", pid, addr);
		deallocate(pid);
		remove_process(pid);

		struct MemCell empty = {0};
		return empty;
	}

	return memory[addr];
}

void mem_write(int addr, struct MemCell cell_val) {
	int pid = get_current_pid();

	if (is_allowed_address(pid, addr) == 0) {
		printf("error in memory: pid %d not allowed to write at address %d\n", pid, addr);
		deallocate(pid);
		remove_process(pid);
		return;
	}
	
	memory[addr] = cell_val;
}

// used by main for output
struct MemCell mem_read_direct(int addr) {
	if (addr >= 0 && addr < 1024) {
		return memory[addr];
	}
	struct MemCell empty = {0};
	return empty;
}
// used by load_prog for disk loading only
void mem_write_direct(int addr, struct MemCell cell_val) {
	if (addr >= 0 && addr < 1024) {
		memory[addr] = cell_val;
	}
}
