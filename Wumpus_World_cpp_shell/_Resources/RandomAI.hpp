// MyAI.hpp
// Author: Minjae Wee   mwee@uci.edu
// Revised by Abdullah Younis

// This is a dummy source file. It will not be compiled, and is only supplied for your study.

// ======================== RANDOMAI ========================
// This agent will randomly select a action every tick. The
// only exception is when the agent finds gold. It will then
// pick it up, trace its steps backwards, and climb out.
// ==========================================================

#ifndef MYAI_LOCK
#define MYAI_LOCK

#define ACTION_SET_SIZE 6
#include "Agent.hpp"
#include <algorithm>
#include <vector>
#include <string>
#include <ctime>

namespace wumpus_project
{
	class MyAI : public Agent
	{
	public:
		MyAI ( void );
		bool					checkPerceptIn	( Percept p );						// Returns true if a given percept is @ current position; otherwise false
		virtual Action 			getAction		( void );							// Returns a random action
		virtual void 			perceiveResult	( std::set<Percept> percepts );		// Updates percepts

	private:
		std::set<Agent::Percept> 	ps;				// The set of all current percepts.
		int							turnAround;		// Two right turns to start the journey back home.
	};
}

#endif /* MYAI_LOCK */