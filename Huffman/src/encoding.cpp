// This is the CPP file you will edit and turn in.
// Also remove these comments here and add your own, along with
// comments on every function and on complex code sections.
// TODO: remove this comment header

#include "encoding.h"
#include <queue>
#include <vector>:
using namespace std;





// TODO: include any other headers you need

/*
 * Returns a map of the frequency of the diffrent characters
 * in the given input istream
 */
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





HuffmanNode* buildEncodingTree(const map<int, int> &freqTable) {
    priority_queue<HuffmanNode*, vector<HuffmanNode*>> prioQueue;
    for (map<int,int>::const_iterator mapIt = freqTable.begin(); mapIt != freqTable.end(); ++mapIt) {
        prioQueue.push(new HuffmanNode(mapIt->first, mapIt->second));
    }
    while (prioQueue.size() > 1){
        HuffmanNode* leftNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode* rightNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode* mergedNode = new HuffmanNode(NOT_A_CHAR, leftNode->count + rightNode->count, leftNode, rightNode);
        prioQueue.push(mergedNode);
    }
    return prioQueue.top();
}







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

/*

void buildEncodingTreeHelper(priority_queue<HuffmanNode> &prioQueue){
    if (prioQueue.size() > 1){
        HuffmanNode leftNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode rightNode = prioQueue.top();
        prioQueue.pop();
        HuffmanNode mergedNode(NOT_A_CHAR, leftNode.count + rightNode.count, leftNode, rightNode);
        prioQueue.push(mergedNode);
        buildEncodingTreeHelper(prioQueue0);
    }
}
*/

map<int, string> buildEncodingMap(HuffmanNode* encodingTree) {
    // TODO: implement this function
    map<int, string> encodingMap;
    return encodingMap;
}

void encodeData(istream& input, const map<int, string> &encodingMap, obitstream& output) {
    // TODO: implement this function
}

void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output) {
    // TODO: implement this function
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
