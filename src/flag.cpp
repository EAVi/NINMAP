#include "flag.h"

using namespace std;

Flag::Flag(int argc, char* args[])
{
	mArgc = argc;
	mArgs = args;
	mOptionIndex = -1;
	mInputIndex = -1;
	mOutputIndex = -1;
	mFileOptions = 0xff;
	mErrorMessage = string();
	mOutputFile = string();
}

bool Flag::Evaluate()
{
	return (
		mParseIndex()
		&& mEvaluateOption());
}

Uint8 Flag::getFileOptions()
{
	return mFileOptions;
}

char* Flag::getInputFile()
{
	return mArgs[mInputIndex];
}

const char* Flag::getOutputFile()
{
	if (mOutputIndex < 0)
		return mOutputFile.c_str();//extended map filename
	return mArgs[mOutputIndex];
}

void Flag::printError()
{
	cout << mErrorMessage;
	cout << endl;
}

void Flag::verbose()
{
	string message;
	switch (mFileOptions & kInputMask)
	{
		case kCompileInput: 
			message = "Compile map";
			break;
		case kTupleInput:
			message = "Tuple map";
			break;
		case kBinaryInput:
			message = "Binary map";
			break;
	}
	
	message += " \"" ;
	message += mArgs[mInputIndex];
	message += "\" to ";

	switch (mFileOptions & kOutputMask)
	{
		case kCompileOutput:
			message += "Decompiled map";
			break;
		case kTupleOutput:
			message += "Tuple map";
			break;
		case kBinaryOutput:
			message += "Binary map";
			break;
	}
	
	if (mOutputIndex == -1)
	{
		message += " \"";
		message += mOutputFile;//generated output file
		message += "\"";
	}
	else 
	{
		message += " \"";
		message += mArgs[mOutputIndex];//given output file
		message += "\"";
	}
	
	cout << message << endl;
}

bool Flag::mParseIndex()
{
	if (mArgc > 4)//too many arguments
	{
		mErrorMessage = "Woah there; that's like - way too many arguments";
		return false;
	}
	
	ifstream ifile;
	for (int i = 1; i < mArgc; ++i)
	{
		//if the first character of the argument is a hypen, that's an option
		//an option is optional, not required
		if ((mOptionIndex == -1) && (mArgs[i][0] == '-'))
			mOptionIndex = i;
		else if (mInputIndex == -1)//if it's not an option first, then it MUST BE an input file first
		{
			//check if input file is valid
			ifile.open(mArgs[i]);
			if (!ifile)//invalid file
			{
				//return
				mErrorMessage = "Invalid input file \"";
				mErrorMessage += (string)mArgs[i];
				mErrorMessage += "\"";
				return false;
			}//invalid file
			
			mInputIndex = i;
			ifile.close();
		}
		else if (mOutputIndex == -1)
		{
			mOutputIndex = i;
		}
		else//if it's not input, output, or option, it shouldn't be there
		{
			mErrorMessage = "The argument \"";
			mErrorMessage += (string)mArgs[i];
			mErrorMessage += "\" is invalid";
			return false;
		}
	}
	
	if (mInputIndex == -1)
	{
		mErrorMessage = "Input File missing from arguments";
		return false;
	}

	return true;
}

bool Flag::mEvaluateOption()
{
	//no option, default to option -CT
	if (mOptionIndex == -1)
	{
		mFileOptions = kCompileInput | kTupleOutput;
		return true;
	}
	
	int inputchar = 1;
	int outputchar = strlen(mArgs[mOptionIndex]) - 1;
	mFileOptions = 0x00;
	
	//input option
	switch (mArgs[mOptionIndex][inputchar])
	{
		case 'C': 
			mFileOptions |= kCompileInput;
			break;
		case 'c': 
			mFileOptions |= kCompileInput;
			break;
		case 'T': 
			mFileOptions |= kTupleInput;
			break;
		case 't': 
			mFileOptions |= kTupleInput;
			break;
		case 'B': 
			mFileOptions |= kBinaryInput;
			break;
		case 'b': 
			mFileOptions |= kBinaryInput;
			break;
	}
	
	//output option
	switch (mArgs[mOptionIndex][outputchar])
	{
		case 'C': 
			mFileOptions |= kCompileOutput;
			break;
		case 'c': 
			mFileOptions |= kCompileOutput;
			break;
		case 'T': 
			mFileOptions |= kTupleOutput;
			break;
		case 't': 
			mFileOptions |= kTupleOutput;
			break;
		case 'B': 
			mFileOptions |= kBinaryOutput;
			break;
		case 'b': 
			mFileOptions |= kBinaryOutput;
			break;
	}
	
	if ((mFileOptions % 17) == 0) //false in cases 0x00 0x11 0x22 ...
	{
		mErrorMessage = "Same Input Option as Output Option";
		return false;
	}
	
	if ((mFileOptions >> 4) > 2 || //if the flagoutput is an undefined number
		(mFileOptions & 0x0f) > 2) //if the flaginput is an undefined number
	{
		mErrorMessage = "Undefined Options";
		return false;
	}
	
	//generate an output file string, file extension dependent on the option
	if (mOutputIndex == -1)
	{
		mExtendFile();
	}
	
	
	return true;
}

void Flag::mExtendFile()
{
	for (int i = 0, j = strlen(mArgs[mInputIndex]); i < j; ++i)
	{
		mOutputFile += mArgs[mInputIndex][i];
		if (mArgs[mInputIndex][i] == '.')
			break;
	}
	switch (mFileOptions & kOutputMask)
	{
		case kBinaryOutput: 
			mOutputFile += "bin";
			break;
		case kCompileOutput: 
			mOutputFile += "nin";
			break;
		case kTupleOutput: 
			mOutputFile += "tup";
			break;
			
	}
}