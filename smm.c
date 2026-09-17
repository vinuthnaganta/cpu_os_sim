#include <stdio.h>
#include <stdlib.h>
#include "smm.h"

int allocation_table[256][3];

struct Hole* hole_list = NULL;

int holes_created = 0;

// to intialize the manager first
void setup_smm() {
	holes_created = 0;
	for (int i = 0; i < 256; i++) {
		// set the pid, base addr, and size all to 0 at first
		allocation_table[i][0] = 0;
		allocation_table[i][1] = 0;
		allocation_table[i][2] = 0;
	}
	// the first hole when settingup
	add_hole(0, 1024);
}

int find_empty_row() {
	for (int i = 0; i < 256; i++) {
		if (allocation_table[i][2] == 0) {
			return i;
		}
	}
	return -1;
}

int get_base_address(int pid) {
	for(int i = 0; i < 256; i++) {
		// make sure size is not 0
		if (allocation_table[i][0] == pid && allocation_table[i][2] != 0) {
			return allocation_table[i][1];
		}
	}
	return -1;
}

void remove_hole(int base) {
	struct Hole* i = hole_list;
	struct Hole* prev = NULL;

	while(i != NULL) {
		if (i->base_addr == base) {
			// remove at head
			if (prev == NULL) {
				hole_list = i->next;
			}
			else {
				prev->next = i->next;
			}
			free(i);
			return;
		}
		prev = i;
		i = i->next;
	}
}

void merge_holes() {
	struct Hole* i = hole_list;
	while (i != NULL && i->next != NULL) {
		// if the current hole ends where the next hold starts
		if ((i->base_addr + i->size) == i->next->base_addr) {
			// save the next holes size into the current
			struct Hole *temp = i->next;
			i->size += temp->size;
			i->next = temp->next;
			free(temp);
		}
		else {
			i = i->next;
		}
	}
}

void add_hole(int base, int size) {
	struct Hole* new_hole = (struct Hole*)malloc(sizeof(struct Hole));

	new_hole->base_addr = base;
	new_hole->size = size;
	new_hole->next = NULL;

	holes_created++;

	// if we need to insert at the beginning
	if (hole_list == NULL || base < hole_list->base_addr) {
		new_hole->next = hole_list;
		hole_list = new_hole;

		merge_holes();	// make sure theres no adj holes
		return;
	}

	struct Hole* i = hole_list;
	// move to where new hole will be insert (where addr less than new hole)
	while (i->next != NULL && i->next->base_addr < base) {
		i = i->next;
	}
	new_hole->next = i->next;
	i->next = new_hole;
	merge_holes();
}

int find_hole(int size) {
	struct Hole* i = hole_list;

	while (i != NULL) {
		// if the size of the hole is big enough
		if (i->size >= size) {
			int og_base = i->base_addr;
			// if the size is exact
			if (i->size == size) {
				int base = i->base_addr;
				remove_hole(base);
			}
			// if size is smaller
			else {
				i->base_addr += size;
				i->size -= size;
			}
			return og_base;
		}
		i = i->next;
	}
	return -1;
}

int allocate(int pid, int size) {
	// make sure table isnt full
	int row = find_empty_row();
	if (row == -1) {
		printf("error in smm: the allocation table is full.");
		return 0;
	}
	// try to find a hole
	int base = find_hole(size);
	if (base == -1) {
		printf("error in smm: could not allocate memory for pid %d\n", pid);
		return 0;
	}
	// yay we can allocate now
	allocation_table[row][0] = pid;
	allocation_table[row][1] = base;
	allocation_table[row][2] = size;
	return 1;
}

void deallocate(int pid) {
	for (int i = 0; i < 256; i++) {
		if (allocation_table[i][0] == pid && allocation_table[i][2] != 0) {
			int base = allocation_table[i][1];
			int size = allocation_table[i][2];
			// the process is now gone so free up the hole
			add_hole(base, size);
			
			allocation_table[i][0] = 0;
			allocation_table[i][1] = 0;
			allocation_table[i][2] = 0;
			return;
		}
	}
}

int is_allowed_address(int pid, int addr) {
	for (int i = 0; i < 256; i++) {
		if (allocation_table[i][0] == pid && allocation_table[i][2] != 0) {
			int base = allocation_table[i][1];
			int size = allocation_table[i][2];
			// if the addr is all good
			if (addr >= base && addr < (base + size)) {
				return 1;
			}
			// if we find the pid but the addr is wrong
			return 0;
		}

	}
	return 0;
}

int get_hole_count() {
	return holes_created;
}
