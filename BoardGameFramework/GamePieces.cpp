//Includes all definitions for our Struct GamePiece and the methods
//to switch from string to color and vice versa

#include "stdafx.h"
#include <string>
#include <vector>
#include <iostream>
#include <sstream>
#include "GamePieces.h"

using namespace std;

//Add any new colors here! And in the header file (GamePieces.h) with the same value as this one
vector<string> colorVector{ "red", "black", "white", "blue", "invalid" };

//declare constructor and extraction operator
GamePiece::GamePiece(piece_color a, string b, string c) : color(a), name(b), printType(c) {}
GamePiece::GamePiece() : color(invalid), name(""), printType(" ") {}

ostream& operator<<(ostream &o, const GamePiece &stuff) {
	o << stuff.printType;
	return o;
}

//straightforward
string colorToString(piece_color myColor) {
	return colorVector[myColor];
}


//checks if the string matches anything in the list of colors, if not returns invalid color
piece_color stringToColor(string colorString) {
	for (size_t i = 0; i < colorVector.size(); ++i) {
		if (colorString == colorVector[i]) {
			return (piece_color)i;
		}
	}
	return piece_color::invalid;
}

GamePiece userPiece() {
	bool badColor = true;

	unsigned int myColor;
	piece_color myPiece_Color;
	string colorInput;
	string myName;
	string printer;
	cout << "What is your name?" << endl;
	getline(cin, myName);
	cout << "What would you like your pieces to be?" << endl;
	getline(cin, printer);

	while (badColor) {

		cout << "Your color options are:" << endl;
		for (size_t i = 0; i < colorVector.size(); ++i) {
			cout << i + 1 << ": " << colorVector[i] << endl;
		}
		cout << "Please input the number corresponding to your color choice!" << endl;

		if (getline(cin, colorInput)) {

			istringstream wrappedInput(colorInput);
			wrappedInput >> myColor;

			if (wrappedInput.fail()) {
				string idiotUser;
				cout << "Invalid number!" << endl;
				wrappedInput.clear();
				wrappedInput >> idiotUser;
				continue;
			}
			else if (myColor < 1 || myColor > colorVector.size()) {
				cout << "Invalid number!" << endl;
			}
			else {
				myColor -= 1;
				badColor = false;
			}
		}
	}
	myPiece_Color = (piece_color)myColor;
	return GamePiece(myPiece_Color, myName, printer);
}

void GamePiece::resetVisits() {
	visitedRight = false;
	visitedDown = false;
	visitedDR = false;
	visitedDL = false;
	return;
}

