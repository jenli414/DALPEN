// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>

using namespace std;
/*
struct LessThanNode
{
  bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) const
  {
    return lhs->count > rhs->count;
  }
};*/


map<int,int> buildFrequencyTable(istream& input) {
    map<int,int> freqTable;
    char currChar;
    while (input.get(currChar)){
        if (!freqTable.count(currChar)) {
            freqTable[currChar] = 0;
        }
        freqTable[currChar]++;
    }
    freqTable[PSEUDO_EOF] = 1;
    input.clear();
    input.seekg(0, ios::beg);
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
    cout << endl;
}


int asciiNumToDecimal(char asciiNum) {
    return asciiNum - 48;
}


void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    string binaryCode;
    int bit = input.readBit();
    while (bit != -1) {
        binaryCode += to_string(bit);
        bit = input.readBit();
    }
    while (!binaryCode.empty()) {
        output.put(decodeChar(encodingTree, binaryCode));
    }
}


char decodeChar(const HuffmanNode* encodingTree, string& binaryCode) {
    int character = encodingTree->character;
    if (character == NOT_A_CHAR) {
        string nextChild = binaryCode.substr(0,1);
        binaryCode = binaryCode.substr(1, string::npos);
        if (nextChild == "0") {
            return decodeChar(encodingTree->zero, binaryCode);
        } else {
            return decodeChar(encodingTree->one, binaryCode);
        }
    } else if (character == PSEUDO_EOF) {
        binaryCode.clear();
    }
    return character;
}


void compress(istream& input, obitstream& output) {
    map<int,int> freqTable = buildFrequencyTable(input);
    map<int,int>::iterator mapIt = freqTable.find(PSEUDO_EOF);
    freqTable.erase(mapIt);
    output.put('{');
    string freq;
    mapIt = freqTable.begin();
    while (mapIt != freqTable.end()) {
        output.put(mapIt->first);
        output.put(':');
        freq = to_string(mapIt->second);
        for (string::iterator it = freq.begin(); it != freq.end(); it++) {
            output.put(asciiNumToDecimal(*it));
        }
        mapIt++;
        if (mapIt != freqTable.end()) {
            output.put(',');
        }
    }
    output.put('}');
    freqTable[PSEUDO_EOF] = 1;
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(encodingTree), output);
}




void decompress(ibitstream& input, ostream& output) {
    map<int,int> freqTable;
    if (!isEmptyHeader(input)) {
        int key;
        string freq;
        char nextChar = input.get();
        while (!isEndOfHeader(nextChar)) {
            key = input.get();
            input.get();
            nextChar = input.get();
            while (!isEndOfFreq(nextChar)) {
                freq += to_string(nextChar);
                nextChar = input.get();
            }
            freqTable[key] = stoi(freq);
            freq.clear();
        }
    }
    freqTable[PSEUDO_EOF] = 1;
    decodeData(input, buildEncodingTree(freqTable), output);
}


bool isEmptyHeader(ibitstream& input) {
    input.get();
    char secondChar = input.get();
    input.clear();
    input.seekg(0, ios::beg);
    return isEndOfHeader(secondChar);
}


bool isEndOfFreq(const int& character) {
    return character == 44 || character == 125; // 44 = ',' and 125 = '}'
}


bool isEndOfHeader(const int& character) {
    return character == 125; // 125 = '}'
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}
