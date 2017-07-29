 // MyAI.hpp

#ifndef MYAI_LOCK
#define MYAI_LOCK
#define MAXTURNS 100
#include "Agent.hpp"
#include <stack>
namespace wumpus_project
{
	class MyAI : public Agent
	{
	public:
		MyAI ( void );
		bool checkPerceptIn	( Percept p );		
		virtual Action getAction ( void );
		virtual void perceiveResult( std::set<Percept> percepts );


	private:

		Agent::Action randomMove( void );
		Agent::Action aboutFace( void);
		bool checkIfInArray( std::pair<int, int> Tile, std::pair<int,int> * arr, int size);
		void markSurroundingTilesUnsafeStench( void );
		void markSurroundingTilesUnsafeBreeze( void );
		void markTileBreeze( void );
		void markTileStench( void );
		std::pair<int, int> getNextMove( void );
		Agent::Action backTrack( void );
		
		//variables
		std::set<Agent::Percept> 	ps;				// The set of all Percepts for the current Tile.
		int							turnAround;		// Two right turns to start the journey back home. -- this variable used to avoid pits/wumpus
		int 						numTurns; 
		bool						deadWumpus; 
		bool 						stuck;
		std::pair<int,int> * unSafeTilesStench;
		std::pair<int,int> * unSafeTilesBreeze;
		std::pair<int,int> * tilesWithStench;
		std::pair<int,int> * tilesWithBreeze;
		int numUnsafeStench;
		int numUnsafeBreeze;
		std::pair<int,int> * visitedTiles;
		int numVisited;
		int numStench;
		int numBreeze;
		std::pair<int, int> lastTile;
		std::pair<int, int> lastTileBackUp;
	    std::stack<std::pair<int, int>> long_way_home; // A stack variable to backtrace once gold has been found or agent gives up
	};
}

#endif /* MYAI_LOCK */