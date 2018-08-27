#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>
#include "GamePieces.h"
#include "HelperEnumFunc.h"

using namespace std;

//global variables used by all GAMEBASE objects
vector<string> myPlayers{ "Player X", "Player O" };
vector<string> playerSymbols{ "X", "O" };
vector<piece_color> playerColors{ red, black };
unsigned int Sudoku = 0;
//----------------------------------------------------------

int useful_message(char program_name[], Error e) {
	cout << endl << "Error " << e << ":" << endl;
	cout << "Improperly formatted command line arguments. This can be caused by an incorrect number of arguments or negative numbers being used" << endl << "Usage: Lab4.exe TicTacToe" << endl <<
		"Usage: Lab4.exe Gomoku" << endl << "Usage: Lab4.exe Gomoku *number of move to win*" << endl << "Usage: Lab4.exe Gomoku *board size* *number of moves to win*" << endl << "Usage: Lab4.exe Sudoku" << endl;
	return NoArgs;
}

void lowerCaseIt(string &randomString) {
	size_t check = 0;
	while (check < randomString.size()) {
		randomString[check] = tolower(randomString[check]);
		++check;
	}
	return;
}

//used by print functions of GameBase derivative classes to determine number of digits board displays for coordinate axis
unsigned int numberOfDigits(int x) {
	int needsChecked = x;
	unsigned int length = 1;
	while (needsChecked /= 10) {
		length++;
	}
	return length;
}

//helper function used by GameBase constructor
//automatically caluclates max length of any players printSymbol
//players are defined by 3 vectors above
unsigned int getlongstring() {
	unsigned int lengthMax = 0;
	for (size_t i = 0; i <= (playerSymbols.size() - 1); ++i)
		if (playerSymbols[i].size() > lengthMax) {
			lengthMax = playerSymbols[i].size();
		}
	return lengthMax;
}

//asks the yes or no, returns true on yes
bool yesOrNo() {
	string myString;
	while (true) {
		cout << "Please enter 'Y' for yes or 'N' for no." << endl;
		getline(cin, myString);
		lowerCaseIt(myString);
		if (myString == "y") {
			return true;
		}
		else if (myString == "n") {
			return false;
		}
		cin.sync();
	}
}

//sets 3rd value of GameBase::prompt(&a,&b,&C) when
//a sudoku game objects calls prompt
//kept here because it isn't really a method
//more just an operation with some user input that only needs
//called conditionally
void sudokuPrompt(unsigned int &z) {
	string myString;
	int value1;

	while (true) {
		cout << "Please input a digit to place in the space or type 'quit' to exit!" << endl;
		getline(cin, myString);
		lowerCaseIt(myString);

		if (myString == "quit") {
			throw UserQuit;
		}
		if (myString.length() == 1) {
			istringstream wrappedString(myString);
			wrappedString >> value1;
			if (wrappedString.fail()) {
				wrappedString.clear();
				continue;
			}
			z = value1;
			return;
		}
	}
}