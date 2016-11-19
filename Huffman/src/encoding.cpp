// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>

using namespace std;

struct LessThanNode
{
  bool operator()(const HuffmanNode* lhs, const HuffmanNode* rhs) const
  {
    return lhs->count > rhs->count;
  }
};


// TODO: include any other headers you need



map<int, int> buildFrequencyTable(istream& input) {
    map<int, int> freqTable;
    char currChar;
    while (input.get(currChar)){
        if (!freqTable.count(currChar)) {
            freqTable[currChar] = 0;
        }
        freqTable[currChar]++;
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
    int bit = input.get();
    string code;
    while (bit != -1) {
        code = encodingMap.find(bit)->second;
        for (string::iterator it = code.begin(); it != code.end(); ++it) {
            output.writeBit(asciiCharToDecimal(*it));
        }
        bit = input.get();
    }
    code = encodingMap.find(PSEUDO_EOF)->second;
    for (string::iterator it = code.begin(); it != code.end(); ++it) {
        output.writeBit(asciiCharToDecimal(*it));
    }
}


int asciiCharToDecimal(char asciiChar) {
    return asciiChar - 48; // subtracts 48 to convert ascii value to decimal value.
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {

}

void compress(istream& input, obitstream& output) {
    // TODO: implement this function
}

void decompress(ibitstream& input, ostream& output) {
    // TODO: implement this function
}

void freeTree(HuffmanNode* node) {
    // TODO: implement this function
}






/*

HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>, LessThanNode> prioQueue;
    for (map<int,int>::const_iterator mapIt = freqTable.begin(); mapIt != freqTable.end(); ++mapIt) {
        prioQueue.push(new HuffmanNode(mapIt->first, mapIt->second));
    }
    while (prioQueue.size() > 1){
        HuffmanNode* leftNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode* rightNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode* mergedNode = new HuffmanNode(NOT_A_CHAR, leftNode->count
                                                  + rightNode->count, leftNode, rightNode);
        prioQueue.push(mergedNode);
    }
    return prioQueue.top();
}*/


/*
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable){
    priority_queue<HuffmanNode, vector<HuffmanNode>> prioQueue;
    for (map<int,int>::const_iterator mapIt = freqTable.begin(); mapIt != freqTable.end(); ++mapIt) {
        prioQueue.push(HuffmanNode(mapIt->first, mapIt->second));
    }
}


*/


/*
//UTAN PEKARE???
HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode, Comparator> prioQueue;
    for (map<int,int>::const_iterator mapIt = freqTable.begin(); mapIt != freqTable.end(); ++mapIt) {
        prioQueue.push(HuffmanNode(mapIt->first, mapIt->second));
    }
    HuffmanNode leftNode;
    HuffmanNode rightNode;
    while (prioQueue.size() > 1){
  //      HuffmanNode* leftNodePtr;
        HuffmanNode* leftNodePtr = new HuffmanNode(prioQueue.top());
       // leftNodePtr = &leftNode;
        prioQueue.pop();
        HuffmanNode* rightNodePtr = new HuffmanNode(prioQueue.top());
        //rightNode = prioQueue.top();
        //rightNodePtr = &rightNode;
        prioQueue.pop();
        HuffmanNode mergedNode(NOT_A_CHAR, leftNode.count + rightNode.count, leftNodePtr, rightNodePtr);
        prioQueue.push(mergedNode);
    }
    HuffmanNode* rootPtr;
    HuffmanNode rootNode = prioQueue.top();
    rootPtr = &rootNode;
    return rootPtr;
}*/
