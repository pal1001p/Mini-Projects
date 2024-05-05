# This makefile is used as follows to regenerate files for the run_simulation program:
# make run_simulation   --  regenerates the executable register file by compiling
# make 			  	    --  same as "make run_simulation"

run_simulation: simulation.cpp customer.h shopQueue.cpp shopQueue.h 
	g++ -Wall simulation.cpp customer.h shopQueue.cpp shopQueue.h -o run_simulation

