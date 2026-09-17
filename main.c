#include <stdio.h>

#include "disk.h"
#include "cpu.h"
#include "scheduler.h"
#include "smm.h"

int main() {
	// !! change file here !!
	char* fpath = "program_list_valid.txt";
	//char* fpath = "program_list_invalid_access.txt";
	//char* fpath = "program_list_invalid_allocation.txt";

	// set up smm before loading the programs
	setup_smm();
	load_programs(fpath);
	
	// need to find the base of the first process
	struct reg_struct first = {0};
	first.Base = get_base();
	context_switch(first);

	int cycle_num = 1;
	int status = 1;
	while (!ready_q_status()) {
		status = clock_cycle();
		if (schedule(cycle_num, status) == 0) {
			break;
		}
		cycle_num++;
	}
	
	printf("Total Holes Counted: %d\n\n", get_hole_count());
	
	int addresses[] = {30, 150, 230};
	printf("Address\t Op\t Arg\n");
	for (int i = 0; i < 3; i++) {
		struct MemCell cell = mem_read_direct(addresses[i]);
		printf("%d\t %d\t %d\n", addresses[i], cell.op, cell.arg);
	}
	
	return 0;
}
