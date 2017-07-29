// main.cpp

#include "World.hpp"
#include <iostream>

using namespace std;
using namespace wumpus_project;

void flushInput ( istream &inStream = std::cin )
{
	inStream.ignore( 999, '\n' );
}

char getChar ( string prompt )
{
	do
	{
		cout << prompt;

		char userAnswer;
		cin >> userAnswer;
		flushInput();

		if ( !cin.fail() )
			return userAnswer;

		cin.clear();
		flushInput();
		cout << "[Input Format Error] Please enter one character." << endl;

	} while ( true );
}

bool getBool ( string prompt )
{
	do
	{
		switch ( getChar(prompt) )
		{
		case 'y':
		case 'Y':
			return true;
		case 'n':
		case 'N':
			return false;
		}

		cout << "[Input Format Error] Please enter y or n." << endl;

	} while ( true );
}

int getBoundedInt ( string prompt, int lowerBound, int upperBound )
{
	do
	{
		cout << prompt;

		int userAnswer;
		cin >> userAnswer;
		flushInput();

		if ( !cin.fail() )
			if ( userAnswer >= lowerBound && userAnswer <= upperBound )
				return userAnswer;

		cin.clear();

		cout << "[Input Format Error] Please enter an integer from "
			 << lowerBound 
			 << " to "
			 << upperBound
			 << "."
			 << endl;

	} while ( true );
}

int main ( int argc, char *argv[] )
{
	if ( argc >= 2 )	// Check if the program was fed a filename through the command line.
	{
		World gameWorld( argv[1] );
		gameWorld.run();
		if ( argc == 3 )
			gameWorld.outputGameInfo( argv[2] );
		else
			gameWorld.outputGameInfo();
		return 0;
	}
	
	// Test your AI on a random board
	srand( time(NULL) );	// Set random seed
	World gameWorld( getBoundedInt ( "Enter dimension of the game board [2-8]: ", 2, 8 ) );
	gameWorld.setStepByStep ( getBool ( "Would you like to take the game step by step? [y/n]: " ) );
	gameWorld.run();
	gameWorld.printGameInfo();
	return 0;
}