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
 * I.e. a map<int,int> of every unique character value from input paired with
 * number of occurances.
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
void buildEncodingMapHelper(const HuffmanNode* encodingTree,
                            map<int,string>& encodingMap, string& code);


/*
 * Uses an encodingMap to output the corresponding binary code of
 * each character from a given input.
 */
void encodeData(istream& input, const map<int,string>& encodingMap,
                obitstream& output);


/*
 * Takes an ASCII-character (0-9) value and returns its decimal value.
 */
int asciiNumToDecimal(const char& asciiNum);


/*
 * Uses an encodingTree to translate given binary code input into the
 * corresponding character.
 */
void decodeData(ibitstream& input, const HuffmanNode* encodingTree,
                ostream& output);


/*
 * Reads input bit by bit and moves through the encodingTree until it
 * finds a character. Returns true if it reached the PSEUDO_EOF character
 * (end of file).
 */
bool decodeDataHelper(ibitstream& input, const HuffmanNode* currNode,
                      ostream& output);


/*
 * Compresses given input. Adds a header of characters and frequencies.
 */
void compress(istream& input, obitstream& output);


/*
 * Decompresses given input.
 */
void decompress(ibitstream& input, ostream& output);


/*
 * Returns true if character equals '}' which means that it is
 * the end of the header.
 */
bool isEndOfHeader(const char& character);


/*
 * Returns true if character equals ':' which means that it is
 * the end of the key.
 */
bool isEndOfKey(const char& character);


/*
 * Returns true if character equals ',' or '}' which means that it is
 * the end of the frequency.
 */
bool isEndOfFreq(const char& character);


/*
 * Frees the associated memory of node.
 */
void freeTree(HuffmanNode* node);


#endif
