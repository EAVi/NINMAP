#include <iostream>
#include <string>
#include <vector>
#include "expression.h"

using namespace std;

int main(int argc, char* args[])
{
	string s = "";
	expRect r = {1,2,3,4};
	Expression e;
	e.setRect(r);
	e.setType(kSInt16Arg);
	if (argc < 2) 
	{
		cout << "enter an expression\n";
		return 0;
	}
	else
	{
		for (int i = 1; i < argc; ++i)
			s += args[i];
	}
	if (e.expr(s))
		e.printData();
	else
	{
		e.printError();
	}
	vector<Uint8> tempvec = e.getData();
	if (e.checkWarning())
	{
		cout << "Warning: ";
		e.printWarning();
	}

	return 0;
}