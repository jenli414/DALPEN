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
    for (int i = 0; i < 26; ++i) {
        vector<string> playerFoundVector;
        vector<string> NPCFoundVector;
        m_playerFound.push_back(playerFoundVector);
        m_NPCFound.push_back(NPCFoundVector);
    }
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
    int index = static_cast<int>(word[0]) - 65;
    for (vector<string>::const_iterator it = m_playerFound[index].begin();
         it != m_playerFound[index].end(); ++it) {
        if (*it == word) {
            return false;
        }
    }
    for (vector<string>::const_iterator it = m_NPCFound[index].begin();
         it != m_NPCFound[index].end(); ++it) {
        if (*it == word) {
            return false;
        }
    }
    return true;
}


/*
 * Inserts a word into m_playerFound and updates relevant data members.
 */
void Boggle::addToPlayerFound(string& word) {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    int index = static_cast<int>(word[0]) - 65;
    m_playerFound[index].push_back(word);
    m_playerFoundNum++;
    m_playerScore += word.length() - 3;
    if (m_playerFoundNum == 1) {
        m_playerFoundStr = "{" + word + "}";
    } else {
        m_playerFoundStr.pop_back();
        m_playerFoundStr += ", " + word + "}";
    }
}


/*
 * Inserts a word into m_NPCFound and updates relevant data members.
 */
void Boggle::addToNPCFound(string& word) {
    transform(word.begin(), word.end(), word.begin(), ::toupper);
    int index = static_cast<int>(word[0]) - 65;
    m_NPCFound[index].push_back(word);
    m_NPCFoundNum++;
    m_NPCScore += word.length() - 3;
    if (m_NPCFoundNum == 1) {
        m_NPCFoundStr = "{" + word + "}";
    } else {
        m_NPCFoundStr.pop_back();
        m_NPCFoundStr += ", " + word + "}";
    }
}


/*
 * Finds all words in board that player hasn't and adds them to m_NPCFound.
 */
void Boggle::findAllWords() {
    map<int,set<int>> visitedPositions;
    string prefix = "";
    for (int row = 0; row < BOARD_SIZE; ++row) {
        for (int col = 0; col < BOARD_SIZE; ++col) {
            findAllWordsHelper(row, col, prefix, visitedPositions);
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
                             map<int,set<int>> visitedPositions) const {
    bool isMatch = word[0] == m_boardGrid.get(row, col);
    bool notVisited = visitedPositions[row].count(col) == 0;
    if (word.length() == 1) {
        return isMatch && notVisited;
    } else if (isMatch && notVisited) {
        visitedPositions[row].insert(col);
        for (int row_i = -1; row_i <= 1; ++row_i) {
            for (int col_i = -1; col_i <= 1; ++col_i) {
                bool isCurrPosition = row_i == 0 && col_i == 0;
                bool isInBounds = m_boardGrid.inBounds(
                            row + row_i, col + col_i);
                if (!isCurrPosition && isInBounds) {
                    if (isInBoardHelper(row + row_i, col + col_i,
                                        word.substr(1, word.size() - 1),
                                        visitedPositions)) {
                        return true;
                    }
                }
            }
        }
        return false;
    } else {
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
    bool notVisited = visitedPositions[row].count(col) == 0;
    if (notVisited) {
        string newPrefix = prefix + m_boardGrid.get(row,col);
        bool isValidWord = isValidLength(newPrefix) &&
                isInDictionary(newPrefix) && isNewWord(newPrefix);
        bool morePossibleWords = m_dictionary.containsPrefix(newPrefix);
        if (isValidWord) {
            addToNPCFound(newPrefix);
        } if (morePossibleWords) {
            visitedPositions[row].insert(col);
            for (int row_i = -1; row_i <= 1; ++row_i) {
                for (int col_i = -1; col_i <= 1; ++col_i) {
                    bool isCurrPosition = row_i == 0 && col_i == 0;
                    bool isInBounds = m_boardGrid.inBounds(
                                row + row_i, col + col_i);
                    if (!isCurrPosition && isInBounds) {
                        findAllWordsHelper(row + row_i, col + col_i, newPrefix,
                                           visitedPositions);
                    }
                }
            }
        }
    }
}

/*
 * VARNING: Jennifer ansvarar inte för eventuella hjärnskador som kan uppstå som en följd av
 * betraktande utav följande kod. Hon vill dock poängtera att den på nåt vänster fungerar.
 * Slut på varningsmeddelande.
 */

/*bool Boggle::isInBoardHelper(int currRow, int currCol, string word,
                             map<int,set<int>> visitedPositions) const {
    pair<int,int> nextPosition = getNextPosition(currRow, currCol);
    bool nextIsInBounds = m_boardGrid.inBounds(nextPosition.first, nextPosition.second);
    bool isStart = visitedPositions.empty();
    bool isMatch = word[0] == m_boardGrid.get(currRow, currCol);
    bool notVisited = true;
    if (!isStart) {
        notVisited = visitedPositions[currRow].count(currCol) == 0;// This will make visitedPositions not empty if it was empty before...
    }
    if (word.length() == 1) {
        return isMatch && notVisited;
    } else if (isMatch && notVisited) {
        if (isStart && nextIsInBounds) {
            return isInBoardHelper(nextPosition.first, nextPosition.second,
                                   word, visitedPositions) ||
                    checkNeighbours(currRow, currCol, word, visitedPositions);
        } else {
            return checkNeighbours(currRow, currCol, word, visitedPositions);
        }
    } else if (isStart && nextIsInBounds) {
            return isInBoardHelper(nextPosition.first, nextPosition.second, word,
                                   visitedPositions);
    } else {
        return false;
    }
}*/


/*
 * Recursive helper to check if neighbouring letters match the next letter in word.
 */
/*bool Boggle::checkNeighbours(int currRow, int currCol, string word,
                             map<int,set<int>> visitedPositions) const {
    visitedPositions[currRow].insert(currCol);
    word = word.substr(1, word.size() - 1);
    for (int row_i = -1; row_i <= 1; ++row_i) {
        for (int col_i = -1; col_i <= 1; ++col_i) {
            bool isCurrPosition = row_i == 0 && col_i == 0;
            bool inBounds = m_boardGrid.inBounds(
                        currRow + row_i, currCol + col_i);
            if (!isCurrPosition && inBounds) {
                if (isInBoardHelper(currRow + row_i, currCol + col_i, word, visitedPositions)) {
                    return true;
                }
            }
        }
    }
    return false;
}*/


/*
 * Takes row and column number and returns a pair that represents the grid
 * coordinates of the next position in m_boardGrid. Counts from top-left to
 * top-right and then moves down one row and starts from left again.
 */
/*pair<int,int> Boggle::getNextPosition(int currRow, int currCol) const {
    int nextCol = currCol + 1;
    int nextRow = currRow;
    if (nextCol == BOARD_SIZE) {
        nextRow += 1;
        nextCol = 0;
    }
    pair<int,int> nextPosition(nextRow, nextCol);
    return nextPosition;
}*/


/*
 * Returns true if given word is in board.
 */
/*bool Boggle::isInBoard(const string word) const {
    set<int> takenIndices;
    return isInBoardHelper(0, word, takenIndices);
}*/


/*
 * Returns true if given word is in board. (Recursive helper)
 */
/*bool Boggle::isInBoardHelper(int startIndex, string word, set<int> takenIndices, int lastIndex) const {
    if (word.length() == 1) {
        return word[0] == m_boardStr[startIndex] && takenIndices.find(startIndex) == takenIndices.end() && areNeighbours(startIndex, lastIndex);

    } else if (word[0] == m_boardStr[startIndex] && takenIndices.find(startIndex) == takenIndices.end() && areNeighbours(startIndex, lastIndex)) {
        string wordWithoutFirstLetter = word.substr(1, word.size() - 1);
        if (lastIndex == -1) {
            set<int> newTakenIndices;
            takenIndices.insert(startIndex);
            return  isInBoardHelper(startIndex + 1, word, newTakenIndices) ||                             // Kolla vad som händer om vi startar helt på nästa index
                    isInBoardHelper(startIndex - 1, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå till vänster
                    isInBoardHelper(startIndex + 1, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå till höger
                    isInBoardHelper(startIndex - 4, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå uppåt
                    isInBoardHelper(startIndex + 4, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå nedåt
                    isInBoardHelper(startIndex + 5, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett nedåt höger
                    isInBoardHelper(startIndex + 3, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett nedåt vänster
                    isInBoardHelper(startIndex - 5, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett uppåt vänster
                    isInBoardHelper(startIndex - 3, wordWithoutFirstLetter, takenIndices, startIndex);    // Prova gå snett uppåt höger
        } else {
            takenIndices.insert(startIndex);
            return  isInBoardHelper(startIndex - 1, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå till vänster
                    isInBoardHelper(startIndex + 1, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå till höger
                    isInBoardHelper(startIndex - 4, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå uppåt
                    isInBoardHelper(startIndex + 4, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå nedåt
                    isInBoardHelper(startIndex + 5, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett nedåt höger
                    isInBoardHelper(startIndex + 3, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett nedåt vänster
                    isInBoardHelper(startIndex - 5, wordWithoutFirstLetter, takenIndices, startIndex) ||  // Prova gå snett uppåt vänster
                    isInBoardHelper(startIndex - 3, wordWithoutFirstLetter, takenIndices, startIndex);    // Prova gå snett uppåt höger
        }

    } else if (lastIndex == -1 && areNeighbours(startIndex, lastIndex)) {
        return isInBoardHelper(startIndex + 1, word, takenIndices);

    } else {
        return false;
    }
}*/


/*
 * Returns true if startIndex and lastIndex are neighbours in board.
 * I.e if moving between those indices could lead to a valid word.
 */
/*bool Boggle::areNeighbours(const int& startIndex, const int& lastIndex) const {
    if ((startIndex < 0) || (startIndex > ((BOARD_SIZE * BOARD_SIZE) - 1))) {               // startIndex utanför range
        return false;
    } else if (lastIndex == -1) {                                                           // Vi har inget senaste index
        return true;
    } else if (lastIndex == 0) {                                                            // Övre vänstra hörnet är granne med
        return startIndex == 1 ||                                                           // Bokstaven på index 1
                startIndex == BOARD_SIZE ||                                                 // Bokstaven nedanför
                startIndex == (BOARD_SIZE + 1);                                             // Bokstaven snett nedanför till höger

    } else if (lastIndex == (BOARD_SIZE - 1)) {                                             // Övre högra hörnet är granne med
        return startIndex == (lastIndex - 1) ||                                             // Bokstaven till vänster
                startIndex == (lastIndex + BOARD_SIZE) ||                                   // Bokstaven nedanför
                startIndex == (lastIndex + BOARD_SIZE - 1);                                 // Bokstaven snett nedanför till vänster

    } else if (lastIndex == (BOARD_SIZE * (BOARD_SIZE - 1))) {                              // Nedre vänstra hörnet är granne med
        return startIndex == (lastIndex + 1) ||                                             // Bokstaven till höger
                startIndex == (lastIndex - BOARD_SIZE) ||                                   // Bokstaven ovanför
                startIndex == (lastIndex - BOARD_SIZE + 1);                                 // Bokstaven snett ovanför till höger

    } else if (lastIndex == ((BOARD_SIZE * BOARD_SIZE) - 1)) {                              // Nedre högre hörnet är granne med
        return startIndex == (lastIndex - 1) ||                                             // Bokstaven till vänster
                startIndex == (lastIndex - BOARD_SIZE) ||                                   // Bokstaven ovanför
                startIndex == (lastIndex - BOARD_SIZE - 1);                                 // Bokstaven snett ovanför till vänster

    } else if (lastIndex < BOARD_SIZE) {                                                    // Första raden, ej hörn, är granne med:
        return startIndex == (lastIndex - 1) ||                                             // Bokstaven till vänster
                startIndex == (lastIndex + 1) ||                                            // Bokstaven till höger
                startIndex == (lastIndex + BOARD_SIZE) ||                                   // Bokstaven nedanför
                startIndex == ((lastIndex + BOARD_SIZE) - 1) ||                             // Bokstaven snett under till vänster
                startIndex == ((lastIndex + BOARD_SIZE) + 1);                               // Bokstaven snett under till höger

    } else if ((lastIndex + 1 % BOARD_SIZE) == 0) {                                         // Kolumnen till höger, ej hörn, är granne med:
        return startIndex == (lastIndex - BOARD_SIZE) ||                                    // Bokstaven ovanför
                startIndex == (lastIndex + BOARD_SIZE) ||                                   // Bokstaven nedanför
                startIndex == (lastIndex - 1) ||                                            // Bokstaven direkt åt vänster
                startIndex == ((lastIndex - BOARD_SIZE) - 1) ||                             // Bokstaven snett upp till vänster
                startIndex == ((lastIndex + BOARD_SIZE) - 1);                               // Bokstaven snett ner till vänster

    } else if (lastIndex >= (BOARD_SIZE * (BOARD_SIZE - 1))) {                              // Undre raden, ej hörn, är granne med:
        return startIndex == (lastIndex - 1) ||                                             // Bokstaven till vänster
                startIndex == (lastIndex + 1) ||                                            // Bokstaven till höger
                startIndex == (lastIndex - BOARD_SIZE) ||                                   // Bokstaven ovanför
                startIndex == ((lastIndex - BOARD_SIZE) - 1) ||                             // Bokstaven snett över till vänster
                startIndex == ((lastIndex - BOARD_SIZE) + 1);                               // Bokstaven snett över till höger

    } else if ((lastIndex % BOARD_SIZE) == 0) {                                             // Kolumnen till vänster, ej hörn, är granne med:
        return startIndex == (lastIndex - BOARD_SIZE) ||                                    // Bokstaven ovanför
                startIndex == (lastIndex + BOARD_SIZE) ||                                   // Bokstaven nedanför
                startIndex == (lastIndex + 1) ||                                            // Bokstaven direkt åt höger
                startIndex == ((lastIndex - BOARD_SIZE) + 1) ||                             // Bokstaven snett ovanför till höger
                startIndex == ((lastIndex + BOARD_SIZE) + 1);                               // Bokstaven snett nedanför till höger

    } else {                                                                                // Mittenbokstäverna är granne med
        return startIndex == (lastIndex - BOARD_SIZE - 1) ||                                // Alla  8 bokstäver runt om sig
                startIndex == (lastIndex - BOARD_SIZE) ||
                startIndex == (lastIndex - BOARD_SIZE + 1) ||
                startIndex == (lastIndex + 1) ||
                startIndex == (lastIndex + BOARD_SIZE + 1) ||
                startIndex == (lastIndex + BOARD_SIZE) ||
                startIndex == (lastIndex + BOARD_SIZE - 1) ||
                startIndex == (lastIndex - 1);
    }
}*/
