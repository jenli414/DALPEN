/*
 * jenli414 och sabse455
 */

#include <cstdlib>
#include <iostream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include "Boggle.h"
#include "bogglemain.h"
#include "strlib.h"


string getBoardInput(const Boggle& boggle);
void printBoard(const Boggle& boggle);
void playPlayerTurn(Boggle& boggle);
bool isValidInput(const Boggle& boggle, string& input);
void playNPCTurn(Boggle& boggle);


/*
 * Plays one game of Boggle using the given boggle game state object.
 */
void playOneGame(Boggle& boggle) {
    clearConsole();
    boggle.newGame();
    if (yesOrNo("Would you like to enter a pre-detemined board? (Y/N): ")) {
        boggle.setBoard(getBoardInput(boggle));
    } else {
        boggle.setRandomBoard();
    }
    clearConsole();
    printBoard(boggle);
    cout << endl;
    playPlayerTurn(boggle);
    playNPCTurn(boggle);
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
    while (true) {
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
            return input;
        }
    }
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
    Grid<char> board = boggle.getBoard();
    cout << endl;
    for (int row = 0; row < boggle.BOARD_SIZE; ++row) {
        for (int col = 0; col < boggle.BOARD_SIZE; ++col) {
            cout << " " << board.get(row, col);
        }
        cout << endl;
    }
}


/*
 * Asks the user to input words and gives error messages upon recieving invalid
 * such until the user only presses enter to end their turn.
 * Lets boggle handle the input if it's valid.
 */
void playPlayerTurn(Boggle& boggle) {
    cout << "Your turn!" << endl;
    string input;
    while (true) {
        cout << "Type a valid word or press enter to end your turn: ";
        getline(cin, input);
        if (input.empty()) {
            cout << endl;
            break;
        } else {
            clearConsole();
            printBoard(boggle);
            cout << endl;
            if (isValidInput(boggle, input)) {
                cout << "You found a new word: " << input << "!" << endl;
                boggle.addToPlayerFound(input);
            }
            cout << "Your words (" << boggle.getPlayerFoundNum() << "): " <<
                    boggle.getPlayerFoundStr() << endl <<
                    "Your score: " << boggle.getPlayerScore() << endl << endl;
        }
    }
}


/*
 * Returns true if input fulfills certain conditions.
 * If any condition is not satisfied, an error message is printed.
 */
bool isValidInput(const Boggle& boggle, string& input) {
    transform(input.begin(), input.end(), input.begin(), ::toupper);
    if (!boggle.isValidLength(input)) {
        cout << "\"" << input << "\" is too short! " <<
                "Please enter a word of length " <<
                boggle.MIN_WORD_LENGTH << " or longer." << endl;
        return false;
    } else if (!boggle.isInDictionary(input)) {
                cout << "Sorry, \"" << input << "\" is not in the dictionary!" << endl;
                return false;
    } else if (!boggle.isNewWord(input)) {
        cout << "Sorry, looks like you already found \"" << input << "\"!" << endl;
        return false;
    } else if (!boggle.isInBoard(input)) {
        cout << "Sorry, couldn't find \"" << input << "\" in board!" << endl;
        return false;
    }
    return true;
}


/*
 * Plays NPC's turn.
 */
void playNPCTurn(Boggle& boggle) {
    cout << "It's my turn! Watch this..." << endl;
    boggle.findAllWords();
    cout << "My words (" << boggle.getNPCFoundNum() << "): " <<
            boggle.getNPCFoundStr() << endl <<
            "My score: " << boggle.getNPCScore() << endl << endl;
    if (boggle.getNPCScore() > boggle.getPlayerScore()) {
        cout << "Ha ha ha, I destroyed you. Better luck next time, puny human!"
             << endl;
    } else {
        cout << "...Impossible! I guess you win this time..." << endl;
    }
}


