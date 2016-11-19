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
 * Returns a map of the frequency of the diffrent characters
 * in the given input istream.
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
 * Builds an encoding map from an encoding tree, i.e a map with each character
 * (that is used >0 times) paired with the binary code that will lead to
 * that character in the encoding tree.
 */
map<int, string> buildEncodingMap(const HuffmanNode* encodingTree);


/*
 * Help-function to add all characters and corresponding binary codes to
 * the encoding map.
 */
void buildEncodingMapHelper(const HuffmanNode* encodingTree, map<int,string>& encodingMap, string& code);


/*
 * Used an encodingMap to output the corresponding binary code of each character
 * from a given input.
 */
void encodeData(istream& input, const map<int,string>& encodingMap, obitstream& output);


/*
 * Takes an ASCII-character and returns its decimal value.
 */
int asciiCharToDecimal(char asciiChar);


/*
 *
 */
void decodeData(ibitstream& input, HuffmanNode* encodingTree, ostream& output);


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
void freeTree(HuffmanNode* node);


#endif
