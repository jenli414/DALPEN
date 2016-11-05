#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"

/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    clearConsole();
    boggle.newGame();
    if (yesOrNo("Would you like to enter a pre-detemined board? (Y/N): ")) {
        boggle.setBoard(getBoardInput(boggle));
    }
    clearConsole();
    printBoard(boggle);
    cout << endl;
    playPlayerTurn(boggle);
}


/*
 * Prompts the user for board input. Prints error messages if input either
 * contains characters apart from A-Z (case-insensitively) or if the input
 * isn't of the correct length. Returns valid valid input in upper case.
 */
string getBoardInput(const Boggle& boggle) {
    int numOfLettersReq = boggle.getNumOfLettersReq();
    int numOfLettersEntered;
    cout << "Please enter a board, i.e. " << numOfLettersReq
         << " letters from A-Z: " << endl;
    string input;
    bool validInput = false;
    while (!validInput) {
        getline(cin, input);
        numOfLettersEntered = 0;
        bool isAllLetters = true;
        for (string::iterator it = input.begin(); it != input.end(); ++it) {
            numOfLettersEntered++;
            if (!isalpha(*it)) {
                isAllLetters = false;
            }
        }
        if (numOfLettersEntered != numOfLettersReq) {
            cout << "Please enter _" << numOfLettersReq
                 << "_ letters from A-Z!: " << endl;
        } else if (!isAllLetters) {
            cout << "Please enter a only LETTERS from A-Z!: " << endl;
        } else {
            validInput = true;
        }
    }
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    return input;
}


/*
 * Erases all currently visible text from the output console.
 */
void clearConsole() {
#if defined(_WIN32) || defined(_WIN64)
    std::system("CLS");
#else
    // assume POSIX
    std::system("clear");
#endif
}


/*
 * Prints current board.
 */
void printBoard(const Boggle& boggle) {
    string board = boggle.getBoard();
    cout << endl;
    for (int i = 0; i < boggle.BOARD_SIZE; ++i) {
        for (int i = 0; i < boggle.BOARD_SIZE; ++i) {
            cout << board[0] << " ";
            board = board.substr(1, board.size() - 1);
        }
        cout << endl;
    }
}


/*
 * Prints the words that a player has found as well as the current score.
 */
void printPlayerStatus(const Boggle& boggle) {
    int numOfWordsFound;
    string foundWordsStr;
    int score;
    boggle.setPlayerStatus(numOfWordsFound, foundWordsStr, score);
    cout << "Your words (" << numOfWordsFound << "): " <<
            foundWordsStr << endl <<
            "Your score: " << score << endl;
}


/*
 * Asks the user to input words and gives error messages upon recieving invalid
 * such until the user only presses enter to end their turn.
 * Lets boggle handle the input if it's valid.
 */
void playPlayerTurn(Boggle& boggle) {
    cout << "Your turn!" << endl;
    bool endTurn = false;
    bool validInput = false;
    string input;
    while (!endTurn) {
        while (!validInput) {
            cout << "Type a valid word or press enter to end your turn: ";
            getline(cin, input);
            if (input.empty()) {
                cout << endl;
                validInput = true;
                endTurn = true;
            } else {
                clearConsole();
                printBoard(boggle);
                cout << endl;
                transform(input.begin(), input.end(), input.begin(), ::toupper);
                validInput = isValidInput(boggle, input);
                if (validInput) {
                    cout << "You found a new word: " << input << "!" << endl;
                    boggle.addToPlayerFound(input);
                    validInput = false;
                }
                printPlayerStatus(boggle);
                cout << endl;
            }
        }
    }
}


/*
 * Returns true if input fulfills certain conditions.
 * If any condition is not satisfied, an error message is printed.
 * Pre-condition: Assumes that any letters in input are upper case.
 */
bool isValidInput(const Boggle& boggle, const string& input) {
    if (!boggle.isValidLength(input)) {
        cout << "\"" << input << "\" is too short! " <<
                "Please enter a word of length " <<
                boggle.MIN_WORD_LENGTH << " or longer." << endl;
        return false;
    } else if (!boggle.isInBoard(input)) {
        cout << "Sorry, couldn't find \"" << input << "\" in board!" << endl;
        return false;
    } else if (!boggle.isInDictionary(input)) {
        cout << "Sorry, \"" << input << "\" is not in the dictionary!" << endl;
        return false;
    } else if (!boggle.isNewWord(input)) {
        cout << "Sorry, looks like you already found \"" << input << "\"!" << endl;
        return false;
    }
    return true;
}
