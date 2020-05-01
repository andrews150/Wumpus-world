// Agent.h
//By: Andrew Springer
//Class: CptS 440 Artificial Intelligence

#ifndef AGENT_H
#define AGENT_H

#include "Action.h"
#include "Percept.h"
#include "Location.h"
#include "Orientation.h"
#include "WorldState.h"

class Agent
{
public:
	Agent ();
	~Agent ();
	void Initialize ();
	Action Process (Percept& percept);
	void GameOver (int score);
	WorldState wstate; //make global so that it would carry over rounds
	Location alocal;
	Orientation ori;
	void orientationupdater (Action& action);
	void pathoptimization();

};

#endif // AGENT_H
