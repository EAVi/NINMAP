#include <iostream>
#include "constants.hpp"
#include "flag.h"
#include "compiler.h"
#include "token.h"
#include "algorithm.h"

using namespace std;

char helpMessage[]= "\nninmap inputfile (-conversion) (outputfile)"
	"\n\tConversion: [format1]to[format2]"
	"\n\nFile formats:"
	"\n\tB - binary file type"
	"\n\tC - compile file type, check out the language defined ./language/"
	"\n\tT - tuple file type"
	"\n\tIf conversion option is not specified, -CtoT is assumed"//default option will change soon
	"\n\nsample usage: \"ninmap -CtoB input.txt output.bin\"\n";

int main (int argc, char* args[])
{
	//handle arguments
	if (argc == 1)
	{
		cout << helpMessage;
		return 0;
	}
	Flag arghandle(argc, args);
	if (!arghandle.Evaluate())
	{
		cout << "Error: ";
		arghandle.printError();
		return 1;
	}
	else
		cout << "Arguments Successfully Handled" <<  endl;
	arghandle.verbose();
	
	Compiler compiler(arghandle.getFileOptions(), arghandle.getInputFile(), arghandle.getOutputFile());
	if (!compiler.compile())
	{
		cout << "Error: ";
		compiler.printError();
		cout << "Conversion failed." << endl;
		return 1;
	}
	
	cout << "Map file succesfully converted.";
	return 0;
}