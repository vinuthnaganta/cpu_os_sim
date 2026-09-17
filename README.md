# cpu_os_sim
This project simulates the hardware of a simplified computer and basic firmware which can run multiple programs where each process has its own separate memory space. The programs mimic assembly language and will manipulate CPU registers.

# Files

memory.h, memory.c

	Simulates the physical memory of the computer represented by an array of structs with size of 1024. 

disk.h, disk.c

	Loads the programs into the OS memory by translating instructions into their integer OP codes before storage.

cpu.h, cpu.c

	Simulates the CPU of a computer by going through one clock cycle of fetching and executing instructions. When the time quantum (10) allocated for a 	program is up, context switching saves the CPU registers of the completed process and loads the CPU registers of the next process.

scheduler.h, scheduler.c

	Schedules the loaded processes in the process control block (PCB) and performs context switching according to a Round Robin algorithm.

smm.h, smm.c

	Implements the Simple Memory Manager (SMM) using contiguous memory allocation with dynamic partitioning. Processes are restricted from where they 	can read and write as well as how much space they can take up. If the conditions are not met, programs are denied accessed and never 	loaded/executed.

main.c

	Starts the computer and drives the CPU by loading the list of programs, getting the base address of the first program, running the CPU, and printing 	out the total number of holes counted as well as the memory for address spaces 30, 150, and 230.

Makefile

	Simple make file that allows for compiling and running the project without writing the compile command multiple times.

loop_50.txt, loop_100.txt, loop_200_invalid.txt, program_list_valid.txt, program_list_invalid_access.txt, program_list_invalid_allocation.txt

	Sample text files to test the computer with given in the project description. 

# How to Run

Unzip all the files into your preferred directory.

To run different .txt files, place the file in the same directory, open main.c, and uncomment the lines after the first 'fpath' is assigned. Initially, program_list_valid.txt is assigned.

In the terminal, run the command 'make p3' to compile all the files.
	- running this command creates .o files which can be removed by the command 'make clean'

Run './p3' to see the output.
