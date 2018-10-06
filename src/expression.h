#ifndef EXPRESSION_H
#define EXPRESSION_H

#include <vector>
#include <string>
#include <iostream>
#include "constants.hpp"
#include "algorithm.h"
/*
	Expression will be able to read a string expression
	and return a byte or more of information
	
	Uses recursive-descent parsing to evaluate expression
	
	an expression may depend on variables, 
	meaning expressions cannot be done in parallel
*/

//rectangle structure that is returned from a previous object
struct expRect
{
	int x1;
	int y1;
	int x2;
	int y2;
};

class Expression
{
public:
	Expression();
	void setLineNumber(int i);
	void setRect(expRect r);
	void setType(argtype a);
	bool expr(std::string s);
	std::vector<Uint8> getData();//returns data in the corect format
	
	bool checkWarning();
	void printWarning();
	void printError();
	std::string getError();
	void printData();
private:
	std::string mString;
	int mIndex;
	int mStringSize;
	double mData;
	int mLineNumber;
	expRect mRect;
	argtype mArgType;
	
	bool mError;
	bool mWarning;
	std::string mErrorMessage;
	std::string mWarningMessage;
	
	/*
	Here's a breakdown of PEMDAS
	factor - Parentheses and numbers
	term - Multiply and Divide
	sum - Add and Subtract
	
	The following functions are a recursive descent parser
	*/
	double factor();
	double term();
	double sum();//more accurately called "expression",  but that would get confusing
	
	double number();
	double name();
};


#endif