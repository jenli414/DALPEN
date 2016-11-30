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


map<int, int> buildFrequencyTable(istream& input);


HuffmanNode* buildEncodingTree(const map<int,int>& freqTable);


map<int, string> buildEncodingMap(const HuffmanNode* encodingTree);


void buildEncodingMapHelper(const HuffmanNode* encodingTree,
                            map<int,string>& encodingMap, string& code);


void encodeData(istream& input, const map<int,string>& encodingMap,
                obitstream& output);


int asciiNumToDecimal(const char& asciiNum);


void decodeData(ibitstream& input, const HuffmanNode* encodingTree,
                ostream& output);


bool decodeDataHelper(ibitstream& input, const HuffmanNode* currNode,
                      ostream& output);


void compress(istream& input, obitstream& output);


void decompress(ibitstream& input, ostream& output);


bool isEndOfHeader(const char& character);


bool isEndOfKey(const char& character);


bool isEndOfFreq(const char& character);


void freeTree(HuffmanNode* node);


#endif
