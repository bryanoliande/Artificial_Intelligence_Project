// World.cpp

#include "World.hpp"

using namespace std;

namespace wumpus_project
{
	// Construct a board from file.
	World::World ( char *filename )
	{
		// Class Attributes
		running = false;
		stepByStep = false;
		gameOver = false;
		goldLooted = false;
		canAgentShot = true;
		points = 0;
		facing = std::pair<int, int>(0,1);
		position = std::pair<int, int>(0,0);
		
		// Generating board from filename
		ifstream file;
		file.open(filename);
		
		// Create a blank board
		file >> dimension;
		board = new Tile*[dimension];
		for ( int i = 0; i < dimension; i++ )
			board[i] = new Tile[dimension];
		
		for ( int r = 0; r < dimension; r++ )
			for ( int c = 0; c < dimension; c++ )
				board[r][c] = Tile();
		
		int x, y;
		
		// Add the Wumpus
		file >> x >> y;
		addWumpus ( x, y );
		
		// Add the Gold
		file >> x >> y;
		addGold ( x, y );
		
		// Add the Pits
		int numOfPits;
		file >> numOfPits;
		
		while ( numOfPits > 0 && !file.eof() )
		{
			--numOfPits;
			file >> x >> y;
			addPit ( x, y );
		}
		
		// Initialize the agent
		agent = MyAI();
		board[0][0].agent = true;
		
		file.close();
	}

	// Construct a randomly generated board from fixed dimensions.
	World::World ( const size_t dim )
	{
		// Class Attributes
		running = false;
		stepByStep = false;
		gameOver = false;
		goldLooted = false;
		canAgentShot = true;
		points = 0;
		dimension = dim;
		facing = std::pair<int, int>(0,1);
		position = std::pair<int, int>(0,0);
		
		board = new Tile*[dimension];
		for ( int i = 0; i < dimension; i++ )
			board[i] = new Tile[dimension];
		
		genWorld();
		
		// Initialize the agent
		board[0][0].agent = true;
		agent = MyAI();
	}

	// Deconstructor
	World::~World()
	{
		for ( int i = 0; i < dimension; ++i )
			delete [] board[i];
		
		delete [] board;
	}

	// Randomly genarate a legal world.
	void World::genWorld ( void )
	{
		float pitRate = 0.2;
		
		// Generate Board
		for ( int r = 0; r < dimension; r++ )
			for ( int c = 0; c < dimension; c++ )
				board[r][c] = Tile();
			
		// Generate pits
		for ( int r = 0; r < dimension; r++ )
			for ( int c = 0; c < dimension; c++ )
				if ( r != 0 && c != 0 )
					if ( randomFloat() < pitRate )
						addPit ( r, c );

		// Generate wumpus and gold
		int wr = randomInt(dimension); int wc = randomInt(dimension);
		int gr = randomInt(dimension); int gc = randomInt(dimension);
		
		// Make sure wumpus and gold are not in starting position
		while ( wr == 0 && wc == 0 )
		{
			wr = randomInt(dimension);
			wc = randomInt(dimension);
		}
		
		while ( gr == 0 && gc == 0 )
		{
			gr = randomInt(dimension);
			gc = randomInt(dimension);
		}
		
		addWumpus ( wr, wc );
		addGold ( gr, gc );
	}
	
	void World::addPit ( size_t r, size_t c )
	{
		if ( isInBounds(r, c) )
		{
			board[r][c].pit = true;
			addBreeze ( r+1, c );
			addBreeze ( r-1, c );
			addBreeze ( r, c+1 );
			addBreeze ( r, c-1 );
		}
	}
	
	void World::addWumpus ( size_t r, size_t c )
	{
		if ( isInBounds(r, c) )
		{
			board[r][c].wumpus = true;
			board[r][c].stench = true;
			addStench ( r+1, c );
			addStench ( r-1, c );
			addStench ( r, c+1 );
			addStench ( r, c-1 );
		}
	}
	
	void World::addGold ( size_t r, size_t c )
	{
		if ( isInBounds(r, c) )
			board[r][c].gold = true;
	}
	
    void World::addStench ( size_t r, size_t c )
	{
		if ( isInBounds(r, c) )
			board[r][c].stench = true;
	}
	
	void World::addBreeze ( size_t r, size_t c )
	{
		if ( isInBounds(r, c) )
			board[r][c].breeze = true;
	}
	
	bool World::isInBounds ( size_t r, size_t c )
	{
		return ( r < dimension && c < dimension );
	}

	// Start game
	void World::run ( void )
	{
		perceive();
		
		running = true;
		
		while ( running )
		{
			if ( stepByStep )
			{
				printGameInfo();
				
				// Pause the game
				std::cout << "Press ENTER to continue...";
				std::cin.ignore( 999, '\n');
			}
			
			ps.clear();		// Empty Precept set, so no lingering, buggy percepts
			act(); 			// This will account for bump and scream.
			perceive();		// This won't account for bump and scream.
			react();		// This will check if the player has died from a pit or a wumpus.
		}
	}

	// The agent will detect and perceive percepts on call to this function.
	void World::perceive ( void )
	{
		int r = AGENT_Y;
		int c = AGENT_X;
		
		if ( board[r][c].stench )
			ps.insert ( Agent::STENCH );
		
		if ( board[r][c].breeze )
			ps.insert ( Agent::BREEZE );
		
		if ( board[r][c].gold )
			ps.insert ( Agent::GLITTER );
		
		agent.perceiveResult(ps);
	}
	
	// This function will ask the agent for an action and carry it out.
	void World::act ( void )
	{
		points--;
		agent.lastAction = agent.getAction();
		
		switch ( agent.lastAction )
		{
			case Agent::FORWARD:
			{
				board[AGENT_Y][AGENT_X].agent = false;
				
				Coordinate tile = move(position, facing);
				if ( tile != INVALID_TILE )
				{
					position = agent.position = tile;
				}
				else	
					ps.insert ( Agent::BUMP );
				
				board[AGENT_Y][AGENT_X].agent = true;
				break;
			}
			
			case Agent::TURN_RIGHT:
			{
				if (facing.first == 0)
					facing = agent.facing = Coordinate(facing.second, facing.first);
				else
					facing = agent.facing = Coordinate(facing.second, -facing.first);
				break;
			}
			
			case Agent::TURN_LEFT:
			{
				if (facing.second == 0)
					facing = agent.facing = Coordinate(facing.second, facing.first);
				else
					facing = agent.facing = Coordinate(-facing.second, facing.first);
				break;
			}
			
			case Agent::SHOOT:
			{
				if (canAgentShot)
				{
					canAgentShot = false;
					agent.notShot = false;
					points -= 10;
					Coordinate tile = position;
					while ( (tile = move(tile, facing)) != INVALID_TILE )
					{
						if (board[tile.first][tile.second].wumpus)
						{
							board[tile.first][tile.second].wumpus = false;
							board[tile.first][tile.second].deadWumpus = true;
							ps.insert(Agent::SCREAM);
						}
					}
				}
				break;
			}
			
			case Agent::GRAB:
			{
				if (board[AGENT_Y][AGENT_X].gold)
				{
					board[AGENT_Y][AGENT_X].gold = false;
					goldLooted = true;
					agent.gold = true;
				}
				break;
			}
			
			case Agent::CLIMB:
			{
				if ( AGENT_Y == 0 && AGENT_X == 0 )
				{
					gameOver = true;
					running = false;
					if ( goldLooted )
						points += 1000;
				}
				break;
			}
		}
	}
	
	// This function will check if the play has died from a pit or a wumpus and end the game if necessary.
	void World::react ( void )
	{
		if ( board[AGENT_Y][AGENT_X].pit == true || board[AGENT_Y][AGENT_X].wumpus == true )
		{
			running = false;
			gameOver = true;
			points -= 1000;
		}	
	}
	
	// Return the new position based on current position and facing (will not go out of bound)
	Coordinate World::move ( Coordinate p, Coordinate f )
	{
		int newX = p.first+f.first;
		int newY = p.second+f.second;
		
		if (newX > dimension-1 || newX < 0 || newY > dimension-1 || newY < 0 )
			return INVALID_TILE;
		
		return Coordinate(newX, newY);
	}

	void World::outputGameInfo ( string filename )
	{
		ofstream file;
		if ( filename == "" )
			file.open( agent.getID() + ".txt", ios::app );
		else
			file.open( filename, ios::app );
		
		file << points << "\r\n";
		
		file.close();
	}
	
	void World::printGameInfo ( void )
	{
		printBoard();
		printScore();
		printPerceptions();
		printAgentDirection();
		printLastAction();
	}
	
	void World::printBoard ( void )
	{
		std::cout << std::endl;
		for ( int r = 0; r < dimension; r++ )
		{
			for ( int c = 0; c < dimension; c++ )
				std::cout << std::setw(9) << board[r][c].stringTile();
			std::cout << std::endl << std::endl;
		}
	}

	void World::printScore ( void )
	{
		std::cout << "Score: " << points << std::endl;
	}
	
	void World::printPerceptions ( void )
	{
		std::cout << "Perceptions: ";
		
		for ( auto i = ps.begin(); i != ps.end(); i++ )
			std::cout << getPercept(*i) << " ";
		
		std::cout << std::endl;
	}

	void World::printAgentDirection ( void )
	{
		std::string direction;
		if (facing.first == 1 && facing.second == 0)
			direction = "DOWN";
		else if (facing.first == -1 && facing.second == 0)
			direction = "UP";
		else if (facing.first == 0 && facing.second == 1)
			direction = "RIGHT";
		else if (facing.first == 0 && facing.second == -1)
			direction = "LEFT";

		std::cout << "Agent Facing: " << direction << std::endl;
	}

	void World::printLastAction ( void )
	{
		std::cout << "Last Action: " << getLastAction(agent.lastAction) << std::endl;
	}

	std::string World::getLastAction ( Agent::Action a )
	{
		switch ( a )
		{
			case Agent::TURN_LEFT:
				return "TURNED LEFT";
			case Agent::TURN_RIGHT:
				return "TURNED RIGHT";
			case Agent::FORWARD:
				return "MOVED FORWARD";
			case Agent::SHOOT:
				return "SHOT THE ARROW";
			case Agent::GRAB:
				return "GRAB";
			case Agent::CLIMB:
				return "CLIMB";
		}
	}
	
	std::string World::getPercept ( Agent::Percept p )
	{
		switch ( p )
		{
			case Agent::STENCH:
				return "STENCH";
			case Agent::BREEZE:
				return "BREEZE";
			case Agent::BUMP:
				return "BUMP";
			case Agent::GLITTER:
				return "GLITTER";
			case Agent::SCREAM:
				return "SCREAM";
		}
		return "";
	}
	
	// Randomly generate a float between 0 and 1
	float World::randomFloat ( void )
	{
		return static_cast <float> (rand()) / static_cast <float> (RAND_MAX);
	}

	// Randomly generate a int between 0 and limit
	int World::randomInt ( int limit )
	{
		return rand()%limit;
	}
	
	void World::setStepByStep ( bool sBS )
	{
		stepByStep = sBS;
	}
}