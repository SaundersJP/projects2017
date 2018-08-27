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
#include "Gomoku.h"

using namespace std;

Gomoku::Gomoku(unsigned int a, unsigned int b, unsigned int c) : GameBase() {
	fileName = "Gomoku.txt";

	bool wantLoad = isRightFormat();
	//if the was the correct format and the user didn't want to load
	//or the file was the incorrect format
	if(!wantLoad){
		dimen_x = a, dimen_y = b, playerTurn = PlayerX, lastPlayer = PlayerO, longString = getlongstring(), movesToWin = c, moveCount = 0;
	}
	//initialize game board
	boardArrayStart(wantLoad, fileName);
}


std::ostream& operator<<(std::ostream &o, const Gomoku &board) {

	int printMax = board.longString + 2;
	int currentBuffer;
	int leftBuffer;
	int rightBuffer;
	unsigned int preBuffer = numberOfDigits(board.dimen_y);
	if (board.longString < preBuffer) {
		printMax = preBuffer + 2;
	}
	//for every row on the gameboard starting at the top ydim = board rows
	for (size_t ydim = board.dimen_y; ydim > 0; --ydim) {

		//uses preBuffer which is number of digits in coordinate axis
		cout << setw(preBuffer) << ydim << " |";

		//for every space in that row
		for (size_t xdim = 0; xdim < board.dimen_x; ++xdim) {

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
	o << setw(preBuffer) << 0 << " |";
	for (size_t xdim = 1; xdim <= board.dimen_x; ++xdim) {
		currentBuffer = printMax - 1;
		leftBuffer = currentBuffer / 2;
		rightBuffer = currentBuffer - leftBuffer;
		int digiCheck = numberOfDigits(xdim);
		if (digiCheck == printMax - 2) {
			o << setw(leftBuffer) << xdim << "|";
		}
		else {
			o << setw(leftBuffer) << xdim << setw(rightBuffer) << "|";
		}
	}
	o << endl;
	return o;
}

void Gomoku::print() {
	cout << (*this);
	return;
}

//see documentations for TicTacToe.done()
bool Gomoku::done() {
	int tracker = 0;
	for (unsigned int b = dimen_y; b >= 1; --b) {
		for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
			if (checkRight(a, b - 1, tracker, invalid)) {
				cout << "Horizontal Victory" << endl;
				return true;
			}
			if (checkDown(a, b - 1, tracker, invalid)) {
				cout << "Vertical Victory" << endl;
				return true;
			}
			if (checkDR(a, b - 1, tracker, invalid)) {
				cout << "DiagonalR Victory" << endl;
				return true;
			}
			if (checkDL(a, b - 1, tracker, invalid)) {
				cout << "DiagonalL Victory" << endl;
				return true;
			}
		}
	}

	for (unsigned int b = 0; b <= dimen_y - 1; ++b) {
		for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
			(*this).boardArray[b][a].resetVisits();
		}
	}
	return false;
}

//Since this method will only be called if done() returns false
//We are now checking empty spaces next to pieces in a line of a certain color
//as well as those pieces themselves
bool Gomoku::draw() {
	if (moveCount == dimen_x * dimen_y) {
		return false;
	}
	//for every color that a player is using
	for (auto elem : playerColors) {
		int tracker = 0;
		//track the maximum number of pieces of that that could lie in a similar direciton
		for (unsigned int b = dimen_y; b >= 1; --b) {
			for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
				if (drawRight(a, b - 1, tracker, elem)) {
					return false;
				}
				if (drawDown(a, b - 1, tracker, elem)) {

					return false;
				}
				if (drawDR(a, b - 1, tracker, elem)) {

					return false;
				}
				if (drawDL(a, b - 1, tracker, elem)) {

					return false;
				}
			}
		}

		for (unsigned int b = 0; b <= dimen_y - 1; ++b) {
			for (unsigned int a = 0; a <= dimen_x - 1; ++a) {
				(*this).boardArray[b][a].resetVisits();
			}
		}
	}
	//if none of the draw functions were True (no possible length n) 
	//then we have a draw
	return true;
}


int Gomoku::turn() {
	//general loop variables and value storage variables
	bool isLooping = true;
	unsigned int a;
	unsigned int b;
	unsigned int c;

	//stores which players turn it is in playerIndex 
	// .playerTurn returns an int from enum Player
	unsigned int playerIndex = playerTurn;

	string playerTurn = myPlayers[playerIndex];

	//Creates a gamepiece for the player with that players color, name, and printing symbol
	GamePiece playerPiece = GamePiece(playerColors[playerIndex], myPlayers[playerIndex], playerSymbols[playerIndex]);

	cout << endl << "It is " << playerTurn << "'s turn!" << endl;


	while (isLooping) {
		//If the user does not type 'quit' as their input, stores the users input in a and b
		try{ prompt(a, b, c); }
		catch (Error){
			return savePrompt();
		}
		//if the the requested space has already been taken
		if (boardArray[b - 1][a - 1].color != invalid) {

			//repromt the user by going to the next iteration of the loop
			cout << "This space is already taken!" << endl << endl;
			continue;
		}

		//otherwise the users input was valid, makes players move and stops while loop
		else {
			boardArray[b - 1][a - 1] = playerPiece;
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

int Gomoku::savePrompt() {

	//says hey output here
	ofstream myOutput(fileName);
	if (!myOutput) {
		cout << "Could not open output file!" << endl;
		return NoOutput;
	}
	
	//did they win
	if (isDone) {
		myOutput << "NO DATA";
		return Success;
	}
	//did the draw
	if (isDraw) {
		myOutput << "NO DATA";
		return Draw;
	}
	//be polite
	cout << endl << "Would you like to save? (Y/N)" << endl;
	if (yesOrNo()) {

		//start member variables to file with whitespace seperators
		myOutput << dimen_x << " " << dimen_y << " " << playerTurn << " " << lastPlayer << " " << movesToWin << " " << moveCount <<endl;

		// write gameboard colors to file as integers
		for (size_t ydim = 0; ydim < dimen_y ; ++ydim) {
			for (size_t xdim = 0; xdim < dimen_x; ++xdim) {
				myOutput << boardArray[ydim][xdim].color << " ";
			}
			myOutput << endl;
		}
		return UserSave;
	}
	else{
		myOutput << "NO DATA";
		return UserQuit;
	}
	
}