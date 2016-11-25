// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>

using namespace std;


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


map<int,string> buildEncodingMap(const HuffmanNode* encodingTree) {
    map<int,string> encodingMap;
    string code = "";
    if (encodingTree != nullptr) {
        buildEncodingMapHelper(encodingTree, encodingMap, code);
    }
    return encodingMap;
}


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


int asciiNumToDecimal(const char& asciiNum) {
    return asciiNum - 48;
}


/*
void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    HuffmanNode* startNode = encodingTree;
    HuffmanNode* currNode = encodingTree;
    int bit = input.readBit();
    int currChar = currNode->character;
    while (bit != -1) {
        if (currChar == NOT_A_CHAR) {
            if (bit == 0) {
                currNode = currNode->zero;
                currChar = currNode->character;
            } else {
                currNode = currNode->one;
            }
        } else {
            output.put(currChar);
            currNode = startNode;
            if (currChar == PSEUDO_EOF) {
                break;
            }
        }
        bit = input.readBit();
    }
}*/


void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    decodeDataHelper(input, encodingTree, encodingTree, output);
}

void decodeDataHelper(ibitstream& input, const HuffmanNode* startNode, const HuffmanNode* currNode, ostream& output) {
    int currChar = currNode->character;
    int bit = input.readBit();
    cout << bit << endl;
    if (bit != -1 && currChar != PSEUDO_EOF) {
        if (currChar == NOT_A_CHAR) {
            if (bit == 0) {
                decodeDataHelper(input, startNode, currNode->zero, output);
            } else {
                decodeDataHelper(input, startNode, currNode->one, output);
            }
        } else {
            output.put(currChar);
            decodeDataHelper(input, startNode, startNode, output);
        }
    }
}


//Från början, FUNKAR!

/*void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    string binaryCode;
    int bit = input.readBit();
    while (bit != -1) {
        binaryCode += to_string(bit);
        bit = input.readBit();
    }
    while (!binaryCode.empty()) {
        output.put(decodeChar(encodingTree, binaryCode));
    }
}*/


/*char decodeChar(const HuffmanNode* encodingTree, string& binaryCode) {
    int byte = encodingTree->character;
    if (byte == NOT_A_CHAR) {
        string nextChild = binaryCode.substr(0,1);
        binaryCode = binaryCode.substr(1, string::npos);
        if (nextChild == "0") {
            return decodeChar(encodingTree->zero, binaryCode);
        } else {
            return decodeChar(encodingTree->one, binaryCode);
        }
    } else if (byte == PSEUDO_EOF) {
        binaryCode.clear();
    }
    return byte;
}*/


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


bool isEndOfHeader(const char& character) {
    return character == '}';
}


bool isEndOfKey(const char& character) {
    return character == ':';
}


bool isEndOfFreq(const char& character) {
    return character == ',' || character == '}';
}


void freeTree(HuffmanNode* node) {
    if (node->character != NOT_A_CHAR) {
        delete node;
    } else {
        freeTree(node->zero);
        freeTree(node->one);
        delete node;
    }
}
