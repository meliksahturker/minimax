#include"Agent.h"
#include"Util.h"
using namespace std;
extern Environment *env;
extern std::vector<Agent *> agents;           // Agents. the first agent is our agent, the others are enemy agents.

// creates the agent, sets the search algorithm of the agent
// do not change
Agent::Agent (int index, char searchStr[], int maxSearchDepth){
    if (!strcmp(searchStr,"RAND"))
        searchType = RAND;
    else if (!strcmp(searchStr,"ATTACK"))
        searchType = ATTACK;
    else if (!strcmp(searchStr,"MINIMAX"))
        searchType = MINIMAX;
    else if (!strcmp(searchStr,"ALPHABETA"))
        searchType = ALPHABETA;
    else if (!strcmp(searchStr,"EXPECTIMINIMAX"))
        searchType = EXPECTIMINIMAX;
    else
        Util::Exit("Wrong search type/string in command line");

    this->maxSearchDepth = maxSearchDepth;
    this->index = index;
    if (index == 0)
        agentType = GOOD_GUY;
    else
        agentType = BAD_GUY;

    curState = 0;
    nSteps = 0;
    score  = 0;
    isLose = false;
    isWin  = false;
}


// agent perceives the current environment
void Agent::perceive (){
    if (curState==0)
        curState = new State();
    else
        curState->updateState();
}

void Agent::act (){
    int newScore = env->act (index, selectedAction);
    if (index==0){
        score += newScore;
        if (newScore<-400)
            isLose = true;
        else if (newScore>400)
            isWin = true;
    } else{
        if (newScore < -400){
            agents[0]->score  += newScore;
            agents[0]->isLose = true;
        }
    }
    if (isWin || isLose)
        env->isGameEnd=true;
}


void Agent::printScore (){
    cout << "Score: " << score << endl;
    if (isWin)
        cout << "Agent wins!" << endl;
    else if (isLose)
        cout << "Agent loses!" << endl;
    if (isWin || isLose)
        env->isGameEnd=true;
}


// calls the corresponding search functions
void Agent::search(){
    if (searchType==RAND)
        randomSearch();
    else if (searchType==ATTACK)
        attackSearch();
    else if (searchType==MINIMAX)
        selectedAction = miniMaxDecision();
    else if (searchType==ALPHABETA)
        selectedAction = alphaBetaDecision();
    else if (searchType==EXPECTIMINIMAX)
        selectedAction = expectiMiniMaxDecision();
    if (index==0){
        cout << "nTimesExpanded: " << env->nTimesExpanded << endl;
        cout << "selectedAction: " << env->actionNames[selectedAction] << endl;
    }
}

// is applicable only to enemy
// with 70%, random action
void Agent::attackSearch(){
    if (env->invTimer>=0 || rand()%10>7){
        randomSearch();
        return;
    }
    if (index==0){
        randomSearch();
        return;
    }
    vector<int> actions;
    env->getPossibleActions (curState->enemyPosList[index-1], actions);
    vector<int> dirActions;
    env->getDirectedActions (curState->enemyPosList[index-1], curState->agentPos, actions, dirActions);
    if (dirActions.size()==0)
        randomSearch();
    else{
        selectedAction = dirActions[rand()%dirActions.size()];
    }
}
// assigned selectedAction to one of the possible actions
void Agent::randomSearch(){
    vector<int> actions;
    if (index==0)
        env->getPossibleActions (curState->agentPos, actions);
    else
        env->getPossibleActions (curState->enemyPosList[index-1], actions);
    selectedAction = actions[rand()%actions.size()];
}


// MINIMAX
int Agent::minValue (int index, State *state, int depth){
    int minVal = INF;

    if (depth == maxSearchDepth || state->isLose || state->isWin){
        return state->evaluate();
    }

    vector<int> actions;
    actions.clear();

    env->getPossibleActions (index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);

        if(index < agents.size() - 1){
            minVal =  min(minVal,minValue(index+1, nextState, depth));
        }
        else{
            minVal =  min(minVal, maxValue(0, nextState, depth + 1));
        }
    }

    return minVal;

}

// index is always 0
int Agent::maxValue (int index, State *state, int depth){
    int maxVal = -INF;

    if (depth == maxSearchDepth || state->isLose || state->isWin){
        return state->evaluate();
    }

    vector<int> actions;
    actions.clear();
    env->getPossibleActions (index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);
        maxVal = max(maxVal, minValue(1, nextState, depth + 1));
    }

    return maxVal;
}

/**
	* Hints:
	* State *curState = new State(); // current state
	* env->getPossibleActions (0, curState, actions);
	* State *nextState = env->getNextState (0, curState, action);
	* int retVal = state->evaluate();
	*/
int Agent::miniMaxDecision(){
    int maxAction;
    int miniMaxVal = -INF;
    // fill in
    vector<int> actions, actionResults;
    actions.clear();
    actionResults.clear();

    env->getPossibleActions (0,curState, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(0,curState, actions[i]);
        actionResults.push_back(minValue(1, nextState, 1));
    }

    // returns the max of actionResults
    for (int i = 0; i < actions.size(); i++){
        if (actionResults[i] > miniMaxVal){
            miniMaxVal = actionResults[i];
            maxAction = actions[i];
        }
    }
    cout << "Minimax: "   << miniMaxVal << endl;
    return maxAction;
}





// ALPHA BETA

int Agent::maxValueAB (int index, State *state, int depth, int alpha, int beta){
    int maxVal = -INF;

    if (depth == maxSearchDepth || state->isLose || state->isWin) {
        return state->evaluate();
    }

    vector<int> actions;
    actions.clear();
    env->getPossibleActions (index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);
        maxVal = max(maxVal, minValueAB(1, nextState, depth + 1, alpha, beta));
        if (maxVal >= beta){
            return maxVal;
        }
        alpha = max(alpha, maxVal);
    }
    return maxVal;
}

int Agent::minValueAB (int index, State *state, int depth, int alpha, int beta){
    int minVal = INF;

    if (depth == maxSearchDepth || state->isLose || state->isWin){
        return state->evaluate();
    }

    vector<int> actions;
    actions.clear();
    env->getPossibleActions (index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);
        if(index < agents.size() - 1){
            minVal =  min(minVal, minValueAB(index+1, nextState, depth, alpha, beta));

            /*
            if (minVal <= -400){ // if current minVal is very negative, it's a death scenario already so no need to check for other enemies.
                return minVal;
            }
            */

            if (minVal <= alpha){
                return minVal;
            }
        }
        else{
            minVal =  min(minVal, maxValueAB(0, nextState, depth + 1, alpha, beta));
            if (minVal <= alpha){
                return minVal;
            }
        }
        beta = min(beta, minVal);
    }

    return minVal;
}

int Agent::alphaBetaDecision(){
    int maxAction;
    int miniMaxVal = -INF;
    int alpha = -INF;
    int beta = INF;
    // fill in
    vector<int> actions, actionResults;
    actions.clear();
    actionResults.clear();

    env->getPossibleActions (0,curState, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(0,curState, actions[i]);
        actionResults.push_back(minValueAB(1, nextState, 1, alpha, beta));
        alpha = max(actionResults[i], alpha);
    }

    // returns the max of actionResults
    for (int i = 0; i < actions.size(); i++){
        if (actionResults[i] > miniMaxVal){
            miniMaxVal = actionResults[i];
            maxAction = actions[i];
        }
    }
    cout << "Minimax: "   << miniMaxVal << endl;
    return maxAction;
}





// ExpectiMiniMax

double Agent::minValueE (int index, State *state, int depth){
    double minVal = INF;

    if (depth == maxSearchDepth || state->isLose || state->isWin){
        return state->evaluate();
    }

    double sum = 0;
    vector<int> actions;
    actions.clear();
    env->getPossibleActions(index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);

        if(index < agents.size() - 1){
            minVal =  min(minVal,minValueE(index+1, nextState, depth));
        }
        else{
            sum+= maxValueE(0, nextState, depth + 1) / actions.size();
        }
    }
    minVal = min(minVal, sum);

    // CODE OF PREVIOUS ATTEMPT
    /*
    for (int j =1; j < agents.size(); j++) {
        vector<int> actions;
        actions.clear();
        env->getPossibleActions(j, state, actions);

        for (int i = 0; i < actions.size(); i++) {
            State *nextState = env->getNextState(j, state, actions[i]);
            sum += maxValueE(0, nextState, depth + 1) / (actions.size());
        }
        minVal = min(minVal, sum);
    }
    */

    return minVal;
}

double Agent::maxValueE (int index, State *state, int depth){
    double maxVal = -INF;
    if (depth == maxSearchDepth || state->isLose || state->isWin){
        return state->evaluate();
    }

    vector<int> actions;
    actions.clear();
    env->getPossibleActions (index, state, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(index, state, actions[i]);
        maxVal = max(maxVal, minValueE(1, nextState, depth + 1));
    }

    return maxVal;
}

int Agent::expectiMiniMaxDecision(){
    int expectiMaxAction;
    double expectiMiniMaxVal = -INF;

    vector<int> actions, actionResults;
    actions.clear();
    actionResults.clear();

    env->getPossibleActions (0,curState, actions);

    for(int i = 0; i < actions.size(); i++ ) {
        State *nextState = env->getNextState(0,curState, actions[i]);
        actionResults.push_back(minValueE(1, nextState, 1));
    }

    // returns the max of actionResults
    for (int i = 0; i < actions.size(); i++){
        if (actionResults[i] > expectiMiniMaxVal){
            expectiMiniMaxVal = actionResults[i];
            expectiMaxAction = actions[i];
        }
    }

    cout << "Expected Minimax: "   << expectiMiniMaxVal << endl;
    return expectiMaxAction;
}
