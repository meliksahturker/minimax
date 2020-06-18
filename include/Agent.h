#ifndef AGENT_H
#define AGENT_H
#include"AvoidEnemyProblem.h" 
#include<stdlib.h>
#include<vector>
#include<queue>

#define RAND            0
#define ATTACK          1
#define MINIMAX         2
#define ALPHABETA       3
#define EXPECTIMINIMAX  4

#define MAX_NODE 0
#define MIN_NODE 1
#define RND_NODE 2

#define GOOD_GUY 0
#define BAD_GUY  1

#define INF (1000000)

class Agent{
				int    searchType;
				State* curState;
				int    score;
				int    agentType;
				int    evalType;
				int    index;
				int    nSteps;
		public:
				// Implement these functions
				int   miniMaxDecision();
				int   minValue (int index, State *state, int depth);
				int   maxValue (int index, State *state, int depth);

				int   alphaBetaDecision();

				int   expectiMiniMaxDecision();

				
				// These are implemented for you, but feel free to change
				Agent (int index, char searchStr[], int maxSearchDepth);

				int   selectedAction;
				int   maxSearchDepth;

				void  perceive ();
				void  search ();
				void  act ();
				bool  checkGameEnded();
				void  printScore();

				void  randomSearch();
				void  attackSearch();

				bool  isLose;
				bool  isWin;

            int maxValueAB(int index, State *state, int depth, int alpha, int beta);

            int minValueAB(int index, State *state, int depth, int alpha, int beta);

            double maxValueE(int index, State *state, int depth);

            double minValueE(int index, State *state, int depth);
};

#endif
