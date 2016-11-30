// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>

using namespace std;


/*
 * Builds and returns a frequency table from given input.
 * I.e. a map<int,int> of every unique character value from input paired with
 * number of occurances.
 */
map<int,int> buildFrequencyTable(istream& input) {
    map<int,int> freqTable;
    int currByte = input.get();
    while (currByte != -1) {
        freqTable[currByte]++;
        currByte = input.get();
    }
    freqTable[PSEUDO_EOF] = 1;
    return freqTable;
}


/*
 * Builds an encoding tree from a frequency table, i.e. a tree of HuffmanNodes.
 * The characters are stored in the leaf positions of the tree. The parents and
 * the root of the tree don't store characters but the total frequency of all
 * characters in the leafs underneath it.
 */
HuffmanNode* buildEncodingTree(const map<int,int>& freqTable) {
    priority_queue<HuffmanNode,vector<HuffmanNode>> prioQueue;
    for (map<int,int>::const_iterator mapIt = freqTable.begin();
         mapIt != freqTable.end(); ++mapIt) {
        prioQueue.push(HuffmanNode(mapIt->first, mapIt->second));
    }
    while (prioQueue.size() > 1) {
        HuffmanNode leftNode = prioQueue.top();
        HuffmanNode* leftPtr = new HuffmanNode(leftNode.character, leftNode.count, leftNode.zero, leftNode.one);
        prioQueue.pop();
        HuffmanNode rightNode = prioQueue.top();
        HuffmanNode* rightPtr = new HuffmanNode(rightNode.character, rightNode.count, rightNode.zero, rightNode.one);
        prioQueue.pop();
        HuffmanNode mergedNode(NOT_A_CHAR, leftNode.count + rightNode.count, leftPtr, rightPtr);
        prioQueue.push(mergedNode);
    }
    HuffmanNode rootNode = prioQueue.top();
    HuffmanNode* rootPnt = new HuffmanNode(rootNode.character, rootNode.count, rootNode.zero, rootNode.one);
    return rootPnt;
}


/*
 * Builds an encoding map from an encoding tree, i.e a map with each character
 * (that is used >0 times) paired with the binary code that will lead to
 * that character in the encoding tree.
 */
map<int,string> buildEncodingMap(const HuffmanNode* encodingTree) {
    map<int,string> encodingMap;
    string code = "";
    if (encodingTree != nullptr) {
        buildEncodingMapHelper(encodingTree, encodingMap, code);
    }
    return encodingMap;
}


/*
 * Help-function to add all characters and corresponding binary codes to
 * the encoding map.
 */
void buildEncodingMapHelper(const HuffmanNode* encodingTree,
                            map<int,string>& encodingMap, string& code) {
    int character = encodingTree->character;
    if (character == NOT_A_CHAR){
        code += "0";
        buildEncodingMapHelper(encodingTree->zero, encodingMap, code);
        code.pop_back();
        code += "1";
        buildEncodingMapHelper(encodingTree->one, encodingMap, code);
        code.pop_back();
    }
    else {
        encodingMap[character] = code;
    }
}


/*
 * Uses an encodingMap to output the corresponding binary code of
 * each character from a given input.
 */
void encodeData(istream& input, const map<int,string>& encodingMap, obitstream& output) {
    string code;
    int byte = input.get();
    while (byte != -1) {
        code += encodingMap.find(byte)->second;
        byte = input.get();
    }
    code += encodingMap.find(PSEUDO_EOF)->second;
    for (string::iterator it = code.begin(); it != code.end(); ++it) {
        output.writeBit(asciiNumToDecimal(*it));
    }
}


/*
 * Takes an ASCII-character (0-9) value and returns its decimal value.
 */
int asciiNumToDecimal(const char& asciiNum) {
    return asciiNum - 48;
}


/*
 * Uses an encodingTree to translate given binary code input into the
 * corresponding character.
 */
void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    bool isEndOfFile = false;
    while (!isEndOfFile) {
        isEndOfFile = decodeDataHelper(input, encodingTree, output);
    }
}


/*
 * Reads input bit by bit and moves through the encodingTree until it
 * finds a character. Returns true if it reached the PSEUDO_EOF character
 * (end of file).
 */
bool decodeDataHelper(ibitstream& input, const HuffmanNode* currNode, ostream& output) {
    int currChar = currNode->character;
    if (currChar == NOT_A_CHAR) {
        if (input.readBit() == 0) {
            return decodeDataHelper(input, currNode->zero, output);
        } else {
            return decodeDataHelper(input, currNode->one, output);
        }
    } else {
        output.put(currChar);
        return currChar == PSEUDO_EOF;
    }
}


/*
 * Compresses given input. Adds a header of characters and frequencies.
 */
void compress(istream& input, obitstream& output) {
    map<int,int> freqTable = buildFrequencyTable(input);
    input.clear();
    input.seekg(0, ios::beg);
    output.put('{');
    string key;
    string freq;
    map<int,int>::const_iterator mapIt = freqTable.begin();
    while (mapIt != freqTable.end()) {
        key = to_string(mapIt->first);
        for (string::iterator keyIt = key.begin(); keyIt != key.end(); keyIt++) {
            output.put(asciiNumToDecimal(*keyIt));
        }
        output.put(':');
        freq = to_string(mapIt->second);
        for (string::iterator freqIt = freq.begin(); freqIt != freq.end(); freqIt++) {
            output.put(asciiNumToDecimal(*freqIt));
        }
        mapIt++;
        if (mapIt != freqTable.end()) {
            output.put(',');
        }
    }
    output.put('}');
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(encodingTree), output);
    freeTree(encodingTree);
}


/*
 * Decompresses given input.
 */
void decompress(ibitstream& input, ostream& output) {
    map<int,int> freqTable;
    string key;
    string freq;
    char nextChar = input.get();
    while (!isEndOfHeader(nextChar)) {
        key += to_string(input.get());
        nextChar = input.get();
        while(!isEndOfKey(nextChar)) {
            key += to_string(nextChar);
            nextChar = input.get();
        }
        freq += to_string(input.get());
        nextChar = input.get();
        while (!isEndOfFreq(nextChar)) {
            freq += to_string(nextChar);
            nextChar = input.get();
        }
        freqTable[stoi(key)] = stoi(freq);
        key.clear();
        freq.clear();
    }
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    decodeData(input, encodingTree, output);
    freeTree(encodingTree);
}


/*
 * Returns true if character equals '}' which means that it is
 * the end of the header.
 */
bool isEndOfHeader(const char& character) {
    return character == '}';
}


/*
 * Returns true if character equals ':' which means that it is
 * the end of the key.
 */
bool isEndOfKey(const char& character) {
    return character == ':';
}


/*
 * Returns true if character equals ',' or '}' which means that it is
 * the end of the frequency.
 */
bool isEndOfFreq(const char& character) {
    return character == ',' || character == '}';
}


/*
 * Frees the associated memory of node.
 */
void freeTree(HuffmanNode* node) {
    if (node->character != NOT_A_CHAR) {
        delete node;
    } else {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
