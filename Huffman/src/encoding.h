/*
 * TDDD86 Huffman Encoding
 * This file declares the functions that you will need to write in this
 * assignment for your Huffman Encoder in huffmanencoding.cpp.
 *
 * Please do not modify this provided file. Your turned-in files should work
 * with an unmodified version of all provided code files.
 */

#ifndef _encoding_h
#define _encoding_h

#include <iostream>
#include <string>
#include <map>
#include "bitstream.h"
#include "HuffmanNode.h"
using namespace std;

/*
 * Builds and returns a frequency table from given input.
 * I.e. a map<int,int> of every unique byte value from input paired with number
 * of occurances.
 */
map<int, int> buildFrequencyTable(istream& input);


/*
 * Builds an encoding tree from a frequency table, i.e. a tree of HuffmanNodes.
 * The characters are stored in the leaf positions of the tree. The parents and
 * the root of the tree don't store characters but the total frequency of all
 * characters in the leafs underneath it.
 */
HuffmanNode* buildEncodingTree(const map<int,int>& freqTable);


/*
 * Builds an encoding map from an encoding tree, i.e a map with each byte
 * (that is used >0 times) paired with the binary code that will lead to
 * that byte in the encoding tree.
 */
map<int, string> buildEncodingMap(const HuffmanNode* encodingTree);


/*
 * Help-function to add all bytes and corresponding binary codes to
 * the encoding map.
 */
void buildEncodingMapHelper(const HuffmanNode* encodingTree, map<int,string>& encodingMap, string& code);


/*
 * Uses an encodingMap to output the corresponding binary code of each byte
 * from a given input.
 */
void encodeData(istream& input, const map<int,string>& encodingMap, obitstream& output);


/*
 * Takes an ASCII-character (0-9) value and returns its decimal value.
 */
int asciiNumToDecimal(const char& asciiNum);


/*
 * Uses an encodingTree to translate given binary code input into the
 * corresponding bytes.
 */
void decodeData(ibitstream& input, const HuffmanNode* encodingTree, ostream& output);


/*
 *
 */
//char decodeChar(const HuffmanNode* encodingTree, string& binaryCode);

void decodeDataHelper(ibitstream& input, const HuffmanNode* startNode, const HuffmanNode* currNode, ostream& output);


/*
 *
 */
void compress(istream& input, obitstream& output);


/*
 *
 */
void decompress(ibitstream& input, ostream& output);


/*
 *
 */
bool isEndOfHeader(const char& character);


/*
 *
 */
bool isEndOfKey(const char& character);


/*
 *
 */
bool isEndOfFreq(const char& character);


/*
 *
 */
void freeTree(HuffmanNode* node);


#endif
