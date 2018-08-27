// Lab4.cpp : Defines the entry point for the console application.
//
#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <memory>
#include "GamePieces.h"
#include "HelperEnumFunc.h"
#include "GameBase.h"
#include "TicTacToe.h"
#include "Gomoku.h"

using namespace std;

int main(int argc, char * argv[])
{
	//attempt to intialize our game
	try {
		GameBase::argCheck(argc, argv);
	}
	//catch memory allocation errors
	//and errors caused by bad user input
	catch (Error e) {
		useful_message(argv[Name], e);
		return e;
	}
	catch (bad_alloc) {
		cout << "Could not allocate memory." << endl;
		return BadAlloc;
	}


	try {
		//create an easy to operate on ptr to our game and play it
		shared_ptr<GameBase> myGame(GameBase::instance());
		return (*myGame).play();
	}
	//if the instance wasnt created
	catch (Error NoInstance) {
		cout << "No GameInstance generated when calling GameBase::instance()." << endl;
		return NoInstance;
	}
	//if memory couldnt be allocated
	catch (bad_alloc) {
		cout << "Could not allocate memory." << endl;
		return BadAlloc;
	}
	return 0;
}

