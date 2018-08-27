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
#include "GameBase.h"
#include "TicTacToe.h"


using namespace std;
//see Gomoku initialization
TicTacToeGame::TicTacToeGame() : GameBase() {
	fileName = "TicTacToe.txt";

	bool wantLoad = isRightFormat();

	if (!wantLoad) {
		dimen_x = 5, dimen_y = 5, playerTurn = PlayerX, lastPlayer = PlayerO, longString = getlongstring(), movesToWin = 3, moveCount = 0;
	}
	boardArrayStart(wantLoad, fileName);
}


std::ostream& operator<<(std::ostream &o, const TicTacToeGame &board) {

	//printMax+2 is to simply make our smallest buffer 1
	int printMax = board.longString + 2;
	int currentBuffer;
	int leftBuffer;
	int rightBuffer;

	//for every row on the gameboard starting at the top ydim = board rows
	for (size_t ydim = board.dimen_y - 1; ydim > 0; --ydim) {

		if (ydim != 1) {
			//prints y coordinates
			cout << ydim - 1 << " |";
		}
		else {
			cout << "   ";
		}

		//for every space in that row
		for (size_t xdim = 1; xdim < board.dimen_x - 1; ++xdim) {

			if (ydim == 1) {
				//prints x coordinates
				currentBuffer = printMax - 1;
				leftBuffer = currentBuffer / 2;
				rightBuffer = currentBuffer - leftBuffer;
				cout << setw(leftBuffer) << xdim << setw(rightBuffer) << " ";
				continue;
			}

			//left buffer + right buffer should equal currentBuffer
			currentBuffer = printMax - board.boardArray[ydim - 1][xdim].printType.length();
			leftBuffer = currentBuffer / 2;
			rightBuffer = currentBuffer - leftBuffer;

			//display the information held by that space's unique gamepiece
			//using the ostream operator defined in GamePiece.cpp
			o << setw(leftBuffer) << board.boardArray[ydim - 1][xdim] << setw(rightBuffer) << "|";
		}
		//proceed to next row
		o << endl;
	}
	return o;
}

void TicTacToeGame::print() {
	cout << (*this);
	return;
}

//checks doneness through recursive search. Goes through worst case 4*a*b operations
bool TicTacToeGame::done() {
	int tracker = 0;
	for (unsigned int b = dimen_y; b >= 1; --b) {
		for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
			if (checkRight(a, b - 1, tracker, invalid)) {
				cout << "Horizontal Victory";
				return true;
			}
			if (checkDown(a, b - 1, tracker, invalid)) {
				cout << "Vertical Victory";
				return true;
			}
			if (checkDR(a, b - 1, tracker, invalid)) {
				cout << "DiagonalR Victory";
				return true;
			}
			if (checkDL(a, b - 1, tracker, invalid)) {
				cout << "DiagonalL Victory";
				return true;
			}
		}
	}
	//resets visited for all the pieces for the next time done() is called
	for (unsigned int b = 0; b <= dimen_y - 1; ++b) {
		for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
			(*this).boardArray[b][a].resetVisits();
		}
	}
	return false;
}

//Checks if the game is a draw
//always possible for someone to win until the last move so we don't bother
//being as rigorous as with gomoku
bool TicTacToeGame::draw() {

	//if there is still a move to be played, and the .done() does not say there are three pieces in a row
	if ((*this).moveCount < 9 || (*this).done()) {

		//the game is not a draw
		return false;
	}
	//otherwise there are no moves to be played and no one has 3 in a row
	return true;
}

//returns boolean true if player wants to save
//and saves if they do
int TicTacToeGame::savePrompt() {

	ofstream myOutput("TicTacToe.txt");
	if (!myOutput) {
		cout << "Could not open output file!" << endl;
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

	cout << endl << "Would you like to save? (Y/N)" << endl;
	if (yesOrNo()) {

		myOutput << dimen_x << " " << dimen_y << " " << playerTurn << " " << lastPlayer << " " << movesToWin << " " << moveCount << endl;

		for (size_t ydim = 0; ydim < dimen_y; ++ydim) {
			for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
				myOutput << boardArray[ydim][xdim].color << " ";
			}
			myOutput << endl;
		}
		return UserSave;
	}
	else {
		myOutput << "NO DATA";
		return UserQuit;
	}
	
}

//method to process one players turn
//ONLY RETURNS SUCCESS OR QUIT crucial to .play() method
int TicTacToeGame::turn() {
	//general loop variables and value storage variables
	bool isLooping = true;
	unsigned int a;
	unsigned int b;
	unsigned int c;
	//stores which players turn it is in playerIndex 
	// .playerTurn returns an int from enum Player
	unsigned int playerIndex = (*this).playerTurn;

	string playerTurn = myPlayers[playerIndex];

	//Creates a gamepiece for the player with that players color, name, and printing symbol
	GamePiece playerPiece = GamePiece(playerColors[playerIndex], myPlayers[playerIndex], playerSymbols[playerIndex]);

	cout << "It is " << playerTurn << "'s turn!" << endl;


	while (isLooping) {
		//If the user does not type 'quit' as their input, stores the users input in a and b
		try { prompt(a, b, c); }
		catch (Error) {
			return savePrompt();
		}
		//if the the requested space has already been taken
		if (boardArray[b][a].color != invalid) {

			//repromt the user by going to the next iteration of the loop
			cout << "This space is already taken!" << endl << endl;
			continue;
		}

		//otherwise the users input was valid, makes players move and stops while loop
		else {
			boardArray[b][a] = playerPiece;
			isLooping = false;

		}
	}

	//logic for allowing the next player to make their turn. 
	(*this).lastPlayer = (*this).playerTurn;

	//sets the next move to the other player. Since there are only two players it cycles
	(*this).playerTurn = (Player)((playerIndex + 1) % myPlayers.size());

	//player successfully made their move
	return Success;
}