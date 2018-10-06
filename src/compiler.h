#ifndef COMPILER_H
#define COMPILER_H

#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include "constants.hpp"
#include "token.h"
#include "expression.h"

class Compiler
{
public:
	Compiler(Uint8 IOflag, char* argInput, const char* argOutput);
	bool compile();
	void printError();
private:
	std::vector<Uint8> mData;
	std::vector<Token> mToken;//for compiling, -Cto[format2], a sorted token array is used to create map data
	std::vector<std::string> mStringObjects;//for decompiling
	
	Uint8 mFlag;
	char* mInput;//filename string
	const char* mOutput;//filename string

	//helper functions
	bool mReadB();//Read binary file to binary data
	bool mReadC();//Read compile file to binary data
	bool mReadT();//Read tuple file to binary data
	void mWriteB();//Write to binary
	void mWriteC();//Write to compile
	void mWriteT();//Write to tuple	
	
	std::string mDataToString(int & i, section & s);
	
	std::string mErrorMessage;
};

#endif
