#pragma once

#ifndef SUDOKUGAME_H
#define SUDOKUGAME_H

class SudokuGame : public GameBase
{
public:
	
	SudokuGame(unsigned int a, unsigned int b);
	virtual void boardArrayStart(bool loading, std::string fileName) override;
	void intBoardStart();

	friend std::ostream & operator<<(std::ostream &, const SudokuGame &);
	virtual void print() override;

	virtual bool done() override;
	virtual bool draw() override;
	virtual int turn() override;
	virtual int savePrompt() override;

	bool spacesLeft();

	bool rowCheck(unsigned int row);
	bool columnCheck(unsigned int column);
	bool squareCheck(unsigned int row, unsigned int column);

protected:
	unsigned int startingNumbers;
	std::vector<std::vector<int>> startingPositions;
};
#endif