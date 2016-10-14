/*  Laboration 2
 *  jenli414 och sabse455
 */
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>
#include <stack>
#include <set>
#include <limits>

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
void setInitialLargestWordFamily(const set<string>& dictionary,
                                 const unsigned int& uWordLength,
                                 stack<string>& largestWordFamily);
void askForGuess(set<char>& alreadyGuessedLetters, char& guess);
void findLargestWordFamily(const unsigned int& uWordLength,
                           const set<char>& alreadyGuessedLetters,
                           const char& guess, stack<string>& largestWordFamily);
void checkBranches(queue<stack<string>>& wordFamilies, stack<string>& equalsGuess,
                   stack<string>& doesntEqualGuess);
void keepLargestWordFamily(queue<stack<string>>& wordFamilies);
bool wordComplete(set<char>& alreadyGuessedLetters, string& chosenWord);
void printGuessStatus(const unsigned int& uNumOfGuesses,
                      const set<char>& alreadyGuessedLetters,
                      const string& currentResult);
void printNumOfPossibleWords(const unsigned int& numOfPossibleWords);
void printEndMessage(const bool& playerWon, const string& chosenWord);
void askForUserAction(char& userAction);


/* The main method reads the dictionary and starts a game of hangman.
 * Afterwards it asks if the user wants to play again. */
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

// Reads the dictionary.txt file and puts the words in a set.
void readDictionary(set<string>& dictionary) {
    ifstream input;
    input.open("dictionary.txt");
    string word;
    while (getline(input, word)) {
        dictionary.insert(word);
    }
    input.close();
}

/* Sets shortestWord and longestWord to the length of the shortest and longest
 * word in the dictionary.*/
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

// Starts and plays a game of hangman.
void startGame(const set<string>& dictionary, const unsigned int& shortestWord,
               const unsigned int& longestWord) {
    int iWordLength, iNumOfGuesses = 0;
    askForWordLength(iWordLength, shortestWord, longestWord);
    unsigned int uWordLength = iWordLength;
    askForNumOfGuesses(iNumOfGuesses);
    unsigned int uNumOfGuesses = iNumOfGuesses;
    bool showNumOfPossibleWords;
    askIfShowNumOfPossibleWords(showNumOfPossibleWords);
    stack<string> largestWordFamily;
    setInitialLargestWordFamily(dictionary, uWordLength, largestWordFamily);
    char guess;
    set<char> alreadyGuessedLetters;
    bool playerWon = false;
    while (!playerWon && uNumOfGuesses > 0) {
        askForGuess(alreadyGuessedLetters, guess);
        --uNumOfGuesses;
        if (largestWordFamily.size() == 1) {
            alreadyGuessedLetters.insert(guess);
            playerWon = wordComplete(alreadyGuessedLetters, largestWordFamily.top());
        } else {
            findLargestWordFamily(uWordLength, alreadyGuessedLetters, guess,
                                   largestWordFamily);
            alreadyGuessedLetters.insert(guess);
            if (largestWordFamily.size() == 1) {
                playerWon = wordComplete(alreadyGuessedLetters, largestWordFamily.top());
            }
        }
        printGuessStatus(uNumOfGuesses, alreadyGuessedLetters,
                         largestWordFamily.top());
        if (showNumOfPossibleWords) {
            printNumOfPossibleWords(largestWordFamily.size());
        }
    }
    printEndMessage(playerWon, largestWordFamily.top());
}

/* Asks what wordlength the user wants. Has to be an integer between
 * the shortest and longest word in the dictionary.*/
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

// Asks how many guesses the user wants. Has to be an integer greater than 0.
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

bool isAllNumbers(const string& input) {
    for (string::const_iterator it = input.begin(); it != input.end(); ++it) {
        if (!isdigit(*it)) {
            return false;
        }
    }
    return true;
}

// Asks if the user wants to see how many possible words remain.
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

/* Sets the initial largest word family to the words in the dictionary
 * with the given wordlength. */
void setInitialLargestWordFamily(const set<string>& dictionary,
                                 const unsigned int& uWordLength,
                                 stack<string>& largestWordFamily) {
    for (auto const& word : dictionary) {
        if (word.length() == uWordLength) {
           largestWordFamily.push(word);
        }
    }
}

// Asks user for a valid guess.
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

/* Sets largestWordFamily to the largest possible word family depending on
 * what letter the player has guessed.*/
void findLargestWordFamily(const unsigned int& uWordLength,
                           const set<char>& alreadyGuessedLetters,
                           const char& guess, stack<string>& largestWordFamily) {
    queue<stack<string>> wordFamilies;
    wordFamilies.push(largestWordFamily);
    stack<string> currentWordFamily;
    string currentWord;
    stack<string> equalsGuess;
    stack<string> doesntEqualGuess;
    for (unsigned int count = 0; count < uWordLength; ++count) {
        int initialWordFamilies = wordFamilies.size();
        for (int i = 0; i < initialWordFamilies; ++i) {
            currentWordFamily = wordFamilies.front();
            wordFamilies.pop();
            if (alreadyGuessedLetters.count(currentWordFamily.top()[count])) {
                wordFamilies.push(currentWordFamily);
            } else {
                while (!currentWordFamily.empty()) {
                    currentWord = currentWordFamily.top();
                    currentWordFamily.pop();
                    if (currentWord[count] == guess) {
                        equalsGuess.push(currentWord);
                    } else {
                        doesntEqualGuess.push(currentWord);
                    }
                }
                checkBranches(wordFamilies, equalsGuess, doesntEqualGuess);
            }
        }

    }
    keepLargestWordFamily(wordFamilies);
    largestWordFamily = wordFamilies.front();
}

/* Checks if the resulting stacks of the sorting in findLargestWordFamily contain
 * any words and should be pushed back to the wordFamilies queue.
 * Also clears the stacks if needed.*/
void checkBranches(queue<stack<string>>& wordFamilies, stack<string>& equalsGuess,
                   stack<string>& doesntEqualGuess) {
    if (!(equalsGuess.empty())) {
        wordFamilies.push(equalsGuess);
        while (!(equalsGuess.empty())) {
            equalsGuess.pop();
        }
    }
    if (!(doesntEqualGuess.empty())) {
        wordFamilies.push(doesntEqualGuess);
        while (!(doesntEqualGuess.empty())) {
            doesntEqualGuess.pop();
        }
    }
}

// Removes every word family in wordFamilies but the largest one.
void keepLargestWordFamily(queue<stack<string>>& wordFamilies) {
    stack<string> largestWordFamily = wordFamilies.front();
    wordFamilies.pop();
    stack<string> currentFamily;
    while (!wordFamilies.empty()) {
        currentFamily = wordFamilies.front();
        if (currentFamily.size() > largestWordFamily.size()) {
            largestWordFamily = currentFamily;
        }
        wordFamilies.pop();
    }
    wordFamilies.push(largestWordFamily);
}

// Returns true if every letter in chosenWord has been guessed.
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

/* Prints remaining guesses, already guessed letters and the current
 * state of the game. */
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

// Prints number of possible words remaining.
void printNumOfPossibleWords(const unsigned int& numOfPossibleWords) {
    cout << "Possible words remaining: " << numOfPossibleWords << endl << endl;
}

// Prints end message. Differs depending on if the player won or not.
void printEndMessage(const bool& playerWon, const string& chosenWord) {
    if (playerWon) {
        cout << "Congratulations! You beat me even though I may or may not have cheated..."
             << endl;
    } else {
        cout << "You're out of guesses!" << endl;
    }
    cout << "The correct word was " << chosenWord << "!" << endl << endl;
}

// Ask the user if it wants to play again and sets userAction accordingly.
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
