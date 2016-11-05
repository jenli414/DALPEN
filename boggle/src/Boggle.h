// This is the .h file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own, as well as on the members.
// TODO: remove this comment header and replace it with your own

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "lexicon.h"

using namespace std;

class Boggle {
public:
    const string DICTIONARY_FILE = "EnglishWords.dat";
    const unsigned int MIN_WORD_LENGTH = 4;
    const int BOARD_SIZE = 4;


    /*
     * Constructor.
     */
    Boggle();


    /*
     * Constructor for pre-determined board. (For testing)
     * Pre-condition: board must be of correct length (pow(BOARD_SIZE,2))
     */
    Boggle(const string& board);


    /*
     * Deconstructor.
     */
    ~Boggle();


    /*
     * Prepares a new game of Boggle.
     */
    void newGame();


    /*
     * Returns the number of letters in a valid board.
     */
    int getNumOfLettersReq() const;


    /*
     * Returns the string representation of the current board.
     */
    string getBoard() const;


    /*
     * Sets the following information about the game state in the given variables:
     * numOfWordsFound: The number of words the player has found.
     * foundWordsStr: String representation of all words the player has found.
     * score: Player's current score.
     */
    void setPlayerStatus(int& numOfWordsFound, string& foundWordsStr, int& score) const;


    /*
     * Sets m_board to given board.
     */
    void setBoard(const string& board);

    /*
     * Returns true if word length >= MIN_WORD_LENGTH.
     */
    bool isValidLength(const string& word) const;


    /*
     * Return true if word is "in" board.
     */
    bool isInBoard(const string word) const;

    /*
     * Returns true if word is in m_dictionary.
     */
    bool isInDictionary(const string& word) const;


    /*
     * Returns true if word is not in m_playerFound
     * NOTE: This function does not check if word is in
     * m_dictionary.
     */
    bool isNewWord(const string& word) const;


    /*
     * Inserts a word into m_playerFound.
     * Precondition: We assume that word fulfills every criteria to count
     * as a valid word.
     */
    void addToPlayerFound(const string& word);


private:
    string m_board = "";                    // Current board.
    vector<vector<string>> m_playerFound;   // Words that player has found.
    vector<vector<string>> m_NPCFound;      // Words that NPC has found.
    Lexicon m_dictionary;                   // Valid Words.


    /*
     * Sets m_board to a random, valid board (string).
     */
    void setRandomBoard();


    /*
     * Returns true if given word is in board. (Recursive helper)
     */
    bool isInBoardHelper(int startIndex, string word, set<int> takenIndices, int lastIndex = -1) const;


    /*
     * Returns true if startIndex and lastIndex are neighbours in board.
     * I.e if moving between those indices could lead to a valid word.
     */
    bool areNeighbours(const int& startIndex, const int& lastIndex) const;
};

#endif
