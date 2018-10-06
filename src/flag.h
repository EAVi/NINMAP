/*
	Flag is just a class that handles the CLI 
	arguments and converts them into usable information
*/

#ifndef FLAG_H
#define FLAG_H
#include "constants.hpp"
#include <string>
#include <iostream>
#include <fstream>
#include <cstring>

class Flag
{
public:
	Flag(int argc, char* args[]);
	bool Evaluate();//handle all the arguments and evaluate them, returns false on error
	Uint8 getFileOptions();
	char* getInputFile();
	const char* getOutputFile();
	void  printError();
	void verbose(); //prints out what was evaluated from the arguments,must call after evaluate
private:
	int mArgc;//from main
	char** mArgs;//from main
	int mOptionIndex;
	int mInputIndex;
	int mOutputIndex;
	Uint8 mFileOptions; //8 bit integer with instructions on whether to compile or decompile 
	std::string mErrorMessage;
	std::string mOutputFile;
	
	//figures out where everything is
	//return false on error
	bool mParseIndex();
	
	//reads the option (if there is one)
	//returns false on error
	bool mEvaluateOption();
	
	//give the output file an extended name
	void mExtendFile();
	
};

#endif
