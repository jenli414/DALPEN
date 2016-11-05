// This is the .cpp file you will edit and turn in.
// We have provided a minimal skeleton for you,
// but you must finish it as described in the spec.
// Also remove these comments here and add your own.
// TODO: remove this comment header and replace it with your own

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
}


/*
 * Constructor for pre-determined board. (For testing)
 * Pre-condition: board must be of correct length (pow(BOARD_SIZE,2))
 */
Boggle::Boggle(const string& board) {
    string boardUpper = board;
    transform(boardUpper.begin(), boardUpper.end(), boardUpper.begin(), ::toupper);
    m_board = boardUpper;
}


/*
 * Deconstructor.
 */
Boggle::~Boggle() {

}


/*
 * Prepares a new game of Boggle.
 */
void Boggle::newGame(){
    setRandomBoard();
    m_playerFound.clear();
    m_NPCFound.clear();
    for (int i = 0; i < 26; ++i) {
        vector<string> playerFoundVector;
        vector<string> NPCFoundVector;
        m_playerFound.push_back(playerFoundVector);
        m_NPCFound.push_back(NPCFoundVector);
    }
}


/*
 * Returns the number of letters in a valid board.
 */
int Boggle::getNumOfLettersReq() const {
    return pow(BOARD_SIZE,2);
}


/*
 * Returns the string representation of the current board.
 */
string Boggle::getBoard() const {
    return m_board;
}


/*
 * Sets the following information about the game state in the given variables:
 * numOfWordsFound: The number of words the player has found.
 * foundWordsStr: String representation of all words the player has found.
 * score: Player's current score.
 */
void Boggle::setPlayerStatus(int& numOfWordsFound, string& foundWordsStr, int& score) const {
    numOfWordsFound = 0;
    foundWordsStr = "{\"";
    score = 0;
    for (int i = 0; i < 26; ++i) {
        for (vector<string>::const_iterator it = m_playerFound[i].begin(); it != m_playerFound[i].end(); ++it) {
            numOfWordsFound++;
            foundWordsStr += *it + "\", \"";
            score += (*it).length() - 3;
        }
    }
    if (numOfWordsFound == 0) {
        foundWordsStr = "{}";
    } else {
        foundWordsStr = foundWordsStr.substr(0, foundWordsStr.size()-3);
        foundWordsStr.append("}");
    }
}


/*
 * Sets m_board to given board.
 */
void Boggle::setBoard(const string& board) {
    m_board = board;
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
bool Boggle::isInBoard(const string word) const {
    set<int> takenIndices;
    return isInBoardHelper(0, word, takenIndices);
}


/*
 * Returns true if given word is in board. (Recursive helper)
 */
bool Boggle::isInBoardHelper(int startIndex, string word, set<int> takenIndices, int lastIndex) const {
    if (word.length() == 1) {
        return word[0] == m_board[startIndex] && takenIndices.find(startIndex) == takenIndices.end() && areNeighbours(startIndex, lastIndex);

    } else if (word[0] == m_board[startIndex] && takenIndices.find(startIndex) == takenIndices.end() && areNeighbours(startIndex, lastIndex)) {
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
}


/*
 * Returns true if startIndex and lastIndex are neighbours in board.
 * I.e if moving between those indices could lead to a valid word.
 */
bool Boggle::areNeighbours(const int& startIndex, const int& lastIndex) const {
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
}


/*
 * Returns true if word is in m_dictionary.
 */
bool Boggle::isInDictionary(const string& word) const {
    return m_dictionary.contains(word);
}


/*
 * Returns true if word is not in m_playerFound
 * NOTE: This function does not check if word is in
 * m_dictionary.
 * Precondition: Assumes that word is in upper case.
 */
bool Boggle::isNewWord(const string& word) const {
    for (unsigned int i = 0; i < 26; i++) {
        for (vector<string>::const_iterator it = m_playerFound[i].begin();
             it != m_playerFound[i].end(); ++it) {
            if (*it == word) {
                return false;
            }
        }
    }
    return true;
}


/*
 * Inserts a word into m_playerFound.
 * Precondition: We assume that word fulfills every criteria to count
 * as a valid word as well as being upper case.
 */
void Boggle::addToPlayerFound(const string& word) {
    int index = static_cast<int>(word[0]) - 65;
    m_playerFound[index].push_back(word);
}


/*
 * Takes a string and sets it to a random, valid board.
 */
void Boggle::setRandomBoard() {
    m_board = "";                                               // Resets m_board.
    shuffle(CUBES, NUM_CUBES);                                  // Shuffles CUBES.
    random_device rd;                                           // Get random num from hardware.
    mt19937 eng(rd());                                          // Seed the generator.
    for (unsigned int i = 0; i < NUM_CUBES; ++i) {
        uniform_int_distribution<> distr(0,(CUBE_SIDES - 1));   // Define the range (inclusive).
        m_board += CUBES[i][distr(eng)];                        // Generates a number and adds new letter to board.
    }
}
