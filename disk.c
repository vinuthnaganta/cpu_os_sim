#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "memory.h"
#include "disk.h"
#include "scheduler.h"
#include "smm.h"

int load_prog(char* fpath, int addr) {
	FILE* fhandle;
	char buffer[256];
	int size = 0;

	fhandle = fopen(fpath, "r");
	if (fhandle == NULL) {
		perror("error opening file.");
		return 0;
	}
	
	// translate the instructions from the file and store in memory
	while (fgets(buffer, sizeof(buffer), fhandle)) {
		struct MemCell cell = translate(buffer);
		// skip comments, new lines, and invalid instructions
		if (cell.op != -1) {
			// just for loading
			mem_write_direct(addr, cell);
			addr++;
			size++;
		}
	}

	fclose(fhandle);
	return size;
}

struct MemCell translate (char* buffer) {
	struct MemCell cell;

	// skip any comments and new lines
	if (buffer[0] == '/' || buffer[0] == '\n') {
		cell.op = -1;
		cell.arg = 0;
		return cell;
	}

	char* instr = strtok(buffer," \n\r");
	char* arg = strtok(NULL, " \n\r");

	// convert each instruction to the opcode and argument
	if (arg != NULL) {
		cell.arg = atoi(arg);
	}
	else {
		cell.arg = 0;
	}

	if (strcmp(instr, "exit") == 0) {
		cell.op = 0;
	}
	else if (strcmp(instr,"load_const") == 0) {
		cell.op = 1;
	}
	else if (strcmp(instr,"move_from_mbr") == 0) {
		cell.op = 2;
	}
	else if (strcmp(instr,"move_from_mar") == 0) {
		cell.op = 3;
	}
	else if (strcmp(instr,"move_to_mbr") == 0) {
		cell.op = 4;
	}
	else if (strcmp(instr,"move_to_mar") == 0) {
		cell.op = 5;
	}
	else if (strcmp(instr,"load_at_addr") == 0) {
		cell.op = 6;
	}
	else if (strcmp(instr,"write_at_addr") == 0) {
		cell.op = 7;
	}
	else if (strcmp(instr,"add") == 0) {
		cell.op = 8;
	}
	else if (strcmp(instr,"multiply") == 0) {
		cell.op = 9;
	}
	else if (strcmp(instr,"and") == 0) {
		cell.op = 10;
	}
	else if (strcmp(instr,"or") == 0) {
		cell.op = 11;
	}
	else if (strcmp(instr,"ifgo") == 0) {
		cell.op = 12;
	}
	else if (strcmp(instr,"sleep") == 0) {
		cell.op = 13;
	}
	else {
		printf("invalid instruction: '%s'\n", instr);
		cell.op = -1;
	}
	return cell;
}

void load_programs(char fname[]) {
	FILE* fhandle;
	fhandle = fopen(fname, "r");

	if (fhandle == NULL) {
		perror("error opening program list file");
		return;
	}
	
	int size;
	char buffer[256];

	while (fscanf(fhandle, "%d %s", &size, buffer) == 2) {
		int pid = get_process_count();
		
		if (allocate(pid, size) == 0) {
			printf("error in disk: program %s (pid %d) could not be allocated, size of %d is too big\n", buffer, pid, size);
			continue;
		}
		
		int base = get_base_address(pid);

		load_prog(buffer, base);

		new_process(base, size);
		//load_prog(buffer, base);
		//printf("loading %s at %d, size %d\n", buffer, base, size);
	}

	fclose(fhandle);
}
