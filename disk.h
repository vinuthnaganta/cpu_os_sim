#ifndef DISK_H
#define DISK_H

#include "memory.h"
#include "scheduler.h"

int load_prog(char* fpath, int addr);
struct MemCell translate(char *buffer);
void load_programs(char fname[]);

#endif
