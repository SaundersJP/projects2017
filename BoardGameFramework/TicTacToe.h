#pragma once

#ifndef TICTACTOE_H
#define TICTACTOE_H

class TicTacToeGame : public GameBase
{
public:
	friend std::ostream & operator << (std::ostream &, const TicTacToeGame &);

	//default constructor
	TicTacToeGame();
	//constructor for a gameboard of size (a x b)
	//TicTacToe must be intialized with a = 5, b = 5

	virtual void print() override;
	virtual bool done() override;
	virtual bool draw() override;
	virtual int turn() override;
	virtual int savePrompt() override;

};

#endif