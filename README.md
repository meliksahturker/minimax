# Boğaziçi University CMPE540 class Project 2
### Minimax, alpha-beta pruning, expectiminimax C++ implementations

Original framework implementation in C++ is written by Emre Ugur.
He adapted the project from CS188 Berkeley course material.

![pic1](https://user-images.githubusercontent.com/67103746/113911371-94377e80-97e2-11eb-94ed-842ea93f8e00.jpg)

Introduction to the problem

The environment includes:
- A: Your agent, i.e. good guy
- E: A number of other agents, i.e. bad guys or enemies enemies have indices in the layout files.
- . : Food
- i: Invisibility cloak
- In each step, first good guy moves, then each bad guy move
- s in sequence.
- If good guy and one enemy are in the same cell, good guy dies.
- If good guy consumes all the food, good guy wins.
- - If good guy puts on the invisibility cloak, he will be invisible for 40 steps. So he cannot be seen or killed by the bad guys. In each step, score of the good guy changes as follows:
- -1 even there is no movement
- +10 if good guy consumes food
- +200 if good guy puts on invisibility cloak
- +500 if good guy consumes all food
- -500 if good guy and enemy are in the same location.
- The game ends if
- Good guy consumes all food: Win!
- Good guy is killed: Lose!
- The objective is to win the game as quick as possible


### Running
./p2 -enemy RAND/ATTACK -seed -steps -search RAND/MINIMAX/ALPHABETA/EXPECTIMINIMAX -depth -eval BASIC/NOVEL -layout -update

-enemy for enemy behavior
-seed for pseudo-random (-1 or 0 for random seed)
-steps for playing (-1 for play until end)
-search for search algorithm
-depth for search max depth
-eval for state evaluation function
-layout for layout file
-update for update speed in printing the solution path (no printing if -1)

# Example runs:
- ./p2 -enemy RAND -seed -1 -steps -1 -search MINIMAX -depth 1 -eval BASIC -layout layouts/trickyClassic.lay -update 1000
- ./p2 -enemy RAND -seed -1 -steps -1 -search ALPHABETA -depth 6 -eval BASIC -layout layouts/smallClassic.lay -update 1000
- ./p2 -enemy RAND -seed -1 -steps -1 -search ALPHABETA -depth 6 -eval BASIC -layout layouts/trappedClassic.lay -update 1000
- ./p2 -enemy RAND -seed -1 -steps -1 -search EXPECTIMINIMAX -depth 6 -eval BASIC -layout layouts/trappedClassic.lay -update 1000
- ./p2 -enemy RAND -seed -1 -steps 100 -search EXPECTIMINIMAX -depth 4 -eval NOVEL -layout layouts/smallClassic.lay -update 1000
