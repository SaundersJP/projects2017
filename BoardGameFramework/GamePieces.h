#pragma once

#ifndef GAMEPIECES_H
#define GAMEPIECES_H

enum piece_color { red, black, invalid };

struct GamePiece {
	GamePiece();
	GamePiece(piece_color a, std::string b, std::string c);
	piece_color color;
	std::string name;
	std::string printType;
	bool visitedRight = false;
	bool visitedDown = false;
	bool visitedDR = false;
	bool visitedDL = false;
	virtual void resetVisits();
};

GamePiece userPiece();

std::ostream& operator<<(std::ostream &, const GamePiece &);

std::string colorToString(piece_color myColor);
piece_color stringToColor(std::string colorString);

#endif