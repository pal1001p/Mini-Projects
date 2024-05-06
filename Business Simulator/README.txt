Business Simulator
Palina Pyachynskaya
March 2024

This program takes in command line arguments to run a simulation of customers in multiple queues being randomly served at a coffee shop. Summary results are printed to the terminal and output is printed to a file. Summary results include information like number of customers served within certain time frames, and probabilities or arrival and departure (all based on random seed given by user).

To compile the program, run the command:
	g++ -Wall simulation.cpp customer.h shopQueue.cpp shopQueue.h -o run_simulation
Or run the command make run_simulation.

To run the program, run the commands:
	./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>

Used for CS2303 System Programming Concepts assignment.
