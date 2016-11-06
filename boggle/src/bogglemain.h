/*
 * TDDD86 Boggle
 * This file declares required function prototypes that are defined in
 * our provided bogglemain.cpp and your boggleplay.cpp that you will write.
 * See the respective .cpp files for implementation comments for each function.
 * Please do not modify this provided file.
 */

#ifndef _bogglemain_h
#define _bogglemain_h

#include "Boggle.h"
#include <string>
using namespace std;

/*
 * Explains the program to the user.
 */
void intro();

/*
 * Prompts the user to answer a yes/no question and returns true if the user
 * typed 'yes' (or anything that starts with a 'y', case-insensitively),
 * false if the user types anything that starts with 'n', or re-prompts if
 * the user doesn't type a 'y' or 'n' word.
 */
bool yesOrNo(string prompt);


/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle);


/*
 * Prompts the user for board input. Prints error messages if input either
 * contains characters apart from A-Z (case-insensitively) or if the input
 * isn't of the correct length. Returns valid valid input in upper case.
 */
string getBoardInput(const Boggle& boggle);


/*
 * Erases all currently visible text from the output console.
 */
void clearConsole();


/*
 * Prints current board.
 */
void printBoard(const Boggle& boggle);


/*
 * Prints the words that a player has found as well as the current score.
 */
void printPlayerStatus(const Boggle& boggle);


/*
 * Returns true if input fulfills certain conditions.
 * If any condition is not satisfied, an error message is printed.
 * Pre-condition: Assumes that any letters in input are upper case.
 */
bool isValidInput(const Boggle& boggle, string& input);


/*
 * Asks the user to input words and gives error messages upon recieving invalid
 * such until the user only presses enter to end their turn.
 * Lets boggle handle the input if it's valid.
 */
void playPlayerTurn(Boggle& boggle);


/*
 * Plays NPC's turn.
 */
void playNPCTurn(Boggle& boggle);


#endif
