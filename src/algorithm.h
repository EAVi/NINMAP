#ifndef ALGORITHM_H
#define ALGORITHM_H

#include <string>
#include <vector>
#include "constants.hpp"

/*
	This file contains decompile/compile functions
	
	In the future, more functions will be moved out
	of their classes to make updating the compiler easier
*/

//lookup string, return int
Uint8 nameint(const std::string & s, const argtype & a);

//lookup int, return string
std::string namestring(const int & i, const argtype & a);

//returns the character array for lookup
stringref getnamearray(const argtype & a, int & size);

argtype getargtype(const section & s, const int & i);

const int* getargarray(const section & s, int & size);
int getargcount(const section & s);

bool checkchain(const section & s, const int & i);

//data to string.
//input some Uint8 data, advance counter
std::string datatostring(const std::vector<Uint8> & data, int & i, const argtype & a);


/*
	These functions need a reference to the data and index
	to increment i. Also, some of the functions use more than 
	one Uint8, such as the floating point using 4 bytes
*/

//blockname string, includes block direction and others
std::string blockname(const std::vector<Uint8> & data, int & i);

//all non-chaining data types
std::string datauint8(const std::vector<Uint8> & data, int & i);
std::string datasint16(const std::vector<Uint8> & data, int & i);
std::string datafloat(const std::vector<Uint8> & data, int & i);
std::string datasubp(const std::vector<Uint8> & data, int & i);

//chaining data types
std::string datahalfbyte(const std::vector<Uint8> & data, int & i, const argtype & a);
std::string databool(const std::vector<Uint8> & data, int & i, const argtype & a);

bool checkchain(int* typearray, int i);

#endif
