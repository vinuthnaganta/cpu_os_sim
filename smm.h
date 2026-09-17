#ifndef SMM_H
#define SMM_H

struct Hole {
	int base_addr;
	int size;
	struct Hole* next;
};

void setup_smm();
int allocate(int pid, int size);
void deallocate(int pid);
void add_hole(int base, int size);
void remove_hole(int base);
void merge_holes();
int find_hole(int size);
int get_base_address(int pid);
int find_empty_row();
int is_allowed_address(int pid, int addr);
int get_hole_count();

#endif
