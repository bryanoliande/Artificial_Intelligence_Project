 // MyAI.cpp

#include "MyAI.hpp"
#include<iostream>
using namespace std;
namespace wumpus_project
{
	MyAI::MyAI() : Agent()
	{
		// Class Attributes
		notShot = true;
		gold = false;
		deadWumpus = false;
		lastAction = CLIMB;   
		turnAround = 0;
		numTurns = 0;
		position = std::pair<int, int>(0, 0);	// start @ cell (0,0)
		facing = std::pair<int, int>(0, 1);		// The Tile the agent would be on if it moved forward. start facing right NOTE: originally was provided as (1,0), changed b/c its (0, 1) in world.cpp file
		numUnsafeStench = 0;
		numUnsafeBreeze = 0;
		unSafeTilesStench = new std::pair<int,int> [1000]; //If a Tile has a stench, add all its surrounding tiles to this array
		unSafeTilesBreeze = new std::pair<int,int> [1000]; //if a Tile has a breeze, add all its surrounding tiles to this array
		tilesWithStench = new std::pair<int,int> [1000];   
		tilesWithBreeze = new std::pair<int,int> [1000];   
		lastTile = std::pair<int, int>(0, 0);
		lastTileBackUp = std::pair<int, int>(0, 0);
   		visitedTiles = new std::pair<int,int> [1000];
   		numVisited = 0;
   		numStench = 0;
   		numBreeze = 0;
		long_way_home.push(make_pair(position.first, position.second));
		
		// ==================== IMPORTANT ==================== //
		// Fill these in with your name and student ID number. //
		idNumber = "13179240";		// 8 digit ID as a string
		author = "OLIANDE, BRYAN";		// Your name in that format		
	}
	


	void MyAI::perceiveResult ( std::set<Agent::Percept> percepts )
	{
		ps = percepts;
	}
	
	// Returns true if a given percept is @ current position; otherwise false
	bool MyAI::checkPerceptIn( Percept p )
	{
		
		const bool is_in = ps.find(p) != ps.end();
		return is_in;
		
	}

	Agent::Action MyAI::getAction()
	{
		++numTurns;
		//cout << "TURN #: " << numTurns << endl;

		std::pair<int,int> start (0,0);

		if( (gold) || (numTurns > MAXTURNS) )
		{
			if(position == start)
			{
				return CLIMB;
			}
			return backTrack();
		}

		//if you arent on the same tile as last turn, add current tile to top of backtracking stack
		if( !( (position.first == long_way_home.top().first) && (position.second == long_way_home.top().second) ) )
		{
			//if you're not backtracking add it, else do nothing
			if(!gold)
			{
			long_way_home.push(make_pair(position.first, position.second));
			}
		}
	
		
		//if current tile not visited, add it to arr of visited tiles
		if( !checkIfInArray(position, visitedTiles, numVisited) )
		{
			visitedTiles[numVisited] = make_pair(position.first, position.second);
			++numVisited;
		}

		if(checkPerceptIn(Agent::BREEZE))
		{
			markTileBreeze();
		}	

		if(checkPerceptIn(Agent::STENCH))
		{
			markTileStench();
		}			

		if(!checkPerceptIn(Agent::BUMP))
		{
			lastTileBackUp.first = lastTile.first;
			lastTileBackUp.second = lastTile.second;
		}
		else
		{
			lastTile.first = lastTileBackUp.first;
			lastTile.second = lastTileBackUp.second;
			return TURN_RIGHT;
		}



		//the cave is probably too dangerous, just exit early so
		//more points aren't lost
		if( (position == start) && (numTurns > MAXTURNS))
		{
			return CLIMB;	
		}

		//if blocked in at start exit
		std::pair<int,int> rightOfStart (0,1);
		std::pair<int,int> downOfStart (1,0);	

		if( position == start)
		{
			if( checkIfInArray(rightOfStart, tilesWithBreeze, numBreeze) || checkIfInArray(rightOfStart, tilesWithStench, numStench) )	
			{
				if( checkIfInArray(downOfStart, tilesWithBreeze, numBreeze) || checkIfInArray(downOfStart, tilesWithStench, numStench) )
				{
					return CLIMB;
				}
			}
		}



		//grab gold if able to
		if ( checkPerceptIn(Agent::GLITTER) )
		{
			gold = true;
			return GRAB;
		}
		
		//if you killed the wumpus go to the tile it was on if its safe
		if(checkPerceptIn(Agent::SCREAM))
		{
			deadWumpus = true;

			if(!checkPerceptIn(Agent::BREEZE))
			{
				lastTile.first = position.first;
				lastTile.second = position.second;
				return FORWARD;
				
			}
		}

		//if the wumpus is suspected to be on the forward facing tile, it isnt dead, and the arrow has not been shot
		//this just randomly guesses tile the wumpus is on, need to fix
		if(checkPerceptIn(Agent::STENCH) && (!deadWumpus) && (notShot))
		{
			notShot = false;
			return SHOOT;
		}

	
			

		//if there's a stench and the wumpus isn't dead or breeze , go back to last tile (takes 3 turns)
		if ( (checkPerceptIn(Agent::STENCH) && (!deadWumpus)) || checkPerceptIn(Agent::BREEZE) )
		{


			//too unsafe
			if(position == start)
			{
				return CLIMB;
			}

			if( checkPerceptIn(Agent::BREEZE) )
			{
				markSurroundingTilesUnsafeBreeze();
				return aboutFace();
			}

			if( checkPerceptIn(Agent::STENCH) && (!deadWumpus) )
			{
				markSurroundingTilesUnsafeStench();
				return aboutFace();
			}
			else
			{
				//wumpus is dead, can move forward
				lastTile.first = position.first;
				lastTile.second = position.second;
				return FORWARD;
			}

			


	
			
		}

		
			
			if( !checkIfInArray(getNextMove(), visitedTiles, numVisited)  )
			{
				lastTile.first = position.first;
				lastTile.second = position.second;
				return FORWARD;
			}
			else
			{
					int action = rand() % 2 +1; //Generates number between 1 - 2
			
					switch ( action )
					{
						case 1:
							//cout << "TURN RIGHT";
							return TURN_RIGHT;
							
						case 2:
							lastTile.first = position.first;
							lastTile.second = position.second;
							//cout << "FORWARD";
							return FORWARD;
					}
			}
			
	}		
			


	Agent::Action MyAI::randomMove( void )
	{
			int action = rand() % 3 +1; //Generates number between 1 - 3
			
			//cout << "Random move function" << endl;
			switch ( action )
			{
			case 1:
				//cout << "TURN RIGHT";
				return TURN_RIGHT;
				
			case 2:
				//cout << "TURN LEFT";
				return TURN_LEFT;
				
			case 3:

				lastTile.first = position.first;
				lastTile.second = position.second;
				//cout << "FORWARD";
				return FORWARD;
			}
	}

	Agent::Action MyAI::aboutFace( void )
	{
		//cout << "Turn Around: " << turnAround << endl;
		//2 right turns to start the journey home
		if(turnAround == 0)
		{
				turnAround = 2;
				return TURN_RIGHT; 
		}

		if(turnAround == 2)
		{
				turnAround = 1;
				return TURN_RIGHT;
		}
			
		if(turnAround == 1)
		{
				turnAround = 0; //resetting
				lastTile.first = position.first;
				lastTile.second = position.second;
				return FORWARD;
		}
	}

   
   void MyAI::markSurroundingTilesUnsafeStench( void )
   {
   		//if tile isnt the last tile (which we know is safe) add it to array

   	
   		int nextX = (position.first - 1);
   		int nextY = (position.second);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesStench[numUnsafeStench] = make_pair(nextX, nextY);
   			++numUnsafeStench;  
   		}

   		nextX = (position.first);
   		nextY = (position.second - 1);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesStench[numUnsafeStench] = make_pair(nextX, nextY);  
   			++numUnsafeStench;
   		}

   		nextX = (position.first + 1);
   		nextY = (position.second);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesStench[numUnsafeStench] = make_pair(nextX, nextY);  
   			++numUnsafeStench;
   		}


   		nextX = (position.first);
   		nextY = (position.second + 1);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesStench[numUnsafeStench] = make_pair(nextX, nextY);  
   			++numUnsafeStench;
   		}

   		/*
   		if( (position.first != lastTile.first) && (position.second != lastTile.second) )
   		{
   			unSafeTilesStench[numUnsafeStench] = make_pair(position.first, position.second);
   			++numUnsafeStench;
		}*/
   } 

   void MyAI::markSurroundingTilesUnsafeBreeze( void )
   {
   		//if tile isnt the last tile add it to array
   		int nextX = (position.first - 1);
   		int nextY = (position.second);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesBreeze[numUnsafeBreeze] = make_pair(nextX, nextY);
   			++numUnsafeBreeze;  
   		}

   		nextX = (position.first);
   		nextY = (position.second - 1);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesBreeze[numUnsafeBreeze] = make_pair(nextX, nextY);  
   			++numUnsafeBreeze;
   		}

   		nextX = (position.first + 1);
   		nextY = (position.second);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesBreeze[numUnsafeBreeze] = make_pair(nextX, nextY);  
   			++numUnsafeBreeze;
   		}


   		nextX = (position.first);
   		nextY = (position.second + 1);

   		if( (nextX != lastTile.first) && (nextY != lastTile.second) )
   		{
   			unSafeTilesBreeze[numUnsafeBreeze] = make_pair(nextX, nextY);  
   			++numUnsafeBreeze;
   		}

   		/*
   		if( (position.first != lastTile.first) && (position.second != lastTile.second) )
   		{
   		unSafeTilesBreeze[numUnsafeBreeze] = make_pair(position.first, position.second);
   		++numUnsafeBreeze;
		}*/
   } 

   	bool MyAI::checkIfInArray( std::pair<int, int> Tile, std::pair<int,int> * arr, int size)
   {
   		for(int i = 0; i < size; ++i)
   		{
   			if( (Tile.first == arr[i].first) && (Tile.second == arr[i].second) )
   			{
   				// << Tile.first << "," << Tile.second << " Is in array" << endl;
   				return true;
   			}
   		}

   		//cout << Tile.first << "," << Tile.second << " Is not in array" << endl;
   		return false;
   } 

   	//gets next tile depending on which direction agent is facing
   std::pair<int, int> MyAI::getNextMove( void )
   {
   		std::pair<int, int> Tile = make_pair(position.first, position.second);
   		
   		
   		//if facing right [1,1] becomes [1,2]
   		if( (facing.first == 0) && (facing.second == 1) )
   		{
   			Tile.second = ++Tile.second;
   		}

   		//if facing down [1,1] becomes [2,1]
   		if( (facing.first == 1) && (facing.second == 0) )
   		{
   			Tile.first = ++Tile.first;
   		}

   		//if facing left [1,1] becomes [1,0]
   		if ( (facing.first == 0) && (facing.second == -1) )
   		{
   			Tile.second = --Tile.second;
   		}

   		//if facing up [1,1] becomes [0,1]
   		if ( (facing.first == -1) && (facing.second == 0) )
   		{
   			Tile.first = --Tile.first;
   		}

   		//cout << "GetnextMove: next tile should be (" << Tile.first << "," << Tile.second << ")" << endl;
   		return Tile;
   }

   //if the next move forward = to last tile go to it, else turn right until you are facing the last tile
   Agent::Action MyAI::backTrack( void )
   {

   		//cout << "Next tile should be " << long_way_home.top().first << ", " << long_way_home.top().second << endl;

	   	if(!long_way_home.empty())
	   	{

	   		if(position == long_way_home.top())
	   		{
	   			long_way_home.pop();
	   		}
	   		if(getNextMove() == long_way_home.top())
	   		{
	   			long_way_home.pop();
	   			return FORWARD;
	   		}
	   		else 
	   		{
	   			return TURN_RIGHT;
	   		}
	   	}
	   	else
	   	{
	   		//theoretically should never happen. It never happened in the thousands of worlds I
	   		//ran the agent on. This is just an emergency back-up plan.
	   		cout << "ERROR! STACK EMPTY" << endl;
	   		return randomMove();
	   	}
   }

   void MyAI::markTileBreeze( void )
   {
   		if(!checkIfInArray(position, tilesWithBreeze, numBreeze) )
   		{
   			tilesWithBreeze[numBreeze] = position;
   			++numBreeze;
   		}
   }

   void MyAI::markTileStench( void )
   {
   		if(!checkIfInArray(position, tilesWithStench, numStench) )
   		{
   			tilesWithStench[numStench] = position;
   			++numStench;
   		}
   }
}
