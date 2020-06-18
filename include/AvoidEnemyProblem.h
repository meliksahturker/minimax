#ifndef AVOIDENEMYPROBLEM_H
#define AVOIDENEMYPROBLEM_H
#include"Util.h"
#include <iostream>

#define MOVE_UP    0
#define MOVE_DOWN  1
#define MOVE_LEFT  2
#define MOVE_RIGHT 3
#define NO_MOVE 4
#define N_MAZE_ACTIONS 5
#define MAX_MAP_SIZE 500

#define WALL    0
#define AGENT   1
#define FOOD    2
#define FREE    3
#define CLOAK   4
#define ENEMY   5
#define N_MAZE_ITEMS 6

#define MAX_N_ACTIONS 10                       


class State{
		public:
				// used to get state from environment description
				State ();
				// used in generating search states
				State (int agentPos, std::vector<bool> &foodExistList, std::vector<int> &enemyPosList, std::vector<bool> &cloakExistList, int nSteps, int invTimer);
				// used to update state from environment
				void updateState ();

				int agentPos;
				std::vector<int>  enemyPosList;
				std::vector<bool> cloakExistList;
				std::vector<bool> foodExistList;

				int  score;
				int  nSteps;
				bool isWin;
				bool isLose;

				int  invTimer; // invisibility cloak timer

				static bool isBasicEval;
				int  evaluate();
				int  basicEvaluate();
				int  novelEvaluate();


};


class Environment {
		public:
				int  nTimesExpanded;                       // number of times a node expanded during search. 
				Environment(char *fileName);
				int act(int index, int action);            // index'ed agent acts, changes the environment, gets the score

				void print(int msec);

				void getPossibleActions (int pos, std::vector<int> &actions);
				void getPossibleActions (int index, State *state, std::vector<int> &actions);

				void getDirectedActions (int selfPos, int otherPos,  // used by enemy to attack the goodguy
								std::vector<int> &actions, std::vector<int> &dirActions);

				State *getNextState (int index, State *curState, int action);

				// Names of items of the maze and actions
				static char mazeItems[N_MAZE_ITEMS];                 // item codes for file input/output
				static char mazeActionNames[N_MAZE_ACTIONS][20];     // use arrows for action names, more easy to understand
				std::string actionNames[MAX_N_ACTIONS];    

				// Static and dynamic environment items
				int agentPos;                   // initial agent position
				std::vector<int> foodPosList;   // list of food positions 
				std::vector<int> wallPosList;   // list of wall positions
				std::vector<int> enemyPosList;     // list of enemy positions 
				std::vector<int> cloakPosList;     // list of gun positions 
				int  nEnemies;

				// Environment dimensions and location indexing
				int  maze[MAX_MAP_SIZE][MAX_MAP_SIZE];                // items in the map (WALL/AGENT/FOOD/FREE)
				int  nCols;     // column number of the maze
				int  nRows;     // row number of the maze
				int  getPosFromRowCol (int r, int c);                       // position: mapping from 2d index to 1d index
				void getRowColFromPos (int &r, int &c, int index);          // position: mapping from 1d index to 2d index
				void getActionEffectRC (int &dr, int &dc, int action);      // (delta) position change with action
				int  nActions;
				
				// read the environment from file
				void readFromFile (char fileName[]);

				// related to invisibility
				bool isAgentInvisible;
				int  invTimer;

				bool isGameEnd;
                int getManhattanDistance(int posA, int posB);
};

#endif 


