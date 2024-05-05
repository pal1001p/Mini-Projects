#include "customer.h"
#include "shopQueue.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <fstream>
using namespace std;

int main(int argc, char *argv[])
{
    // issue error message if number of command line arguments isn't equal to 7
    if (argc != 7)
    {
        cout << "Error: Wrong number of arguments!" << endl;
        cout << "Usage: ./run_simulation <seed> <total_min> <arrival_prob> <order_prob> <num_queues> <outputFile>" << endl;
        return 1;
    }

    // save all the commmand line arguments to local variables
    // convert forms as needed
    auto seed = atoi(argv[1]);
    auto total_min = atoi(argv[2]);
    auto arrival_prob = stof(argv[3]);
    auto order_prob = stof(argv[4]);
    auto num_queues = atoi(argv[5]);
    auto outputFile = argv[6];

    // create a vector to store all the queues, and add one ShopQueue object
    // for each queue given by the user
    vector<ShopQueue> shopQueueVector;
    for (auto i = 0; i < num_queues; i++)
    {
        ShopQueue newQueue;
        shopQueueVector.push_back(newQueue);
    }

    // create a vector of ints to store the number of customers served by each queue
    // initialize to zero
    vector<int> customersServed(num_queues, 0);

    // set the random seed
    srand(seed);

    // print welcome message
    cout << "Welcome to Gompei Coffee Shop!" << endl;
    cout << "-- # of Checkout Queues: " << num_queues << " --" << endl;

    // initialize ID and completed time variables
    auto completedTime = 0;
    auto ID = 0;

    // add a default customer to the first queue at time = 0
    shopQueueVector[0].addNewCustomer(0, 0, 0, 0);
    cout << "New customer at t = 0." << endl;

    // create a vector to hold completed customers
    vector<shared_ptr<Customer>> completed;

    // run simulation with each incremented time step
    for (int currentTime = 0; currentTime < total_min; currentTime++)
    {
        // check if a random value is less than the probability of arrival
        // to move on with adding customers
        if ((float)rand() / RAND_MAX < arrival_prob)
        {
            // incremement the customer's ID and print arrival message
            ID++;
            cout << "New customer at t = " << currentTime << endl;

            // find the shortest queue's index and length by looping over
            // the number of queues given by the user and updating the local
            // variables if a greater value is reached
            auto shortestQueueIndex = 0;
            auto shortestLength = shopQueueVector[0].getLength();
            for (auto j = 0; j < num_queues; j++)
            {
                if (shopQueueVector[j].getLength() < shortestLength)
                {
                    shortestQueueIndex = j;
                    shortestLength = shopQueueVector[j].getLength();
                }
            }
            // add the customer to the queue with the shortest length
            // ensure that their start time is equal to the current time
            // to avoid start time being 0 if queue is empty
            shopQueueVector[shortestQueueIndex].addNewCustomer(currentTime, currentTime, completedTime, ID);
        }

        // loop over for each queue given by the user
        for (int i = 0; i < num_queues; i++)
        {
            // check to ensure the queue at the moment is not empty
            if (!shopQueueVector[i].isEmpty())
            {
                // check to see if a random value is less than the order probability
                // to move on with removing customers
                if ((float)rand() / RAND_MAX < order_prob)
                {
                    // increment the vector of customers served in each queue when condition is met
                    customersServed[i]++;

                    // remove the first customer from the given queue and update their
                    // completed time to be equal to the current time
                    shared_ptr<Customer> removedCustomer = shopQueueVector[i].removeFrontCustomer();
                    removedCustomer->completedTime = currentTime;
                    // print serving message and put the customer's data into the vector of completed customers
                    cout << "Customer #" << removedCustomer->ID << " was served from t = " << removedCustomer->startTime << " to " << removedCustomer->completedTime << "." << endl;
                    completed.push_back(removedCustomer);

                    // if the vector of queues isn't empty, set the next customer's time to the current time
                    if (shopQueueVector[i].getLength() > 0)
                    {
                        shopQueueVector[i].peekFrontCustomer()->startTime = currentTime;
                    }
                }
            }
        }
    }

    // open output file for writing, check if it opens
    ofstream file(outputFile);
    if (!file.is_open())
    {
        cout << "Error: Unable to open output file" << endl;
        return 1;
    }

    // set up headings in file
    file << setw(8) << "ID" << setw(8) << "Arrive" << setw(8) << "Start" << setw(8) << "Leave" << endl;

    // for each item in the vector of completed customers, print out that person's data
    // to the file, and close the file
    for (auto person : completed)
    {
        file << setw(8) << person->ID << setw(8) << person->arrivalTime << setw(8)
             << person->startTime << setw(8) << person->completedTime << endl;
    }
    file.close();

    // for each queue, print out how many people remain in that queue
    // by getting the length from each of the queue vector's indices
    for (auto i = 0; i < num_queues; i++)
    {
        cout << "After " << total_min << " minutes, " << shopQueueVector[i].getLength() << " customers remain in queue #" << i << "." << endl;
    }

    // for each queue, print out how many people each cashier served
    // by accessing each index of the vector of customers served
    for (auto i = 0; i < num_queues; i++)
    {
        cout << "Cashier " << i << " served " << customersServed[i] << " customers." << endl;
    }

    return 0;
}

/*
./run_simulation 6 60 0.25 0.4 1 outputFile.txt
./run_simulation 6 60 0.25 0.1 1 outputFile.txt
./run_simulation 3 60 0.25 0.1 1 outputFile.txt
./run_simulation 3 60 0.25 0.1 2 outputFile.txt
./run_simulation 3 90 0.25 0.1 2 outputFile.txt
*/