Stadium Thread Simulator
February 2025
Palina Pyachynskaya and Cindy Wu

To compile this simulation:
1. Type "make" into the terminal
2. Enter "gcc -g -pthread -o sim sim.c" into the terminal
3. Enter "./sim" to run the simulation

This C project is a virtual simulation of a Worcester Gompei Park, a sports facility that can house three different types of sports: baseball, football, and rugby. The field/park can only house one sport at a time with a different set of rules for each sport type. 

We decided to use 4 locks: 1 field lock that threads will attempt to grab to start playing a game, and a lock for each respective sport, which threads of a specific sport will grab when modifying certain data. These additional locks were necessary because we also created a struct for each player type, characterizing them with their thread id, field_slot_id, name, times_played, starter (meant to represent team leader or someone who starts the game for their team/pair), and found_slot. Additionally, we also initialized the field as a struct itself, which kept track of the amount of sport-specific players on the field, the field slots the occupied, the current sport on the field, the last sport played, and the amount of games each sports played (including how many games in a row rugby has played, which we used as a mechanism to avoid rugby players monopolizing the filed). Since our implementation requires the accessing and updating of so much data, we thought it would be safer to use multiple specific locks and ensure that a sport can only modify information that we decide is necessary at each point in time. Of course, 4 conditional variables complement each lock. 

In our main function, we created threads that represents 36 baseball players, 44 football players and 60 rugby players. After creation, each thread calls a function dedicated to each sports type. 

For the baseball function, we start with a randomized pre-nap for each player before they enter the field. We let the players fill in the 18 baseball slots (as that's the maximum amount of baseball players allowed on the field), inputting their "player name" (id) into each slot. When 
all slots are filled, we announce that the baseball team is ready to play. The last player to join the team is designated with an update to the "starter" field of its structure, since due to our implementation, we thought it would make most sense if only one thread was to grab the global field lock to let its team play on the field. If the field isn't empty or if baseball just played, then it would have to wait until the requirements are filled. The length of the baseball team's game is also randomly determined. After the game is done, we release each player from its slot, and broadcast to other sports that the field is open for them to play. This is to prevent monopolization of the field, so other sports have a chance to play. As a result, baseball (and football) can't play more than once in a row. Moreover, after a player finishes a game, we randomize a rest period for them to sleep through so that other players of the sport have a chance to play. This process repeats until players that played twice exit.

Since baseball and football have similar rules, our implementation for the football function was nearly identical, with the key difference being the amount of players allowed on the field as per the initial rules of the project: 18 for baseball and 22 for football.

For rugby, the team creation process is a little different from baseball and football. We let rugby players fill up slots up to 30, since up to 30 rugby players may be on the field at a time. By iterating over 30 in increments of 2, we form a pair of two players, with again one player of the pair being designated as the leader to eventually start and simulate the pair's game. Before a rugby pair may enter a field, they must verify that neither football nor baseball are playing, and that there have not been more than a certain amount of rugby games already played in a row. This mechanism is used to ensure fairness and that rugby players will not monopolize the field. Like their baseball and football counterparts, each rugby pair has a random a pre-game nap time. After a pair's game is done, they are released from their slots and put to a randomized resting period, allowing following rugby pairs to join the field as long as previous conditions are met. Once all rugby players have left the field, we signal football and baseball that the field is open to them, which allows ready teams a chance to play without risk of rugby consistently taking up every opportunity. Rugby players also leave after playing twice.

Also, please note that we have hard-coded "seed.txt" to be the file that a random seed is derived from.

Also note that in the output.txt, we didn't let the program run to its fruition (until all players have played twice) because it was taking quite a bit of time which we didn't want to waste in vain.

Defects:
Occasionally, the check-in statement that is meant to showcase the parallelism of our program for rugby threads,  may fail to print current rugby players despite a game being in progress. This might occur due to a race condition where threads aren't properly synchronized between announcing game start and updating player slot information. The simulation tries to print the check-in statement before all the player data is properly stored in their slots, leading to empty or invalid reads.

Used for CS 3013 Operating Systems.