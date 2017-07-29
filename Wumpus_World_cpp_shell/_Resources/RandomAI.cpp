// MyAI.cpp
// Author: Minjae Wee	mwee@uci.edu
// Revised by Abdullah Younis

// This is a dummy source file. It will not be compiled, and is only supplied for your study.

#include "MyAI.hpp"

namespace wumpus_project
{
	MyAI::MyAI() : Agent()
	{
		position = std::pair<int, int>(0, 0);	// start @ cell (0,0)
		facing = std::pair<int, int>(0, 1);		// start facing right
		gold = false;
		notShot = true;
		turnAround = 0;
		lastAction = CLIMB;
		long_way_home.push_back(CLIMB);
		srand( time(NULL) ); // Set random seed
	}

	// Returns true if a given percept is @ current position; otherwise false
	bool MyAI::checkPerceptIn( Percept p )
	{
		const bool is_in = ps.find(p) != ps.end();
		return is_in;
	}
	
	// Returns a random action
	Agent::Action MyAI::getAction()
	{
		if ( checkPerceptIn(Agent::GLITTER) )	// Grab gold if can.
		{
			gold = true;
			return GRAB;
		}
		
		int action = rand() % ACTION_SET_SIZE;	// Select a random action.
		
		switch ( action )
		{
		case 0:
			return TURN_RIGHT;
			
		case 1:
			return TURN_LEFT;
			
		case 2:
			return FORWARD;
			
		case 3:
			notShot = false;
			return SHOOT;
			
		case 4:
			return GRAB;
			
		case 5:
		default:
			return CLIMB;
		}
	}

	// Update percepts
	void MyAI::perceiveResult(std::set<Percept> percepts)
	{
		ps = percepts;
	}
}