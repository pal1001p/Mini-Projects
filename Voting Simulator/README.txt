Voting Simulator
Palina Pyachynskaya
December 2023

This program takes in command line arguments from the user to form a line-up of candidates. The user is given the choice to nominate candidates, vote for candidates, switch the voting strategy, and find the winner of the election. These choices will be prompted until the user decides to quit the session.

Winners of the election are determined based on two strategies, between which the user can switch to compare the results of the election: the most agreeable strategy, where the nominated candidate with the most votes in any one category (first, second or third preference) wins, or the most first votes strategy, where the nominated candidate with the most first votes/preferences wins.

The program handles numerous exceptions, such as checking whether a candidate has already been voted for (to prevent "cheating").

Used for CS2102 Introduction to Object-Oriented Design assignment.

