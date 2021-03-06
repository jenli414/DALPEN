/*
 * jenli414 och sabse455
 */

#ifndef _boggle_h
#define _boggle_h

#include <iostream>
#include <string>
#include <vector>
#include <set>
#include "grid.h"
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
     * Destructor.
     */
    ~Boggle() = default;


    /*
     * Prepares a new game of Boggle.
     */
    void newGame();


    /*
     * Takes a string representation of board and sets it to m_boardGrid.
     */
    void setBoard(string board);


    /*
     * Sets m_boardGrid to a random, valid board (Grid).
     */
    void setRandomBoard();


    /*
     * Returns the number of letters in a valid board.
     */
    int getNumOfLettersReq() const;


    /*
     * Returns the current boarmap<int, set<int>>& visitedPositionsd represented in a Grid object.
     */
    Grid<char> getBoard() const;


    /*
     * Returns the number of words in m_playerFound.
     */
    int getPlayerFoundNum() const;


    /*
     * Returns the number of words in m_NPCFound,
     */
    int getNPCFoundNum() const;


    /*
     * Returns the player score.
     */
    int getPlayerScore() const;


    /*
     * Returns the NPC score.
     */
    int getNPCScore() const;


    /*
     * Returns the string representation of m_playerFound.
     */
    string getPlayerFoundStr() const;


    /*
     * Returns the string representation of m_NPCFound.
     */
    string getNPCFoundStr() const;


    /*
     * Returns true if word length >= MIN_WORD_LENGTH.
     */
    bool isValidLength(const string& word) const;


    /*
     * Return true if word is in board.
     */
    bool isInBoard(const string& word) const;

    /*
     * Returns true if word is in m_dictionary.
     */
    bool isInDictionary(const string& word) const;


    /*
     * Returns true if word is not in m_playerFound or m_NPCFound.
     * NOTE: This function does not check if word is in m_dictionary.
     */
    bool isNewWord(string& word) const;


    /*
     * Inserts a word into m_playerFound and updates relevant data members.
     * Pre-condition: Word is a valid word and in upper case.
     */
    void addToPlayerFound(const string& word);


    /*
     * Inserts a word into found and updates the other given data members.
     * Pre-condition: Word is a valid word and in upper case.
     */
    void addToCharacterFound(const string& word, set<string>& found,
                             int& score, string& foundStr);


    /*
     * Finds all words in board that player hasn't and adds them to m_NPCFound.
     */
    void findAllWords();


private:
    Grid<char> m_boardGrid;         // Current board represented as a grid.
    set<string> m_playerFound;      // Words that player has found.
    set<string> m_NPCFound;         // Words that NPC has found.
    Lexicon m_dictionary;           // Valid Words.    
    int m_playerScore;              // Player score
    int m_NPCScore;                 // NPC score
    string m_playerFoundStr;        // String representation of m_playerFound
    string m_NPCFoundStr;           // String representation of m_NPCFound


    /*
     * Recursive helper that checks if the first letter in word matches the letter
     * in (row,col) in m_boardGrid and if so, checks if the rest of the word can be
     * traced from there.
     */
    bool isInBoardHelper(const int& currRow, const int& currCol,
                         const string& word, map<int,set<int>>& visitedPositions) const;


    /*
     * Recursive helper that checks if given prefix + letter at (row,col) in
     * m_boardGrid is a word or prefix to any word i m_dictionary.
     * If it makes a valid word it will be added to m_ NPCFound, if it is a
     * prefix to another word in dictionary we continue looking for valid words
     * by moving to neighbouring letters in board.
     */
    void findAllWordsHelper(const int& row, const int& col, string& prefix,
                            map<int,set<int>>& visitedPositions);

};

#endif
