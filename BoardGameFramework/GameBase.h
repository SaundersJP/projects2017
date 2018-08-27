#pragma once

#ifndef GAMEBASE_H
#define GAMEBASE_H

enum Player { PlayerX, PlayerO };

class GameBase
{

public:
	GameBase() {};
	GameBase(unsigned int x, unsigned int y) : dimen_x(x), dimen_y(y) {}
	void addPlayer(GamePiece &);
	virtual void print() = 0;
	virtual bool done() = 0;
	virtual bool draw() = 0;
	virtual int turn() = 0;
	virtual void prompt(unsigned int &m, unsigned int &n, unsigned int &p);
	virtual int savePrompt() = 0;
	virtual int play();
	virtual bool isRightFormat();

	static void argCheck(int c, char * v[]);
	static std::shared_ptr<GameBase> instance();

	bool checkRight(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst);
	bool checkDown(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst);
	bool checkDR(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst);
	bool checkDL(unsigned int a, unsigned int b, int tracker, piece_color checkAgainst);
	bool drawRight(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst);
	bool drawDown(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst);
	bool drawDR(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst);
	bool drawDL(unsigned int a, unsigned int b, int tracker, piece_color drawAgainst);
	virtual void boardArrayStart(bool loading, std::string fileName);


protected:
	bool isDone = false;
	bool isDraw = false;

	unsigned int dimen_x;
	unsigned int dimen_y;

	//movecount doesn't need to ever be assigned as it defaults to zero
	unsigned int moveCount;
	unsigned int longString;
	unsigned int movesToWin;

	//keeps track of whose turn it is and who just went. Player X starts
	Player playerTurn;
	Player lastPlayer;

	//future use gameplayer vector
	std::vector<GamePiece> gamePlayers;

	//Vector of Gamepiece Vectors to create a dynamic 2D board
	std::vector<std::vector<GamePiece>> boardArray;

	static std::shared_ptr<GameBase> gamePointer;

	std::string fileName;

};

#endif