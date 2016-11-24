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


int asciiNumToDecimal(char asciiNum) {
    return asciiNum - 48;
}


void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output) {
    string binaryCode;
    cout << input.readBit() << endl;
    int bit = input.readBit();
    cout << bit << endl;
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
        binaryCode = binaryCode.substr(1, binaryCode.size() - 1);
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
    output.put(123);                    //MAGIC NUMBER '{'
    map<int,int>::const_iterator mapIt = freqTable.begin();
    while (mapIt != freqTable.end()) {
        output.put(mapIt->first);
        output.put(58);                 //MAGIC NUMBER ':'
        string freq = to_string(mapIt->second);
        for (string::iterator it = freq.begin(); it != freq.end(); it++) {
            output.put(asciiNumToDecimal(*it));
        }
        mapIt++;
        if (mapIt != freqTable.end()) {
            output.put(44);         //MAGIC NUMBER ','
        }
    }
    output.put(125);                //MAGIC NUMBER '}'
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(encodingTree), output);
}




void decompress(ibitstream& input, ostream& output) {
    map<int,int> freqTable;
    int key;
    string freq;
    char nextChar = input.get();
    while (!endOfHeader(nextChar)) {
        key = input.get();
        input.get();
        nextChar = input.get();
        while (!endOfFreq(nextChar)) {
            freq += to_string(nextChar);
            nextChar = input.get();
        }
        freqTable[key] = stoi(freq);
        char keyChar = key;
        cout << "key: " << keyChar << " freq: " << stoi(freq) << endl;
        freq.clear();
    }
    decodeData(input, buildEncodingTree(freqTable), output);
}


/*
void decompress(ibitstream& input, ostream& output) {
    map<int,int> freqTable;
    string keyAscii;
    string keyDecimal;
    string freqAscii;
    string freqDecimal;
    char nextChar = input.get();
    while (!endOfHeader(nextChar)) {
        keyAscii += to_string(input.get());
        cout << "key: " << keyAscii << endl;
        nextChar = input.get();
        while (!endOfKey(nextChar)) {
            keyAscii += to_string(nextChar);
            cout << "key: " << keyAscii << endl;
            nextChar = input.get();
        } while (keyAscii.length() >= 2) {
            keyDecimal += to_string(asciiNumToDecimal(stoi(keyAscii.substr(0,2))));
            keyAscii = keyAscii.substr(2, keyAscii.size() - 2);
        }
        freqAscii += to_string(input.get());
        cout << "freq: " << freqAscii << endl;
        nextChar = input.get();
        while (!endOfFreq(nextChar)) {
            freqAscii += to_string(nextChar);
            cout << "freq: " << freqAscii << endl;
            nextChar = input.get();
        } while (freqAscii.length() >= 2) {
            freqDecimal += to_string(asciiNumToDecimal(stoi(freqAscii.substr(0,2))));
            freqAscii = freqAscii.substr(2, freqAscii.size() - 2);
        }
        freqTable[stoi(keyDecimal)] = stoi(freqDecimal);
        keyAscii.clear();
        keyDecimal.clear();ke
        freqAscii.clear();
        freqDecimal.clear();
    }
    decodeData(input, buildEncodingTree(freqTable), output);
}
*/


/*
void compress(istream& input, obitstream& output) {
    map<int,int> freqTable = buildFrequencyTable(input);
    output.put('{');
    map<int,int>::const_iterator mapIt = freqTable.begin();
    string key;
    string freq;
    while (mapIt != freqTable.end()) {
        key = to_string(mapIt->first);
        for (string::iterator it = key.begin(); it != key.end(); it++) {
            output.put(*it);
        }
        output.put(':');
        freq = to_string(mapIt->second);
        for (string::iterator it = freq.begin(); it != freq.end(); it++) {
            output.put(*it);
        }
        mapIt++;
        if (mapIt != freqTable.end()) {
            output.put(',');
        }
    }
    output.put('}');
    HuffmanNode* encodingTree = buildEncodingTree(freqTable);
    encodeData(input, buildEncodingMap(encodingTree), output);
}*/

bool endOfKey(const int& character) {
    return character == 58; // 58 = ':'
}


bool endOfFreq(const int& character) {
    return character == 44 || character == 125; // 44 = ',' och 125 = '}'
}


bool endOfHeader(const int& character) {
    return character == 125; // 125 = '}'
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
