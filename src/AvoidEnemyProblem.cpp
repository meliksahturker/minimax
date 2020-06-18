#include"AvoidEnemyProblem.h"
using namespace std;
extern Environment *env;
#define INV_TIMER (40)


/******************************* State class **********************/

/**
	* Read the current state from the environment
	*/
State::State (){
		agentPos = env->agentPos;
		for (int i=0;i<env->foodPosList.size();i++)
				foodExistList.push_back(true);
		for (int i=0;i<env->enemyPosList.size();i++)
				enemyPosList.push_back(env->enemyPosList[i]);
		for (int i=0;i<env->cloakPosList.size();i++)
				cloakExistList.push_back(true);
		nSteps  = 0;
		isWin   = false;
		isLose  = false;
		invTimer = env->invTimer;
}

/**
	* Read the current state from the environment
	*/
void State::updateState(){
		agentPos = env->agentPos;
		foodExistList.clear();
		cloakExistList.clear();
		for (int i=0;i<env->foodPosList.size();i++)
				foodExistList.push_back(true);
		for (int i=0;i<env->enemyPosList.size();i++)
				enemyPosList[i] = env->enemyPosList[i];
		for (int i=0;i<env->cloakPosList.size();i++)
				cloakExistList.push_back(true);
		invTimer = env->invTimer;
}

/** 
	* Initialize a state with the required variables
	*/
State::State (int agentPos, std::vector<bool> &foodExistList, 
				std::vector<int> &enemyPosList, std::vector<bool> &cloakExistList, int nSteps, int invTimer){
		this->agentPos = agentPos;
		for (int i=0;i<foodExistList.size();i++)
				this->foodExistList.push_back(foodExistList[i]);
		for (int i=0;i<enemyPosList.size();i++)
				this->enemyPosList.push_back(enemyPosList[i]);
		for (int i=0;i<cloakExistList.size();i++)
				this->cloakExistList.push_back(cloakExistList[i]);
		this->nSteps = nSteps;
		this->isWin  = false;
		this->isLose = false;
		this->invTimer = invTimer;
}

bool State::isBasicEval = true;

int State::evaluate (){
    if (isBasicEval)
        return basicEvaluate();
    else
        return novelEvaluate();
}

/**
	* score-=1   in  each step
	* score+=10  for each consumed food
	* score+=200 for each cloak put on
	* score-=500 and isLose=true if hits to enemy while visible
	* score+=500 and isWin=true  if all food is consumed
	**/
int State::basicEvaluate (){
		score=0;
		for (int i=0;i<foodExistList.size();i++)
				if (foodExistList[i]==false)
						score+=10;
		if (score == foodExistList.size()*10)
				isWin = true;
		for (int i=0;i<cloakExistList.size();i++)
				if (cloakExistList[i]==false)
						score+=200;
		score -= nSteps;
		if (isWin)
				score += 500;
		if (isLose)
				score -= 500;
		return score;
}

int State::novelEvaluate (){
		int score = 0;
		int INF  = 1000000;

        int distFood = INF;
        int distEnemy = INF;
        int distCloak = INF;
        int nFoodsLeft = 0;
        int nCloak = 0;
        int mDistFood, mDistEnemy, mDistCloak, mNFoodsLeft;

        // calculates distance to closest Food
        for(int i =0; i < env->foodPosList.size(); i ++){
            distFood = min(distFood, env->getManhattanDistance(agentPos, env->foodPosList[i]));
        }

        // calculates distance to closest Enemy
        for(int i =0; i < env->enemyPosList.size(); i ++){
            distEnemy = min(distEnemy, env->getManhattanDistance(agentPos, env->enemyPosList[i]));
        }

        // calculates distance to closest Enemy
        for(int i =0; i < env->cloakPosList.size(); i ++){
            distCloak = min(distCloak, env->getManhattanDistance(agentPos, env->cloakPosList[i]));
        }

        nFoodsLeft = foodExistList.size();
        nCloak = cloakExistList.size();

        mDistFood = 100;
        mDistEnemy = 10;
        mDistCloak = 1000;
        mNFoodsLeft = 10;


        // Once all cloaks are taken, we do not need to take cloaks into consideration.
        if (nCloak == 0){
            mDistCloak = 0;
        }


        score += -(distFood * mDistFood) + (distEnemy*mDistEnemy) - (nFoodsLeft*mNFoodsLeft) - ((distCloak + 1)*mDistCloak);

        int winloss = 0;
        if (isWin)
            winloss = 50000;
        if (isLose)
            winloss = 50000;

        score -= winloss;

		return score;

}

int Environment::getManhattanDistance(int posA, int posB){
    int xA, yA, xB, yB;
    getRowColFromPos(xA, yA, posA);
    getRowColFromPos(xB, yB, posB);

    return (abs(xA - xB) + abs(yA - yB));
}

/******************************* AvoidEnemyProblem class **********************/
char Environment::mazeItems[N_MAZE_ITEMS]={'%','A','.',' ','i','E'};
char Environment::mazeActionNames[N_MAZE_ACTIONS][20] = 
{"MOVE_UP", "MOVE_DOWN", "MOVE_LEFT", "MOVE_RIGHT","NO_MOVE"};

/**
	* Creates a new environment
	* Reads the maze from file
	* Initializes the maze data
	**/
Environment::Environment(char *layoutFileName){
		nTimesExpanded=0;
		for (int a=0;a<N_MAZE_ACTIONS;a++)
				actionNames[a] = string (mazeActionNames[a]);
		nCols=nRows=-1;

		for (int i=0;i<10;i++){
				enemyPosList.push_back(-1);
		}
		readFromFile (layoutFileName);
		isAgentInvisible = false;
		invTimer = -1;
		isGameEnd = false;
}




// index 0: good guy
// ow     : bad guys
/**
	* Agent with the corresponding index acts
	* index 0: good guy
	* otherwise: bad guy
	* Returns the actual score of the actual action
	*/
int Environment::act (int index, int action){
		int realScoreChange=0;
		if (index==0){
				invTimer--;
				realScoreChange=-1;
				nTimesExpanded = 0;
		}

		State curState;
		State *nextState = getNextState (index, &curState, action); 
		int pos;
		if (index==0){
				agentPos = nextState->agentPos;
				// If invisible, enemy cannot kill
				if (invTimer<0){ 
						for (int i=0;i<enemyPosList.size();i++){
								if (enemyPosList[i]==agentPos){
										realScoreChange-=500;
										break;
								}
						}
				}
				// Really consumes the food
				for (int i=0;i<foodPosList.size();i++){  
						if (foodPosList[i]==agentPos){
								foodPosList.erase (foodPosList.begin()+i);
								realScoreChange+=10;
								break;
						}
				}
				// Really put on the invisibility cloak (for INV_TIMER steps)
				for (int i=0;i<cloakPosList.size();i++){  
						if (cloakPosList[i]==agentPos){
								cloakPosList.erase (cloakPosList.begin()+i);
								realScoreChange+=200;
								invTimer = INV_TIMER;
								break;
						}
				}
		}else{
				enemyPosList[index-1] = nextState->enemyPosList[index-1];
				// If invisible, enemy cannot kill
				if (invTimer<0){
						if (nextState->agentPos == nextState->enemyPosList[index-1]){
								realScoreChange-=500;
						}
				}
		}
		// If all food is consumed, add +500 to score
		if (foodPosList.size()==0){
				realScoreChange+=500;
		}
		return realScoreChange;
}

/**
	* Returns a list of actions in dirActions that can potentially
	* move the agent in selfPos in direction towards the agent in otherPos.
	* This list of actions are selected from possibleActions.
	* The actions in dirActions might be used by attacking enemy.
	*/
void Environment::getDirectedActions (int selfPos, int otherPos, 
				std::vector<int> &possibleActions, std::vector<int> &dirActions){
		int selfR, selfC;
		int otherR, otherC;
		dirActions.clear();
		getRowColFromPos (selfR, selfC, selfPos);
		getRowColFromPos (otherR, otherC, otherPos);
		for (int i=0;i<possibleActions.size();i++){
				int a = possibleActions[i];
				switch (a){
						case MOVE_UP:
								if (otherR <= selfR)
										dirActions.push_back(a);
								break;
						case MOVE_DOWN:
								if (otherR >= selfR)
										dirActions.push_back(a);
								break;
						case MOVE_LEFT:
								if (otherC <= selfC)
										dirActions.push_back(a);
								break;
						case MOVE_RIGHT:
								if (otherC >= selfC)
										dirActions.push_back(a);
								break;
				}
		}
}

/**
	* Given agent position, returns the list of possible actions
	* The action order is very important!
	*/
void Environment::getPossibleActions (int pos, vector<int> &actions){
		int dC, dR, curC, curR, nextC, nextR;
		getRowColFromPos (curR, curC, pos);
		actions.clear();
		for (int a=0;a<N_MAZE_ACTIONS;a++){
				getActionEffectRC (dR, dC, a);
				nextR = curR+dR;
				nextC = curC+dC;
				if (maze[nextR][nextC] == WALL || nextR<0 || nextR>=nRows || nextC<0 || nextC>=nCols)
						continue;
				actions.push_back(a);
		}
}

/**
	* Given agent index and environment state, returns the list of possible actions
	* The action order is very important!
	*/
void Environment::getPossibleActions (int index, State *state, vector<int> &actions){
		int dC, dR, curC, curR, nextC, nextR;
		int pos;
		if (index==0)
				pos = state->agentPos;
		else
				pos = state->enemyPosList[index-1];
		getPossibleActions (pos, actions);
}


/**
	* Returns the next state
	* Given index of agent, the action and the current state 
	* If agent hits to the wall, return 0
	*/
State *Environment::getNextState (int index, State *state, int action){
		nTimesExpanded++;
		int curPos;
		if (index==0)
				curPos = state->agentPos;
		else 
				curPos = state->enemyPosList[index-1];
		int dC, dR, curC, curR, nextC, nextR;
		getRowColFromPos (curR, curC, curPos);
		getActionEffectRC (dR, dC, action);
		nextR = curR+dR;
		nextC = curC+dC;
		if (maze[nextR][nextC] == WALL || nextR<0 || nextR>=nRows || nextC<0 || nextC>=nCols){
				return 0;
		}

		int newPos = getPosFromRowCol (nextR, nextC);
		State *nextState;
		if (index == 0){
				nextState = new State (newPos, 
								state->foodExistList, state->enemyPosList, state->cloakExistList, state->nSteps+1, state->invTimer-1);
		}else{
				nextState = new State (state->agentPos, 
								state->foodExistList, state->enemyPosList, state->cloakExistList, state->nSteps, state->invTimer);
				nextState->enemyPosList[index-1]=newPos;
		}
		
		// get killed by the enemy
		for (int i=0;i<nextState->enemyPosList.size();i++){
				if (nextState->invTimer<0){
						if (nextState->agentPos==nextState->enemyPosList[i]){ 
								nextState->isLose = true;
								break;
						}
				}
		}

		if (index==0){
				// consume food
				for (int i=0;i<nextState->foodExistList.size();i++){
						if (nextState->agentPos==foodPosList[i]){
								nextState->foodExistList[i]=false;
								break;
						}
				}
				// put on the invisibility cloak
				for (int i=0;i<nextState->cloakExistList.size();i++){
						if (nextState->agentPos==cloakPosList[i]){
								nextState->invTimer = INV_TIMER;
								nextState->cloakExistList[i]=false;
								break;
						}
				}
		}
		nextState->evaluate();
		return nextState;
}



/** 
	* Print out the maze
	*/
void Environment::print(int msec) {
		usleep(msec*1000); system("clear");
		int agentC, agentR;
		getRowColFromPos (agentR, agentC, agentPos);
		for (int r=0;r<nRows;r++){
				for (int c=0;c<nCols;c++){
						int pos = getPosFromRowCol (r,c);

						bool isAssigned=false;
						for (int i=0;i<enemyPosList.size();i++){
								if (pos == enemyPosList[i]){
										isAssigned = true;
										if (invTimer>=0)
												cout << FGRN("E");
										else
												cout << FRED("E") ;
										break;
								}
						}
						if (isAssigned) continue;

						if (agentPos == pos){
								cout << FBLU("A") ;
								continue;
						}
						if (maze[r][c]==WALL){
								cout << "%" ;
								continue;
						}
						if (maze[r][c]==FOOD){
								for (int i=0;i<foodPosList.size();i++){
										if (foodPosList[i]==pos){
														cout << FGRN(".");
												isAssigned = true;
												break;
										}
								}
						}
						if (isAssigned) continue;

						if (maze[r][c]==CLOAK){
								for (int i=0;i<cloakPosList.size();i++){
										if (cloakPosList[i]==pos){
												cout << FGRN("i");
												isAssigned = true;
												break;
										}
								}
						}
						if (isAssigned) continue;

						cout << " ";
				}
				cout << endl;
		}
}

/**
	* Read the maze from file
	* %: wall
	* A: Our agent (good guy)
	* E: Enemy agents (bad guys)
	* .: Food
	* i: Invisibility cloak
	*/
void Environment::readFromFile (char fileName[]){
		FILE *fp = fopen (fileName,"r");
		if (fp==0){
				Util::Exit("Cannot open maze file for reading");
		}
		char s;
		int c;
		nEnemies = 0;
		nRows=c=0;
		while (fscanf(fp,"%c",&s)!=EOF){
				if (s>='0' && s<='9'){
						nEnemies++;
						enemyPosList[(int)(s-'0')] = (getPosFromRowCol (nRows, c));
						maze[nRows][c++]=ENEMY;
						continue;
				}
				switch (s){
						case '%':
								wallPosList.push_back (getPosFromRowCol (nRows, c));
								maze[nRows][c++]=WALL;
								break;
						case 'A':
								agentPos = getPosFromRowCol (nRows, c);
								maze[nRows][c++]=FREE; 
								break;
						case '.':
								foodPosList.push_back (getPosFromRowCol (nRows, c));
								maze[nRows][c++]=FOOD;
								break;
						case 'i':
								cloakPosList.push_back (getPosFromRowCol (nRows, c));
								maze[nRows][c++]=CLOAK;
								break;
						case ' ':
								maze[nRows][c++]=FREE;
								break;
						case '\n':
								if (nCols==-1)
										nCols = c;
								else if (nCols != c){
										Util::Exit("Environment in maze file in dimensions");
								}
								c=0; 
								nRows++;
								break;
						default: 
								std::cout << s << std::endl;
								Util::Exit("Environment in maze file in characters");
				}
		}
		if (c!=0)
				nRows++;
		nActions = N_MAZE_ACTIONS;
		enemyPosList.erase(enemyPosList.begin()+nEnemies,enemyPosList.end());
}

/**
	* Utility function that returns 1d index from 2d position
	*/
int  Environment::getPosFromRowCol (int r, int c){
		return (r*nCols + c);
}

/** 
	* Utility function that returns 2d position from 1d index
	*/
void Environment::getRowColFromPos (int &r, int &c, int index){
		c = index%nCols;
		r = floor(1.0*index/nCols);
}

/**
	* Returns the change in 2d position with an action
	*/
void Environment::getActionEffectRC (int &dr, int &dc, int action){
		dr=dc=0;
		switch (action){
				case (MOVE_UP):    dr = -1; break;
				case (MOVE_DOWN):  dr = +1; break;
				case (MOVE_LEFT):  dc = -1; break;
				case (MOVE_RIGHT): dc = +1; break;
		}
}
