run_simulation.cpp
Palina Pyachynskaya
2/25/24

This program takes in command line arguments to run a simulation of customers in multiple queues being randomly served at a coffee shop. Summary results are printed to the terminal and output is printed to a file.

To compile the program, run the command:
	g++ -Wall simulation.cpp customer.h shopQueue.cpp shopQueue.h -o run_simulation
Or run the command make run_simulation.

To run the program, run the commands:
	./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>

