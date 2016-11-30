/*  Laboration 2
 *  jenli414 och sabse455
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <set>
#include <limits>
#include <map>

using namespace std;

void readDictionary(set<string>& dictionary);
void getWordLengthLimits(const set<string>& dictionary, unsigned int& shortestWord,
                         unsigned int& longestWord);
void startGame(const set<string>& dictionary, const unsigned int& shortestWord,
               const unsigned int& longestWord);
void askForWordLength(int& iWordLength, const unsigned int& shortestWord,
                      const unsigned int& longestWord);
void askForNumOfGuesses(int& iNumOfGuesses);
bool isAllNumbers(const string& input);
void askIfShowNumOfPossibleWords(bool& showNumOfPossibleWords);
void setInitialRemainingWords(const set<string>& dictionary,
                                 const unsigned int& uWordLength,
                                 set<string>& remainingWords);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void findLargestWordFamily(set<string>& remainingWords, const int& uNumOfGuesses,
                           const char& guess, const int& wordLength);
bool wordComplete(set<char>& alreadyGuessedLetters, string& chosenWord);
void setNumOfGuesses(unsigned int& numOfGuesses, const char& guess,
                     const string& sampleWord);
void printGuessStatus(const unsigned int& uNumOfGuesses,
                      const set<char>& alreadyGuessedLetters,
                      const string& currentResult);
void printNumOfPossibleWords(const unsigned int& numOfPossibleWords);
void printEndMessage(const bool& playerWon, const string& chosenWord);
void askForUserAction(char& userAction);


/*
 * The main method reads the dictionary and starts a game of hangman.
 * Afterwards it asks if the user wants to play again.
 */
int main() {
    set<string> dictionary;
    readDictionary(dictionary);
    unsigned int shortestWord, longestWord = 0;
    getWordLengthLimits(dictionary, shortestWord, longestWord);
    cout << "Welcome to Hangman." << endl << endl;
    char userAction = 's';
    bool run = true;
    while(run) {//adasd
        switch(userAction) {
        case 's':
            startGame(dictionary, shortestWord, longestWord);
            askForUserAction(userAction);
            break;
        case 'q':
            run = false;
        }
    }
    return 0;
}

/*
 * Reads the dictionary.txt file and puts the words in a set.
 */
void readDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
    }
    input.close();
}

/*
 * Sets shortestWord and longestWord to the length of the sh                           ortest and longest
 * word in the dictionary.
 */
void getWordLengthLimits(const set<string>& dictionary, unsigned int& shortestWord,
                         unsigned int& longestWord) {
    string word;
    for (set<string>::const_iterator it =
         dictionary.begin(); it != dictionary.end(); ++it) {
        word = *it;//adasdrs
        if (shortestWord == 0) {
            shortestWord = word.length();
        } else if (word.length() < shortestWord) {
            shortestWord = word.length();
        }
        if (word.length() > longestWord) {
            longestWord = word.length();
        }
    }
}

/*
 * Starts and plays a game of hangman.
 */
void startGame(const set<string>& dictionary, const unsigned int& shortestWord,
               const unsigned int& longestWord) {
    int iWordLength, iNumOfGuesses = 0;
    askForWordLength(iWordLength, shortestWord, longestWord);
    unsigned int uWordLength = iWordLength;
    askForNumOfGuesses(iNumOfGuesses);
    unsigned int uNumOfGuesses = iNumOfGuesses;
    bool showNumOfPossibleWords;
    askIfShowNumOfPossibleWords(showNumOfPossibleWords);
    set<string> remainingWords;
    setInitialRemainingWords(dictionary, uWordLength, remainingWords);
    char guess;
    set<char> alreadyGuessedLetters;
    bool playerWon = false;
    set<string>::const_iterator wordIt = remainingWords.begin();
    string sampleWord= *wordIt;
    while (!playerWon && uNumOfGuesses > 0) {
        askForGuess(alreadyGuessedLetters, guess);       
        if (remainingWords.size() != 1) {
            findLargestWordFamily(remainingWords, uNumOfGuesses, guess, uWordLength);
            wordIt = remainingWords.begin();
            sampleWord = *wordIt;
        }
        alreadyGuessedLetters.insert(guess);
        playerWon = wordComplete(alreadyGuessedLetters, sampleWord);
        setNumOfGuesses(uNumOfGuesses, guess, sampleWord);
        printGuessStatus(uNumOfGuesses, alreadyGuessedLetters,
                         sampleWord);
        if (showNumOfPossibleWords) {
            printNumOfPossibleWords(remainingWords.size());
        }
    }
    printEndMessage(playerWon, sampleWord);
}

/*
 * Asks what wordlength the user wants. Has to be an integer between
 * the shortest and longest word in the dictionary.
 */
void askForWordLength(int& iWordLength, const unsigned int& shortestWord,
                      const unsigned int& longestWord) {
    cout << "Enter a word length between " << shortestWord << " and "
         << longestWord << ": ";
    string input;
    bool validInput = false;
    while (!validInput) {
        cin >> input;
        if (isAllNumbers(input)) {
            stringstream convert(input);
            convert >> iWordLength;
            if (iWordLength < 0) {
                cout << endl << "Enter a number BETWEEN " << shortestWord << " and "
                     << longestWord << "!: ";
            } else {
                unsigned int uWordLength = iWordLength;
                if ((uWordLength < shortestWord) || (uWordLength > longestWord)) {
                    cout << endl << "Enter a number BETWEEN " << shortestWord << " and "
                         << longestWord << "!: ";
                } else {
                    cout << endl;
                    validInput = true;
                }
            }
        } else {
            cout << endl << "Enter a NUMBER between " << shortestWord << " and "
                 << longestWord << "!: ";
        }
    }
}

/*
 * Asks how many guesses the user wants. Has to be an integer greater than 0.
 */
void askForNumOfGuesses(int& iNumOfGuesses) {
    cout << "Enter number of guesses: ";
    string input;
    bool validInput = false;
    while (!validInput) {
        cin >> input;
        if (isAllNumbers(input)) {
            stringstream convert(input);
            convert >> iNumOfGuesses;
            if (iNumOfGuesses > 1) {
                if (iNumOfGuesses > 26) {
                    iNumOfGuesses = 26;
                }
                cout << endl;
                validInput = true;
            }
        }
        if (!validInput) {
            cout << endl << "Enter a positive number!: ";
        }
    }
}

/*
 * Returns true if input contains only numbers.
 */
bool isAllNumbers(const string& input) {
    for (string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

/*
 * Asks if the user wants to see how many possible words remain.
 */
void askIfShowNumOfPossibleWords(bool& showNumOfPossibleWords) {
    cout << "Show remaining words? (y/n): ";
    string answer;
    bool validInput = false;
    while (!validInput) {
        cin >> answer;
        if (answer == "y") {
            showNumOfPossibleWords = true;
            cout << endl;
            validInput = true;
        } else if (answer == "n") {
            showNumOfPossibleWords = false;
            cout << endl;
            validInput = true;
        } else {
            cout << endl << "Enter y for yes and n for no!: ";
        }
    }
}

/*
 * Sets the initial remaining words to the words in the dictionary
 * with the given wordlength.
 */
void setInitialRemainingWords(const set<string>& dictionary,
                                 const unsigned int& uWordLength,
                                 set<string>& remainingWords) {
    for (auto const& word : dictionary) {
        if (word.length() == uWordLength) {
           remainingWords.insert(word);
        }
    }
}

/*
 * Asks user for a valid guess.
 */
void askForGuess(set<char>& alreadyGuessedLetters, char& guess) {
    string input;
    cout << "Enter a guess: ";
    bool validInput = false;
    while (!validInput) {
        cin >> input;
        if (input.length() != 1 || !(isalpha(*input.begin()))) {
            cout << endl << "Enter a LETTER!: ";
        } else {
            stringstream convert(input);
            convert >> guess;
            guess = tolower(guess);
            if (alreadyGuessedLetters.count(guess) == 1) {
                cout << endl << "You have already guessed that! Try again: ";
            } else {
                cout << endl;
                validInput = true;
            }
        }
    }
}

/*
 * Sets remainingWords to the largest possible word family depending on
 * what letter the player has guessed.
 */
void findLargestWordFamily(set<string>& remainingWords, const int& uNumOfGuesses,
                           const char& guess, const int& wordLength) {
    map<string,set<string>> wordFamilies;
    for (set<string>::iterator wordIt = remainingWords.begin();
         wordIt != remainingWords.end(); ++wordIt) {
        string currentWord = *wordIt;
        stringstream keyStream;
        for (int i = 0; i < wordLength; ++i) {
            if (currentWord[i] == guess) {
                keyStream << guess;
            }
            else {
                keyStream << '-';
            }
        }
        wordFamilies[keyStream.str()].insert(currentWord);
    }
    string emptyFamily;
    emptyFamily.insert(0, wordLength, '-');
    if (uNumOfGuesses == 1 && wordFamilies.count(emptyFamily)) {
        remainingWords = (*wordFamilies.find(emptyFamily)).second;
    } else {
        unsigned int currLargestSize = 0;
        for (map<string,set<string>>::iterator mapIt = wordFamilies.begin();
             mapIt != wordFamilies.end(); ++mapIt) {
            if (mapIt->second.size() > currLargestSize) {
                currLargestSize = mapIt->second.size();
                remainingWords = mapIt->second;
            }
        }
    }
}


/*
 *  Returns true if every letter in chosenWord has been guessed.
 */
bool wordComplete(set<char>& alreadyGuessedLetters, string& chosenWord) {
    bool wordComplete = true;
    for (string::const_iterator it = chosenWord.begin();
         it < chosenWord.end(); ++it) {
        if (!alreadyGuessedLetters.count(*it)) {
            wordComplete = false;
        }
    }
    return wordComplete;
}

/*
 * Decrements uNumOfGuesses if the user guessed wrong, else does nothing.
 */
void setNumOfGuesses(unsigned int& numOfGuesses, const char& guess, const string& sampleWord) {
    if (sampleWord.find(guess) == string::npos) {
        --numOfGuesses;
    }
}

/*
 * Prints remaining guesses, already guessed letters and the current
 * state of the game.
 */
void printGuessStatus(const unsigned int& uNumOfGuesses,
                      const set<char>& alreadyGuessedLetters,
                      const string& currentResult) {
    cout << "Remaining guesses: " << uNumOfGuesses << endl;
    cout << "Guessed letters: ";
    for (set<char>::const_iterator it = alreadyGuessedLetters.begin();
         it != alreadyGuessedLetters.end(); ++it) {
        cout << *it << " ";
    }
    stringstream visibleToPlayer;
    for (string::const_iterator it = currentResult.begin();
         it != currentResult.end(); ++it) {
        if (alreadyGuessedLetters.count(*it)) {
            visibleToPlayer << *it << " ";
        } else {
            visibleToPlayer << "_ ";
        }
    }
    cout << endl << visibleToPlayer.str() << endl << endl;
}

/*
 * Prints number of possible words remaining.
 */
void printNumOfPossibleWords(const unsigned int& numOfPossibleWords) {
    cout << "Possible words remaining: " << numOfPossibleWords << endl << endl;
}

/*
 * Prints end message. Differs depending on if the player won or not.
 */
void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {
        cout << "Congratulations! You beat me even though I may or may not have cheated..."
             << endl;
    } else {
        cout << "You're out of guesses!" << endl;
    }
    cout << "The correct word was " << chosenWord << "!" << endl << endl;
}

/*
 * Ask the user if it wants to play again and sets userAction accordingly.
 */
void askForUserAction(char& userAction) {
    cout << "Play again? (y/n): ";
    string answer;
    bool validInput = false;
    while (!validInput) {
        cin >> answer;
        if (answer == "y") {
            userAction = 's';
            cout << endl;
            validInput = true;
        } else if (answer == "n") {
            userAction = 'q';
            cout << endl;
            validInput = true;
        } else {
            cout << endl << "Enter y for yes and n for no!: ";
        }
    }
}
