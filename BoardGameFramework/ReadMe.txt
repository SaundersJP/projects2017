========================================================================
    CONSOLE APPLICATION : Lab4 Project Overview
	Jonathan Saunders									CSE 260M Shidal
========================================================================

NOTE THIS APPLICATION ALLOWS YOU TO SAVE YOUR GAMES!
IF YOU DO NOT WANT TO LOSE YOUR PREVIOUS SAVE ON QUIT SIMPLY X OUT THE WINDOW! 

GameBase.h/cpp
	Definitions for the GameBase class;
	Includes static instance methods with shared pointersfor memory management;

GamePieces.h/cpp
	Definitions for basic gamepiece objects used in the board vectors generated
	by GameBase derivative classes

Gomoku.h/cpp
	Derivative GameBass class. Modular, Supports GameBoards well over the maximum
	size that will fit in a command window.

HelperEnumFunc.h/cpp
	 Relatively random helper functions that do not fit in GameClasses or GamePieces
	 NOTE: Does include global variables used by ALL gamebase classes as certain
	 Gamebase methods have to call on derivative classes, and it makes linkage easier

SudokuGame.h/cpp
	Derivative GameBase class. Semi-modular, can be implemented in more GOMOKU 
	style for expandability, but input file parsing makes it a difficult convert.
	WARNING: Constructor is possible

TicTacToeGame.h/cpp
	Derivative GameBase class for TicTacToe. Almost similar in every regard,
	less modular but most methods are almost exactly the same

Lab4.cpp
    This is the main application source file.

/////////////////////////////////////////////////////////////////////////////
REDESIGN DECISIONS

This was probably one of the biggest challenges I have faced in terms of troubleshooting
tiny little errors on the other end of my codebase. I wound up making a couple of key 
redesign decisions. The first redesign decision I made was implementing a draw() method for 
gomoku that has the ability to stop the game on the first turn where winning is not possible.
The next was moving some semi-globals used by Gamebase classes and making
them pure globals for ease of functionality. Otherwise my already large GameBase class would
have exploded. I was also running into linking errors when trying to keep it semi-global
as gamebase needs to be able to call its derivative class constructors with its instance method.
Looking back I probably could have gotten around this more elegantly but overall it is functional.

The most important decisions I made was implemented STRICT input file parsing, and Implementing the
3 input void prompt method as opposed to sticking with a 2 input int prompt method with addition 
SudokuGame::extraPrompt() method. I also was somehow able to keep the extra credit portion
from lab 3 working!

With regards to file parsing. I do slightly break from standard coding practices. While trycatch
blocks are not supposed to be used for regular program control, with input file streams they 
wound up being crucial for implementing negative number detection and non-digit character
detection. I do not use them to jump large distance, but more to set flags and exit some tiny
loops that are automatically pointless to continue checking if you get a bad character. There
is a fair bit of tracking which character you are reading for Sudoku and a single bad character makes it very
difficult to properly load the rest of game. While there were otherways of implementing this, there
was a lot of string to int conversions that needed to happen to properly deal with large strings/signed numbers.
Technically it follows the guidelines of "abnormal conditions" but it just feels dirty.

Implementing the three input void prompt method was a bit tricky. Initially I had been using a two input virtual
prompt method that I inteded on tweaking for when a Sudoku object entered it. However once
I began implementing a few globals I decided to just have any calls to the Sudoku constructor to
simply set an integer and use that to give the player an additional input in the method. Making the function
void though required me to implement try catch blocks for program control as I was relying on return values
from prompt() in every turn() method and the primary play() method. While the exceptions thrown 
may seem like they don't immediately unwind the stack, they ultimately do in the prompt method.

 

/////////////////////////////////////////////////////////////////////////////
PART V COPY CONTROL DESIGN DECISIONS

I will admit. I did not put too much thought into strict copy control mainly due to my utilization of
shared pointers and lack of dynamic memory allocation. A lot of the questions 1,2,3 imply that you made a conscious
decision with regards to them. I just said "I am not going to dynamically allocate memory to often and use shared_ptrs"
Most memory issues are resolved as stack frames pop in and out but for the sake of being thorough I will list out my lack of 
thinking. (Thank god for shared_ptrs)

GAMEBASE:
copy constructor: Didn't even touch it directly thanks to shared_ptrs;
move constructor: Didn't even touch it;
copy-assignment operator: Didn't even touch it? (I may have? I am still sketch about copy-assignment)
move-assignment operator: Didn't even touch it;
destructor: Called when program exits via shared_ptr. Allows game to constantly run until we quit! 

GAMEPIECE:
copy constructor: Didn't even touch it;
move constructor: Didn't even touch it;
copy-assignment operator: Used for implementing gamepieces in local functions, never dynamically allocated;
move-assignment operator: I do not believe I used the move assignment operator
destructor:  Generally called pretty consistently to remove stack Gamepieces automatically

GOMOKU/TICTACTOE
copy constructor: Didn't even touch it;
move constructor: Didn't even touch it;
copy-assignment operator: I don't believe I touched it;
move-assignment operator: Didn't even touch it;
destructor:  Called when program exits via shared_ptr. Allows game to constantly run until we quit!

SUDOKUGAME
copy constructor: Didn't even touch it;
move constructor: Didn't even touch it;
copy-assignment operator: I don't believe I touched it;
move-assignment operator: Didn't even touch it;
destructor:  Called when program exits via shared_ptr. Allows game to constantly run until we quit!

I can however say that, For a lot of these unused/untouched  constructors/assignment operators
it could be beneifical to move them to being private members of their classes in order to  prevent
unwanted calls to them. I think it could be particularly useful in programs that do require
a lot of dynamic memory allocation as you could prevent a large number of objects from being lost
in the the heap and drastically reduce your system resources. 
/////////////////////////////////////////////////////////////////////////////
ERRORS/WARNINGS

I got tons of error for a while dealing with linkage issues and the fact that my gamebase class
was having to call derivative class via a static method definied in in GameBase.cpp.
Ultimately I fixed that issue and most of my other errors were with file parsing save games.
Sudoku took no time to implement it was repeatedly trying to figure out obscure edge cases
with file parsing!
/////////////////////////////////////////////////////////////////////////////
RETURN VALUES:

Success = 0; Game ended in victory condition
UserQuit = 1; User quit on their turn
Draw = 2; Game ended in draw
MassiveError = 3; Arrived at unexpected location in code
NoInstance = 4; No instance of the game was generated by instance method
InstancePresent = 5; Instance already present when calling instance method
BadName = 6; Didn't input a proper game name
NoArgs = 7; no arguments given with executable name
TicTooMany = 8; too many arguments while trying to play tic tac toe
GoTooMany = 9; too many arguments for any gomoku implementation
GoNeg1 = 10; tried to input a negative number of moves to win
GoNotInt1 = 11; didnt input integer character for moves to win
GoNeg2 = 12; extension to board size of error 10
GoNotInt2 = 13; extension of error 11
BadAlloc = 14; couldn't allocate memory
UserSave = 15; user saved on exit
SudTooMany = 16; too many arguments for sudoku
NoOutput = 17; Couldn't generate output file
CantSave = 18; generated file couldn't write
/////////////////////////////////////////////////////////////////////////////
TEST CASES

As usual, the sheer number of test cases can be overwhelming. Especially
when we also have input files to deal with. Luckily for you as a user
you shouldn't have to worry about any of that! Here are properly formatted
arguments followed by so bad ones that I have spent a fair bit of time checking!

lab4 TicTacToe
Lab3 Gomoku
Lab3 Gomoku *moves to win*
Lab3 Gomoku *boardsize* *moves to win*
Lab4 Sudoku

Here are a few that did not work and returned argument errors:

Lab3
Lab3 TicTacToe123123123
Lab3 123TicTacToe
Lab3 TicTacToe 123
Lab3 Gomoku -1 -1
Lab3 Gomoku s -1
Lab3 Gomoku 0 0
Lab3 Gomoku -1 1
Lab3 Gomoku s
Lab3 Gomoku -1
Lab3 Gomoku Too Many Arguments
Lab3 Gomoku 19 7 Psych
Lab4 Sudoku Too Much Arguing

EXAMPLE INPUTS

SudokuGame.txt:

0 30// moves last game followed by number of starting pieces
0 4 //below here all number lower than 10 greater than -1
0 7
0 8
1 3
1 4
1 5
1 8
2 1
2 6
2 7
3 0 //these 30 rows are the starting pieces
3 4
3 8
4 0
4 3
4 5
4 8
5 0
5 4
5 8
6 1
6 2
6 7
7 0
7 3
7 4
7 5
8 0
8 1
8 4
0 0 0 0 8 0 0 7 9 
0 0 0 4 1 9 0 0 5 
0 6 0 0 0 0 2 8 0  // gameboard all 
7 0 0 0 2 0 0 0 6 
4 0 0 8 0 3 0 0 1 
8 0 0 0 6 0 0 0 3 
0 9 8 0 0 0 0 6 0 
6 0 0 1 9 5 0 0 0 
5 3 0 0 7 0 0 0 0 

TicTacToe/Gomoku.txt

5 5 0 1 3 0 //// *xdimension *ydimension *playerturn *lastturn *movestowin *movecount
2 2 2 2 2 
2 2 2 2 2 ////positive integers less than/equal to piece_color invalid
2 2 2 2 2 
2 2 2 2 2 
2 2 2 2 2 
