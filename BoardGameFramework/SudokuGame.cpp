#include "stdafx.h"
#include <vector>
#include <unordered_map>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include "GamePieces.h"
#include "HelperEnumFunc.h"
#include "GameBase.h"
#include "SudokuGame.h"

using namespace std;

//this is a mess because parsing the file gets out of hand very quickly
//lots of checking to make sure things go right and seperating it
//into helper functions require me to do some even dumber things
SudokuGame::SudokuGame(unsigned int a, unsigned int b) : GameBase() {
	dimen_x = a;
	dimen_y = b;
	lastPlayer = PlayerX;
	playerTurn = PlayerX;
	//global variable Sudoku in helper.cpp
	//enables the 3 variable prompt method
	// once sudokugame is intialized
	Sudoku = 10;
	fileName = "SudokuGame.txt";

	//initializes board containing starter locations
	intBoardStart();

	ifstream myInput;
	string myFile = fileName;
	bool parsed = false;
	bool wantLoad = true;
	int count = 0;
	myInput.open(myFile);
	int loadedMoves = 0;
	int startingMoves = 0;
	int xStart;
	int yStart;

	if(myInput){ 
		myInput >> loadedMoves >> startingMoves;
		//checks if we were successful loading number of moves
		//and number of starting pieces
		if (myInput.fail()) {
			cout << "Improper save file format. Fetching new game." << endl;
		}
		else if (loadedMoves < 0 || startingMoves < 0) {
			cout << "Improper save file format. Fetching new game." << endl;
			
		}
		//if we didnt fail
		else if (!myInput.fail()) {
			moveCount = loadedMoves;
			startingNumbers = startingMoves;
			string dummyString;
			bool isAnumber = true;

			//While we still have startingpieces left
			while (startingMoves != 0) {
	
				myInput >> yStart >> xStart;
				if (myInput.fail()) {
					isAnumber = false;
					break;
				}
				else if (yStart < 0 || xStart < 0 || yStart > 9 ||xStart > 9) {

					isAnumber = false;
					break;
				}
				--startingMoves;
				//set positions equal to one for our "starter" board
				//will lock a spot out if coords are incorrect
				startingPositions[yStart][xStart] = 1;
			}

			while (myInput >> dummyString) {
				int myInt;
				int tempInt;
				//attempts to turn the string into an int
				try { tempInt = stoi(dummyString); }
				catch (exception) {
					//if it fails improper format
					isAnumber = false;
					break;
				}
				//test the sign and magnitude of the obtained int
				myInt = (signed)tempInt;
				if (myInt < 0 || myInt > 9) {
					//if larger than a digit or negativeimproper format
					isAnumber = false;
					break;
				}
				//if was a nonnegative digit count it
				++count;
			}
			//if count is equal to our board dimensions and proper format
			if (count == a*b && isAnumber) {
				parsed = true;
			}
			else { 
				cout << "Improper save file format. Fetching new game." << endl; 
			}
		}
	}
	myInput.close();
	if (parsed) {
		cout << "Saved game detected, would you like to load it?" << endl;
		//if they dont want to load saved game set file name to new board
		if (!yesOrNo()) {
			//if they dont want to load
			myFile = "sudoku0.txt";
			moveCount = 0;
			startingNumbers = 0;
		}
	}
	else {
		//improper format
		moveCount = 0;
		startingNumbers = 0;
		myFile = "sudoku0.txt";
	}
	boardArrayStart(wantLoad, myFile);
}

//start board vector that holds location of starting points
void SudokuGame::intBoardStart() {
	vector<int> intStorageVector;
	for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
		for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
			intStorageVector.push_back(0);
		}
		startingPositions.push_back(intStorageVector);
		intStorageVector.clear();
	}
	return;
}

//initializes board based on the success of the constructor trying to parse
//SudokuGame.txt
void SudokuGame::boardArrayStart(bool loading, string fileName) {
	vector<GamePiece> storageVector;
	ifstream myInput;
	string intString;
	int pieceInt;
	myInput.open(fileName);

	//if our file name is sudoku game loop like a mad man until we get to the board
	if (fileName == "SudokuGame.txt") {
		string myDummy;
		for(size_t i = 0; i <= startingNumbers; ++i){
			getline(myInput, myDummy);
		}
	}
	//if not clear starting positions because we will add them manually from sudoku0
	//as the constructor intially attempts to set them
	else {
		startingPositions.clear();
		intBoardStart();
	}

	for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
		for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
			myInput >> pieceInt;
			//if piece is open
			if (pieceInt == 0) {
				storageVector.push_back(GamePiece(invalid, "0", " "));
			}
			else {
				piece_color myColor = black;
				intString = to_string(pieceInt);
				if (startingPositions[ydim][xdim]) {
					myColor = red;
				}
				storageVector.push_back(GamePiece(myColor, intString, intString));
				//If our file name is sudoku0 and we are placing a piece
				//from the file, we need to include it in the starting positions
				if (fileName == "sudoku0.txt") {
					++startingNumbers;
					startingPositions[ydim][xdim] = 1;
				}
			}
		}
		boardArray.push_back(storageVector);
		storageVector.clear();
	}
	myInput.close();
	return;
}

//see gomoku and tictactoe ostream operator
//explained already and this is way simpler
std::ostream& operator<<(std::ostream &o, const SudokuGame &board) {
	string prettyString = "-";
	for (size_t ydim = board.dimen_y; ydim > 0; --ydim) {

		o << ydim << "||  ";
		for (size_t xdim = 0; xdim < board.dimen_x; ++xdim) {
			o << board.boardArray[ydim - 1][xdim];
			if ((xdim + 1) % 3 == 0) {
				o << "||";
			}
			else {
				o << "|";
			}
		}
		o << endl << "-||  ";
		
		if ( (ydim + 2 ) % 3 == 0) {
			prettyString = "=";
		}
		else {
			prettyString = "-";
		}
		for (size_t xdim = 0; xdim < board.dimen_x; ++xdim) {
			o << prettyString;
			if ((xdim + 1) % 3 == 0) {
				o << "||";
				if (xdim == 8) {
					o << endl;
				}
			}
			else {
				o << "|";
			}
		}
	}

	o << endl << "#||  ";
	for (size_t xdim = 0; xdim < board.dimen_x; ++xdim) {
		o << xdim + 1;
		if ((xdim + 1) % 3 == 0) {
			o << "||";
		}
		else {
			o << "|";
		}
	}
	o << endl;
	return o;
}

void SudokuGame::print() {
	cout << (*this);
}

//simply returns tru if there is an empty game space
bool SudokuGame::spacesLeft() {
	for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
		for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
			if (boardArray[ydim][xdim].color == invalid) {
				return true;
			}
		}
	}
	return false;
}

//see gomoku and tictactoe method definitions
bool SudokuGame::done() {
	bool isSpace = spacesLeft();
	//checks every row
	for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
		if (rowCheck(ydim)) {
			continue;
		}
		if (!isSpace) {
			cout << "Your solution is incorrect!" << endl;
		}
		return false;
	}

	//checks every column
	for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
		if (columnCheck(xdim)) {
			continue;
		}
		if (!isSpace) {
			cout << "Your solution is incorrect!" << endl;
		}
		return false;
	}
	//checks every square
	//all 9 with two 3 loop nested for loops
	for (size_t xdim = 0; xdim <= 2; ++xdim) {
		for (size_t ydim = 0; ydim <= 2; ++ydim) {
			if (squareCheck(ydim * 3, xdim * 3)) {
				continue;
			}
			if (!isSpace) {
				cout << "Your solution is incorrect!" << endl;
			}
			return false;
		}
	}
	return true;
}

//always returns false even though you can 'draw' a sudoku game with utensils ;)
bool SudokuGame::draw() {
	return false;
}

int SudokuGame::turn() {
	bool isLooping = true;
	unsigned int a;
	unsigned int b;
	unsigned int c;
	string intString;
	string printString;

	//user pieces are black unless placing an empty space
	piece_color myColor = black;

	while (isLooping) {
		//attempts the three variable prompt
		try {
			prompt(a, b, c);
		}
		//while it isn't immediately apparent, this continues unwinding the stack
		//only unwinds when user types 'quit'
		catch (Error) {
			return savePrompt();
		}
		//if it is starting location ask them for another one
		if (boardArray[b - 1][a - 1].color == red) {
			cout << "That is the location of one of the starting numbers, please pick a new coordinate!" << endl << endl;
			continue;
		}
		else {
			isLooping = false;
		}
	}

	//put the number on the board
	intString = to_string(c);
	printString = intString;
	if (c == 0) {
		myColor = invalid;
		printString = " ";
	}
	GamePiece playerPiece = GamePiece(myColor, intString, printString);
	boardArray[b - 1][a - 1] = playerPiece;
	return Success;
}

//see gomoku savePrompt for beginning
int SudokuGame::savePrompt() {
	ofstream myOutput("SudokuGame.txt");

	if (!myOutput) {
		cout << "Unable to save!" << endl;
		return NoOutput;
	}

	if (isDone) {
		myOutput << "NO DATA";
		return Success;
	}

	if (isDraw) {
		myOutput << "NO DATA";
		return Draw;
	}

	cout << endl <<  "Would you like to save? (Y/N)" << endl;
	if (yesOrNo()) {

		//puts number of moves and #startingDigits in first two spots
		myOutput << moveCount << " " << startingNumbers << endl;

		//put the coordinates of all starting pairs in the next
		//#startingDigits spots
		for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
			for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
				if (startingPositions[ydim][xdim]) {
					myOutput << ydim << " " << xdim << endl;
				}
			}
		}
		//place gameboard in file based on digits stored at loc
		for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
			for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
				myOutput << boardArray[ydim][xdim].name << " ";
			}
			myOutput << endl;
		}
		return UserSave;
	}
	else {
		myOutput << "NODATA";
		return UserQuit;
	}
}



//probably should have used sets instead of unordered maps for check functions
//all of these functions check either a row, column or square
//they do this by adding every element that isnt of invalid color
//to a hashmap and if the size is 9 digits then there weren't
//any repeats and the functions should return true.
//we only actually need two check functions 
//but I don't want to try the proof for that at the moment 
bool SudokuGame::rowCheck(unsigned int row) {
	unordered_map<string, string> myHash; 

	for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
		GamePiece currentPiece = boardArray[row][xdim];
		if (currentPiece.color != invalid) {
			myHash.insert({ currentPiece.printType, currentPiece.printType });
		}
	}
	if (myHash.size() < dimen_x) {
		return false;
	}
	return true;
}

bool SudokuGame::columnCheck(unsigned int column) {
	unordered_map<string, string> myHash;

	for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
		GamePiece currentPiece = boardArray[ydim][column];
		if (currentPiece.color != invalid) {
			myHash.insert({ currentPiece.printType, currentPiece.printType });
		}
	}
	if (myHash.size() < dimen_x) {
		return false;
	}
	return true;
}

bool SudokuGame::squareCheck(unsigned int row, unsigned int column) {
	unordered_map<string, string> myHash;
	
	//check 3x3 area with two for loops
	for (size_t ydim = row; ydim < row + 3; ++ydim) {
		for (size_t xdim = column; xdim < column + 3; ++xdim) {

			GamePiece currentPiece = boardArray[ydim][xdim];
			if (currentPiece.color != invalid) {
				myHash.insert({ currentPiece.printType, currentPiece.printType });
			}
		}
	}
	if (myHash.size() < dimen_x) {
		return false;
	}
	return true;
}