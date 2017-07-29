#ifndef TILE_LOCK
#define TILE_LOCK

#include <string>

namespace wumpus_project
{
	class Tile
	{
		// The state of each square of the board
	public:
		Tile() : pit(false), wumpus(false), deadWumpus(false), gold(false), breeze(false), stench(false), agent(false) {}

		bool pit;
		bool wumpus;
		bool deadWumpus;
		bool gold;
		bool breeze;
		bool stench;
		bool agent;
		
		std::string stringTile()
		{
			std::string result(".");
			if (pit)
				result.append("P");
			if (wumpus)
				result.append("W");
			if (deadWumpus)
				result.append("D");
			if (gold)
				result.append("G");
			if (breeze)
				result.append("B");
			if (stench)
				result.append("S");
			if (agent)
				result.append("@");
			return result;
		}
	};
}
#endif /* TILE_LOCK */
