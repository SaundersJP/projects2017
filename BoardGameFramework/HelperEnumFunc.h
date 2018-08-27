#pragma once

#ifndef HELPERENUMFUNC_H
#define HELPERENUMFUNC_H


//the external variables are because multiple .cpp files need them to function
//but they all need to access the same "list"
//Patchy solution but it work
extern std::vector<std::string> myPlayers;
extern std::vector<std::string> playerSymbols;
extern std::vector<piece_color> playerColors;
extern unsigned int Sudoku;


enum ArrayIndicies { Name, InputFile, ArgLengthStd, ArgLength1, ArgLength2 };
enum Error { Success, UserQuit, Draw, MassiveError, NoInstance, InstancePresent, BadName,
			 NoArgs, TicTooMany, GoTooMany, GoNeg1, GoNotInt1, GoNeg2, GoNotInt2, BadAlloc,
			 UserSave, SudTooMany, NoOutput, CantSave};

void sudokuPrompt(unsigned int &z);
void lowerCaseIt(std::string &randomString);
int useful_message(char program_name[], Error e);
unsigned int numberOfDigits(int x);
unsigned int getlongstring();
bool yesOrNo();

#endif