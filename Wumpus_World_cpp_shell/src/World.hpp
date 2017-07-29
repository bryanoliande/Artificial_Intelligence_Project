#ifndef WORLD_LOCK
#define WORLD_LOCK

#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <ctime>
#include "Tile.hpp"
#include "MyAI.hpp"

#define INVALID_TILE std::pair<int,int>(-1,-1)
#define AGENT_Y agent.position.first
#define AGENT_X agent.position.second

namespace wumpus_project
{
	class World
	{
	public:
		// Constructors
		World ( char *filename );	// Construct game from file
		World ( size_t dim );		// Construct random game board from dimensions ( always square )
		
		// Destructor
		~World();	// This safely deletes the game board.
		
		// Main Function
		void run ( void );	// Runs the agent through the game
		
		// Game Information Functions
		void printGameInfo	( void );						// Main Function for Console
		void outputGameInfo	( std::string filename = "" );	// Main Function for file output
		
		// Helper Functions
		void setStepByStep ( bool sBS ); // Sets stepByStep

	private:
		// Class Variables
		size_t dimension;				// Dimension of square game board
		int points;						// Agent Score
		bool goldLooted;				// True if gold was successfully picked up
		bool canAgentShot;				// True if the agent can shoot
		bool running;					// True if the game is running
		bool gameOver;					// True if the game is over
		bool stepByStep;				// True if in debug mode ( displays board and info after every move )
		MyAI agent;						// The agent
		Coordinate facing;				// The agent's direction
		Coordinate position;			// The agent's position
		Tile** board;					// The game board
		std::set<Agent::Percept> ps;	// The set of percepts to be passed to agent
		
		// World Generation Functions
		void genWorld 	( void );
		void addPit 	( size_t r, size_t c );
		void addWumpus	( size_t r, size_t c );
		void addGold	( size_t r, size_t c );
		void addStench	( size_t r, size_t c );
		void addBreeze	( size_t r, size_t c );
		bool isInBounds	( size_t r, size_t c );
		
		// Core functions in main loop
		void perceive 	( void );		// The agent will detect and perceive percepts on call to this function.
		void act		( void );		// This function will ask the agent for an action and carry it out.
		void react		( void );		// This function will check if the play has died from a pit or a wumpus and end the game if necessary.
		
		// Game Information Functions
		void 		printBoard					( void );
		void 		printScore					( void );
		void 		printPerceptions			( void );
		void 		printAgentDirection			( void );
		void 		printLastAction				( void );
		std::string getLastAction				( Agent::Action a );
		std::string getPercept					( Agent::Percept p );
		
		// Helper Functions
		Coordinate	move			( Coordinate p, Coordinate f );	// This function returns the tile from position p one step in direction f.
		float		randomFloat		( void );						// Randomly generate a float between 0 and 1
		int			randomInt		( int limit );					// Randomly generate a int between 0 and limit
	};
}

#endif /* WORLD_LOCK */
