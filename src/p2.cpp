
#include<stdio.h>
#include<string.h>
#include<stdlib.h>
#include"Util.h"
#include"Agent.h"
using namespace std;

Environment *env;
std::vector<Agent *> agents;           // Agents. the first agent is our agent, the others are enemy agents.


void parseCmdLine (int argc, char *argv[], 
				char *layoutFileName, int &maxSearchDepth,
				char *searchType, char *enemyType, 
				int &nSteps, int &seed, int  &msec);

int main(int argc, char *argv[]){
		// parse
		char layoutFileName[100]; int  maxSearchDepth; char searchType[100]; char enemyType[100]; int  msec,seed, nSteps;
		parseCmdLine(argc, argv,layoutFileName,maxSearchDepth,searchType,enemyType,seed,nSteps,msec);
		if (nSteps==-1)
				nSteps = INF;
		if (seed == -1 || seed == 0)
				srand(time(NULL));
		else srand(seed);

		// create the environment 
		env = new Environment (layoutFileName);

		// create agents
		Agent *goodGuy = new Agent (0, searchType, maxSearchDepth); 
		goodGuy->perceive();
		agents.push_back (goodGuy);
		for (int i=0;i<env->nEnemies;i++){
				Agent *badGuy = new Agent (i+1, enemyType, 0);
				badGuy->perceive();
				agents.push_back (badGuy);
		}
		env->print(msec);
		agents[0]->printScore();

		// agents perceive, search and act
		for (int i=0;i<nSteps;i++){
				env->print(msec);
				agents[0]->printScore();
				for (int i=0; !env->isGameEnd && i<env->nEnemies+1;i++){
						agents[i]->perceive ();
						agents[i]->search ();
						agents[i]->act ();
				}
				if (env->isGameEnd)
						break;
		}
		env->print(msec);
		agents[0]->printScore();
}


void printUsage()
{
		cout << "Usage:" << endl;
		cout << "./p2 -enemy RAND/ATTACK -seed <randSeed> -steps <nStepPlay> -search RAND/MINIMAX/ALPHABETA/EXPECTIMINIMAX -depth <maxSearchDepth> -eval BASIC/NOVEL -layout <layoutFile> -update <msec>" << endl;
		cout << "-enemy for enemy behavior" << endl;
		cout << "-seed for pseudo-random (-1 or 0 for random seed)" << endl;
		cout << "-steps for playing (-1 for play until end) " << endl;
		cout << "-search for search algorithm" << endl;
		cout << "-depth for search max depth" << endl;
		cout << "-eval for state evaluation function" << endl;
		cout << "-layout for layout file" << endl;
		cout << "-update for update speed in printing the solution path (no printing if -1) " << endl;
		Util::Exit("Wrong command line arguments");
}

void parseCmdLine (int argc, char *argv[], 
				char *layoutFileName, 
				int  &maxSearchDepth,
				char *searchType,
				char *enemyType,
				int  &seed,
				int  &nSteps,
				int  &msec)
{
		char evalType[100];
		if (argc!=17
						|| strcmp(argv[1],"-enemy")
						|| strcmp(argv[3],"-seed")
						|| strcmp(argv[5],"-steps")
						|| strcmp(argv[7],"-search")
						|| strcmp(argv[9],"-depth") 
						|| strcmp(argv[11],"-eval") 
						|| strcmp(argv[13],"-layout") 
						|| strcmp(argv[15],"-update")
						){
				printUsage();
		}

		strcpy(enemyType,     argv[2]);
		seed   = atoi(        argv[4]);
		nSteps = atoi(        argv[6]);
		strcpy(searchType,    argv[8]);
		maxSearchDepth = atoi(argv[10]);
		strcpy(evalType      ,argv[12]);
		strcpy(layoutFileName,argv[14]);
		msec   = atoi(        argv[16]);

		if (strcmp(searchType, "RAND") 
						&& strcmp(searchType, "MINIMAX") 
						&& strcmp(searchType, "ALPHABETA")
						&& strcmp(searchType, "EXPECTIMINIMAX"))
				printUsage();
		if (strcmp(enemyType, "RAND") && strcmp(enemyType, "ATTACK"))
				printUsage();
		if (!strcmp(evalType, "BASIC"))
				State::isBasicEval = true;
		else if (!strcmp(evalType, "NOVEL"))
				State::isBasicEval = false;
		else
				printUsage();
}
