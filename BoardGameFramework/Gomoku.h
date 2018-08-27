#pragma once

#ifndef GOMOKU_H
#define GOMOKU_H

class Gomoku : public GameBase
{
public:
	friend std::ostream & operator << (std::ostream &, const Gomoku &);

	Gomoku(unsigned int a, unsigned int b, unsigned int c);

	virtual void print() override;
	virtual bool done() override;
	virtual bool draw() override;
	virtual int turn() override;
	virtual int savePrompt() override;
};

#endif //