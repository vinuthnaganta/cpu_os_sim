#ifndef MEMORY_H
#define MEMORY_H

struct MemCell {
	int op;
	int arg;
};

struct MemCell mem_read(int addr);
void mem_write(int addr, struct MemCell cell_val);
struct MemCell mem_read_direct(int addr);
void mem_write_direct(int addr, struct MemCell cell_val);

#endif
