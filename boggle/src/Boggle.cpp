#include <sstream>
#include <algorithm>
#include <set>
#include "Boggle.h"
#include "random.h"
#include "shuffle.h"
#include "strlib.h"

static const int NUM_CUBES = 16;   // the number of cubes in the game
static const int CUBE_SIDES = 6;   // the number of sides on each cube
static string CUBES[NUM_CUBES] = {        // the letters on all 6 sides of every cube
   "AAEEGN", "ABBJOO", "ACHOPS", "AFFKPS",
   "AOOTTW", "CIMOTU", "DEILRX", "DELRVY",
   "DISTTY", "EEGHNW", "EEINSU", "EHRTVW",
   "EIOSST", "ELRTTY", "HIMNQU", "HLNNRZ"
};


/*
 * Constructor.
 */
Boggle::Boggle() {
    Lexicon dictionary(DICTIONARY_FILE);
    m_dictionary = dictionary;
    m_boardGrid.resize(BOARD_SIZE, BOARD_SIZE);
}


/*
 * Deconstructor.
 */
Boggle::~Boggle() {

}


/*
 * Prepares a new game of Boggle.
 */
void Boggle::newGame() {
    setRandomBoard();
    m_playerFound.clear();
    m_NPCFound.clear();
    m_playerFoundNum = 0;
    m_NPCFoundNum = 0;
    m_playerScore = 0;
    m_NPCScore = 0;
    m_playerFoundStr = "{}";
    m_NPCFoundStr = "{}";
}


/*
 * Returns the number of letters in a valid board.
 */
int Boggle::getNumOfLettersReq() const {
    return pow(BOARD_SIZE,2);
}


/*
 * Returns the current board represented in a Grid object.
 */
Grid<char> Boggle::getBoard() const {
    return m_boardGrid;
}


/*
 * Returns the number of words in m_playerFound.
 */
int Boggle::getPlayerFoundNum() const {
    return m_playerFoundNum;
}


/*
 * Returns the number of words in m_NPCFound,
 */
int Boggle::getNPCFoundNum() const {
    return m_NPCFoundNum;
}


/*
 * Returns the player score.
 */
int Boggle::getPlayerScore() const {
    return m_playerScore;
}


/*
 * Returns the NPC score.
 */
int Boggle::getNPCScore() const {
    return m_NPCScore;
}


/*
 * Returns the string representation of m_playerFound.
 */
string Boggle::getPlayerFoundStr() const {
    return m_playerFoundStr;
}


/*
 * Returns the string representation of m_NPCFound.
 */
string Boggle::getNPCFoundStr() const {
    return m_NPCFoundStr;
}


/*
 * Takes a string representation of board and sets it to m_boardGrid.
 */
void Boggle::setBoard(string board) {
    transform(board.begin(), board.end(), board.begin(), ::toupper);
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            m_boardGrid.set(row, col, board[col + (row * BOARD_SIZE)]);
        }
    }
}


/*
 * Returns true if word length >= MIN_WORD_LENGTH.
 */
bool Boggle::isValidLength(const string& word) const {
    return word.length() >= MIN_WORD_LENGTH;
}


/*
 * Returns true if given word is in board.
 */
bool Boggle::isInBoard(string word) const {
    map<int,set<int>> visitedPositions;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            if (isInBoardHelper(row, col, word, visitedPositions)) {
                return true;
            }
        }
    }
    return false;
}


/*
 * Returns true if word is in m_dictionary.
 */
bool Boggle::isInDictionary(const string& word) const {
    return m_dictionary.contains(word);
}


/*
 * Returns true if word is not in m_playerFound or
 * m_NPCFound.
 * NOTE: This function does not check if word is in
 * m_dictionary.
 */
bool Boggle::isNewWord(string& word) const {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    return !m_playerFound.count(word) && !m_NPCFound.count(word);
}

/*
 * Inserts a word into m_playerFound and updates relevant data members.
 * Pre-condition: Word is a valid word and in upper case.
 */
void Boggle::addToPlayerFound(string& word) {
    addToCharacterFound(word, m_playerFound, m_playerFoundNum,
                        m_playerScore, m_playerFoundStr);
}


/*
 * Inserts a word into found and updates the other given data members.
 * Pre-condition: Word is a valid word and in upper case.
 */
void Boggle::addToCharacterFound(const string& word, set<string>& found,
                                 int& foundNum, int& score, string& foundStr) {
    found.insert(word);
    foundNum++;
    score += word.length() - 3;
    if (foundNum == 1) {
        foundStr = "{" + word + "}";
    } else {
        foundStr.pop_back();
        foundStr += ", " + word + "}";
    }
}


/*
 * Finds all words in board that player hasn't and adds them to m_NPCFound.
 */
void Boggle::findAllWords() {
    map<int,set<int>> visitedPositions;
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            findAllWordsHelper(row, col, "", visitedPositions);
        }
    }
}


/*
 * Sets m_boardGrid to a random, valid board (Grid).
 */
void Boggle::setRandomBoard() {
    string board = "";
    shuffle(CUBES, NUM_CUBES);
    random_device rd;                                           // Get random num from hardware.
    mt19937 eng(rd());                                          // Seed the generator.
    for (unsigned int i = 0; i < NUM_CUBES; ++i) {
        uniform_int_distribution<> distr(0,(CUBE_SIDES - 1));   // Define the range (inclusive).
        board += CUBES[i][distr(eng)];                          // Generates a number and adds new letter to board.
    }
    setBoard(board);
}


/*
 * Recursive helper that checks if the first letter in word matches the letter
 * in (row,col) in m_boardGrid and if so, checks if the rest of the word can be
 * traced from there.
 */
bool Boggle::isInBoardHelper(const int& row, const int& col, string word,
                             map<int,set<int>>& visitedPositions) const {
    bool isMatch = word[0] == m_boardGrid.get(row, col);
    if (word.length() == 1) {
        return isMatch;
    } else if (isMatch) {
        visitedPositions[row].insert(col);
        for (int row_i = -1; row_i <= 1; ++row_i) {
            for (int col_i = -1; col_i <= 1; ++col_i) {
                bool isInBounds = m_boardGrid.inBounds(
                            row + row_i, col + col_i);
                bool visited = visitedPositions[row + row_i].count(col + col_i);
                if (isInBounds && !visited) {
                    if (isInBoardHelper(row + row_i, col + col_i,
                                        word.substr(1, word.size() - 1),
                                        visitedPositions)) {
                        return true;
                    }
                }
            }
        }
        visitedPositions.clear();
        return false;
    } else {
        visitedPositions.clear();
        return false;
    }
}


/*
 * Recursive helper that checks if given prefix + letter at (row,col) in
 * m_boardGrid is a word or prefix to any word i m_dictionary.
 * If it makes a valid word it will be added to m_ NPCFound, if it is a
 * prefix to another word in dictionary we continue looking for valid words
 * by moving to neighbouring letters in board.
 */
void Boggle::findAllWordsHelper(const int& row, const int& col, string prefix,
                             map<int,set<int>> visitedPositions) {
    prefix += m_boardGrid.get(row,col);
    bool isValidWord = isValidLength(prefix) &&
            isInDictionary(prefix) && isNewWord(prefix);
    bool morePossibleWords = m_dictionary.containsPrefix(prefix);
    if (isValidWord) {
        addToCharacterFound(prefix, m_NPCFound, m_NPCFoundNum,
                            m_NPCScore, m_NPCFoundStr);
    } if (morePossibleWords) {
        visitedPositions[row].insert(col);
        for (int row_i = -1; row_i <= 1; ++row_i) {
            for (int col_i = -1; col_i <= 1; ++col_i) {
                bool isInBounds = m_boardGrid.inBounds(
                            row + row_i, col + col_i);
                bool visited = visitedPositions[row + row_i].count(col + col_i);
                if (isInBounds && !visited) {
                    findAllWordsHelper(row + row_i, col + col_i, prefix,
                                       visitedPositions);
                }
            }
        }
    }
}





/*
 * Inserts a word into m_playerFound and updates relevant data members.
 * Pre-condition: Word is a valid word and in upper case.
 */
/*void Boggle::addToPlayerFound(string& word) {
    m_playerFound.insert(word);
    m_playerFoundNum++;
    m_playerScore += word.length() - 3;
    if (m_playerFoundNum == 1) {
        m_playerFoundStr = "{" + word + "}";
    } else {
        m_playerFoundStr.pop_back();
        m_playerFoundStr += ", " + word + "}";
    }
}*/


/*
 * Inserts a word into m_NPCFound and updates relevant data members.
 * Pre-condition: Word is a valid word and in upper case.
 */
/*void Boggle::addToNPCFound(string& word) {
    m_NPCFound.insert(word);
    m_NPCFoundNum++;
    m_NPCScore += word.length() - 3;
    if (m_NPCFoundNum == 1) {
        m_NPCFoundStr = "{" + word + "}";
    } else {
        m_NPCFoundStr.pop_back();
        m_NPCFoundStr += ", " + word + "}";
    }
}*/
