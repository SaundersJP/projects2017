#include "stdafx.h"
#include <vector>
#include <string>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <memory>
#include "GamePieces.h"
#include "HelperEnumFunc.h"

//note to self, dependent header files are always
//included after the header they are dependent on
#include "GameBase.h"
#include "TicTacToe.h"
#include "Gomoku.h"
#include "SudokuGame.h"

using namespace std;

shared_ptr<GameBase> GameBase::gamePointer = nullptr;

//checks input file format for tictactoe and gomoku
//not effective to attempt to do the convouluted sudoku parsing this way
bool GameBase::isRightFormat() {
	ifstream myInput;
	string myString;
	bool parsed = false;
	bool wantLoad = false;
	int x;
	int y;
	int currentTurn;
	int lastTurn;
	int toWin;
	int currentMoves;
	myInput.open(fileName);

	//if we can open file
	if (myInput) {

		//grab some data
		myInput >> x >> y >> currentTurn >> lastTurn >> toWin >> currentMoves;
		if (myInput.fail()) {
			cout << "Improper save file format. Starting new game now!" << endl;
		}
		else if (x < 0 || y < 0 || currentTurn < 0 || lastTurn < 0 || toWin < 0 || currentMoves < 0){
			cout << "Improper save file format. Starting new game now!" << endl;
		}
		else if (!myInput.fail()) {
			//if we were successful
			int count = 0;
			int checker;
			int tempInt;
			string dummyString;
			bool isAnumber = true;
			while (myInput >> dummyString) {
				try { tempInt = stoi(dummyString); }
				catch (exception) {
					isAnumber = false;
					break;
				}
				checker = (signed)tempInt;
				if (checker < 0 || checker >(int)invalid) {
					isAnumber = false;
					break;
				}
				++count;
			}
			if (count == x*y && isAnumber) {
				parsed = true;
			}
			else {
				cout << "Improper save file format. Starting new game now!" << endl;
			}
		}
		else {
			cout << "Improper save file format. Starting new game now!" << endl;
		}
	}

	if (parsed) {
		cout << "Saved game detected. If you do not load, the saved game will be overwritten. Do you wish to load it?" << endl;
		if (yesOrNo()) {
			wantLoad = true;
		}
	}
	if (wantLoad) {
		cout << "Loading saved game!" << endl << endl;
		dimen_x = x, dimen_y = y, playerTurn = (Player)currentTurn, lastPlayer = (Player)lastTurn, longString = getlongstring(), movesToWin = toWin, moveCount = currentMoves;
	}
	return wantLoad;
}

//MODIFY THE ABOVE TO CHANGE PLAYERS
//EVENTUALLY INCORPORATE IT INTO BASE CLASS
void GameBase::addPlayer(GamePiece &myPiece) {
	gamePlayers.push_back(myPiece);
	if (myPiece.printType.length() > longString) {
		longString = myPiece.printType.length();
	}
	return;
}


//Helper function thatnitializes the vector of vectors that is our gameboard
//accomplishes this by pushing gamepieces vectors of size xdim
//into the TicTacToeGame.boardArry vector that has size ydim

void GameBase::boardArrayStart(bool loading, string fileName) {
	vector<GamePiece> storageVector;
	piece_color tempColor;
	ifstream myInput;
	string dummyLine;
	int index;
	//loop for pushing into .boardArray

	//dont have to worry about checking any of our operations as 
	//boardArrayStart is only called during constructors after checks have been made
	//to ensure everything opens and is formatted correctly
	myInput.open(fileName);
	getline(myInput, dummyLine);

	for (size_t ydim = 0; ydim < (*this).dimen_y; ++ydim) {

		//creates the gamepieces vectors of size xdim
		//uses invalid color to make empty spaces identifiable
		for (size_t xdim = 0; xdim < (*this).dimen_x; ++xdim) {
			if (loading) {
				myInput >> index;
				tempColor = (piece_color)index;
				if (tempColor != invalid) {
					storageVector.push_back(GamePiece(tempColor, myPlayers[index], playerSymbols[index]));
				}
				else {
					storageVector.push_back(GamePiece(invalid, "", " "));
				}
			}
			else {
				storageVector.push_back(GamePiece(invalid, "", " "));
			}
		}
		(*this).boardArray.push_back(storageVector);
		storageVector.clear();
	}
	myInput.close();
	return;
}

//prompts the player to move and requests properly formatted imput in the form of coordinates x,y
//cin.sync is used to clear input stream

void GameBase::prompt(unsigned int &m, unsigned int &n, unsigned int &p) {
	//loop variable and general storage variables
	bool isLooping = true;
	string myString;
	string tooMany;
	unsigned int value1;
	unsigned int value2;
	unsigned int value3;

	while (isLooping) {
		cout << "Please input coordinates in the form 'a,b' or type 'quit'." << endl;

		//requests user for coordinate in the for x,y
		//ignores whitespace characters before non whitespace input
		

		getline(cin, myString);

		lowerCaseIt(myString);

		//return a unique case for a user quitting via 'quit'
		if (myString == "quit") {
			throw UserQuit;
		}

		//if the users input was the correct length.
		else if (myString.length() >= 3 &&
			myString.length() <= (numberOfDigits(dimen_x) + numberOfDigits(dimen_x) + 1)) {

			size_t comma = myString.find(",");
			//if ((isdigit(myString[0])) && (isdigit(myString[2])) && (myString[1] == ',')) {
			if (comma != -1) {

				int counter = 0;
				int spaces = 0;
				myString[comma] = ' ';
				for (size_t i = 0; i < myString.length(); ++i) {
					if (isdigit(myString[i])) {
						counter++;
					}
					else if (isspace(myString[i])) {
						counter++;
						spaces++;
					}
				}
				if (counter != myString.length() || spaces != 1) {
					cout << "Improper input format! Please follow instructions!" << endl << endl;
					continue;
				}

				//wraps the input in a string stream and uses the extraction operator to store the input in value1 and value2
				istringstream wrappedString(myString);
				wrappedString >> value1 >> value2;

				//if the user input was outside the playing area
				if (value1 < 1 ||
					value1 > dimen_x ||
					value2 < 1 ||
					value2 > dimen_y) {

					//Proceed to next interation of while (isLooping) and request input in the playing area
					cout << "Coordinates out of range. Please limit your coordinates to the game board" << endl << endl;
					continue;
				}
				//otherwise the input has met all the necessary conditions to be a valid input
				//stores value1 and value two in unsigned ints m and n that were passed by reference
				m = value1;
				n = value2;

				if (Sudoku) {
					try{ sudokuPrompt(value3); }
					catch (Error e) { throw e; }
					p = value3;
				}
				//set loop variable to false and iterate while loop to exit
				isLooping = false;
				continue;
			}
		}

		//If the users input was any other string of length != 3, proceed to next iteration of while loop requesting proper formatting
		cout << "Improper input format! Please follow instructions!" << endl << endl;
	}
	return;
}


int GameBase::play() {


	cout << endl;
	print();
	//continually request user input until quit statement or game is finished
	while (true) {
		//check the board to see if that move made 3 in a row
		if (done()) {
			isDone = true;
			//if so a player has won the game and we need to return success
			//Checks lastPlayer because the last player to make a move 
			cout << endl << endl << "**************************************" << endl;
			print();
			cout << myPlayers[(*this).lastPlayer] << " won the game with " << (*this).moveCount << " moves played." << endl;
			savePrompt();
			return Success;
		}
		//check draw condition as we can load in full board
		if (draw()) {
			isDraw = true;
			cout << endl << endl << "**************************************"<<  endl;
			print();
			cout << "The game was a draw." << endl;
			savePrompt();
			return Draw;
		}
		//if the players typed quit as their input. Proper format is insured by .turn() method
		int currentTurnReturn = (*this).turn();
		if (currentTurnReturn) {
			//print how many tuns elapsed and returns quit value
			cout << "The game was quit with " << moveCount << " moves played." << endl;
			return currentTurnReturn;
		}
		//Otherwise the player made a valid move, increment our move tracker
		moveCount++;
		cout << endl;
		print();

	}


	//Since the above while loop never exits, if we get to this point there has been some massive error
	//I do not believe this state can actually be hit
	cout << "Massive error";
	return MassiveError;
}

//returns pointer to game
//if no pointer throws exception
shared_ptr<GameBase> GameBase::instance() {
	if (GameBase::gamePointer == nullptr) {
		throw NoInstance;
	}
	return GameBase::gamePointer;
}

//does all the argument checking
void GameBase::argCheck(int c, char * v[]) {
	//checks if no arguments submitted
	if (GameBase::gamePointer != nullptr) {
		throw InstancePresent;
	}

	if (c == 1) {
		throw NoArgs;
	}

	string myString;
	myString = v[1];
	lowerCaseIt(myString);

	//if some were submitted and the first string is tictactoe
	if (myString == "tictactoe") {
		//if tictactoe was not only thing submitted
		if (c != ArgLengthStd) {
			throw TicTooMany;
		}
		shared_ptr<GameBase> managed(new TicTacToeGame());
		gamePointer = managed;
		//managed is removed but gamePointer keeps object alive
		return;
	}
	else if (myString == "gomoku") {
		//if gomoku only thing submitted
		if (c == ArgLengthStd) {
			shared_ptr<GameBase> managed(new Gomoku(19,19, 5));
			gamePointer = managed;
			return;
		}
		//if user supplied number in a row for win
		else if (c == ArgLength1) {
			int n;
			istringstream wrapped(v[ArgLength1 - 1]);
			wrapped >> n;
			if (wrapped.fail()) {
				throw GoNotInt1;
			}
			else if (n <= 0) {
				throw GoNeg1;
			}
			shared_ptr<GameBase> managed(new Gomoku(19, 19, n));
			gamePointer = managed;

			return;
		}
		//if user supplied size and number for win
		else if (c == ArgLength2) {
			int m;
			int n;
			istringstream wrapped1(v[ArgLength1 - 1]);
			istringstream wrapped2(v[ArgLength2 - 1]);
			wrapped1 >> m;
			wrapped2 >> n;
			//if m or n were not integers
			if (wrapped1.fail() || wrapped2.fail()) {
				throw GoNotInt2;
			}
			// if the were check if they are non-zero positives
			else if (n <= 0 || m <= 0) {
				throw GoNeg2;
			}
			shared_ptr<GameBase> managed(new Gomoku(n, n, m));
			gamePointer = managed;
			return;
		}
		else {
			throw GoTooMany;
		}
	}
	else if (myString == "sudoku"){
		if (c != ArgLengthStd) {
			throw SudTooMany;
		}
		shared_ptr<GameBase> managed(new SudokuGame(9,9));
		gamePointer = managed;
		//managed is removed but gamePointer keeps object alive
		return;
	}
	else {
		throw BadName;
	}
}

//Helper functions to find winning states
//uses directional recursion to find out if there are the correct number
//of pieces in a row
//Only checkright is seriously commented. Others follow same format with different coordinates being checked
bool GameBase::checkRight(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != checkAgainst) {
		//makes not have to movesToWin*a*b calls of checkRight, forces it to a*b worst case
		if (boardArray[b][a].visitedRight) {
			return false;
		}

		boardArray[b][a].visitedRight = true;
		storage += 1;
		//if we have found enough moves in a row
		if (storage == movesToWin) {
			return true;
		}
		//if we are on edge
		else if (a >= (dimen_x - 1)) {
			return false;
		}
		//if the piece to the right is from the same player
		//continue checking in that direction
		else if (boardArray[b][a].name == boardArray[b][a + 1].name) {
			return checkRight(a + 1, b, storage, checkAgainst);
		}
	}
	return false;
}
bool GameBase::checkDown(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != checkAgainst) {

		if (boardArray[b][a].visitedDown) {
			return false;
		}

		boardArray[b][a].visitedDown = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (b <= 0) {
			return false;
		}
		else if (boardArray[b][a].name == boardArray[b - 1][a].name) {
			return checkDown(a, b - 1, storage, checkAgainst);
		}
	}
	return false;

}
bool GameBase::checkDR(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != checkAgainst) {

		if (boardArray[b][a].visitedDR) {
			return false;
		}

		boardArray[b][a].visitedDR = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (a >= (dimen_x - 1) || b <= 0) {
			return false;
		}
		else if (boardArray[b][a].name == boardArray[b - 1][a + 1].name) {
			return checkDR(a + 1, b - 1, storage, checkAgainst);
		}
	}
	return false;

}
bool GameBase::checkDL(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != invalid) {

		if (boardArray[b][a].visitedDL) {
			return false;
		}

		boardArray[b][a].visitedDL = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (a <= 0 || b <= 0) {
			return false;
		}
		
		else if (boardArray[b][a].name == boardArray[b - 1][a - 1].name) {
			return checkDL(a - 1, b - 1, storage, checkAgainst);
		}
	}
	return false;
}

//the drawDirection functions are the exact same as the check functions
//but include whitespace next to pieces when trying to see if there is a draw.
bool GameBase::drawRight(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != drawAgainst) {
		//makes not have to movesToWin*a*b calls of drawRight, forces it to a*b worst case
		if (boardArray[b][a].visitedRight) {
			return false;
		}

		boardArray[b][a].visitedRight = true;
		storage += 1;
		//if we have found enough moves in a row
		if (storage == movesToWin) {
			return true;
		}
		//if we are on edge
		else if (a >= (dimen_x - 1)) {
			return false;
		}
		//if the piece to the right is from the same player
		//continue drawing in that direction
		if (boardArray[b][a + 1].color != drawAgainst) {
			return drawRight(a + 1, b, storage, drawAgainst);
		}
		else if (boardArray[b][a].name == boardArray[b][a + 1].name) {
			return drawRight(a + 1, b, storage, drawAgainst);
		}
	}
	return false;
}
bool GameBase::drawDown(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != drawAgainst) {

		if (boardArray[b][a].visitedDown) {
			return false;
		}

		boardArray[b][a].visitedDown = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (b <= 0) {
			return false;
		}
		if (boardArray[b - 1][a].color != drawAgainst) {
			return drawDown(a, b - 1, storage, drawAgainst);
		}
		else if (boardArray[b][a].name == boardArray[b - 1][a].name) {
			return drawDown(a, b - 1, storage, drawAgainst);
		}
	}
	return false;

}
bool GameBase::drawDR(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != drawAgainst) {

		if (boardArray[b][a].visitedDR) {
			return false;
		}

		boardArray[b][a].visitedDR = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (a >= (dimen_x - 1) || b <= 0) {
			return false;
		}
		if (boardArray[b - 1][a + 1].color != drawAgainst) {
			return drawDR(a + 1, b - 1, storage, drawAgainst);
		}
		else if (boardArray[b][a].name == boardArray[b - 1][a + 1].name) {
			return drawDR(a + 1, b - 1, storage, drawAgainst);
		}
	}
	return false;

}
bool GameBase::drawDL(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst) {

	int storage = tracker;

	if (boardArray[b][a].color != invalid) {

		if (boardArray[b][a].visitedDL) {
			return false;
		}

		boardArray[b][a].visitedDL = true;
		storage += 1;

		if (storage == movesToWin) {
			return true;
		}
		else if (a <= 0 || b <= 0) {
			return false;
		}

		if (boardArray[b][a].name == boardArray[b - 1][a - 1].name) {
			return drawDL(a - 1, b - 1, storage, drawAgainst);
		}
		if (boardArray[b - 1][a - 1].color != drawAgainst) {
			return drawDL(a - 1, b - 1, storage, drawAgainst);
		}

	}
	return false;
}