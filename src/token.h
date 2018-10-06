#ifndef TOKEN_H
#define TOKEN_H

#include <vector>
#include <string>
#include <iostream>
#include "constants.hpp"
#include "expression.h"

/*
	The token class will take in a string
	and convert it into parsed, readable data
	
	One token can be turned into one map object
*/

class Token
{
public:
	Token();
	bool stringToObject(std::string s);//takes in a full object instantiation string and turns it into bytes
	std::vector<Uint8> getObject();//returns object in byte form
	void printError();
	std::string getError();
	void setLineNumber(int i);
	section getSection();
	void setRect(expRect r);
	expRect rectShift();
private:
	//boolean functions here return false on failure
	bool mParse();//split function into separate strings, removing delimiters
	bool mCheckType();//read the object name and give the token a type
	bool mEvaluate();//evaluate all
	bool mEvaluate(int i);//overload, evaluate single index

	int mLineNumber;
	expRect mRect;
	std::string mObjectString;//unparsed, unprocessed string
	std::vector<Uint8> mData;//the object data
	std::vector<std::string> mArgS;
	section mType;//the type of object, IE the section it belongs
	
	//wimpy helper functions
	bool mIsDelimiter(const char & c);
	bool mIsIgnore(const char & c);
	bool mIsLineCommentBegin(const char & c);
	bool mIsLineCommentEnd(const char & c);
	argtype mGetType(int i);
	bool mCheckChain(int i);//check if a single argument should be OR'd onto the previous argument
	bool mChain(Uint8 i);//chain to previous argument using ||
	
	//error handling
	std::string mErrorMessage;
	friend bool operator<(Token lhs, Token rhs);//for sorting
};

#endif
