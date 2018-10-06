#include "compiler.h"

using namespace std;

Compiler::Compiler(Uint8 IOflag, char* argInput, const char* argOutput)
{
	mFlag = IOflag;
	mInput = argInput;
	mOutput = argOutput;
	mData = vector<Uint8>();
	mToken = vector<Token>();
	
}

bool Compiler::compile()
{
	bool success = true;

	//input
	switch(mFlag & kInputMask)
	{
		case kCompileInput:
			success &= mReadC();
			break;
		case kTupleInput:
			success &= mReadT();
			break;
		case kBinaryInput:
			success &= mReadB();
			break;
		default:
			mErrorMessage = "Input Flag undefined";
			return false;
	}
	
	if (!success)//on read failure, do not write to file
		return success;
	
	switch(mFlag & kOutputMask)
	{
		case kCompileOutput:
			mWriteC();
			break;
		case kTupleOutput:
			mWriteT();
			break;
		case kBinaryOutput:
			mWriteB();
			break;
		default:
			mErrorMessage = "Output Flag undefined";
			return false;
	}
	
	return success;
}

void Compiler::printError()
{
	cout << mErrorMessage << endl;
}

bool Compiler::mReadB()
{
	ifstream ifile;
	
	ifile.open(mInput, std::ifstream::in | std::ifstream::binary);
	if (!ifile)
	{
		mErrorMessage = "File not found: ";
		mErrorMessage += (string)mInput;
		return false;
	}
	char tempchar = ifile.get();
	while (ifile.good())
	{
		mData.push_back(tempchar);
		tempchar = ifile.get();
	}
	return true;
}

bool Compiler::mReadC()
{
	int linecount = 1;
	char tempchar = '\0';
	string tempstring = "";
	bool linecomment = false;
	expRect temprect = {-1,-1,-1,-1};
	
	//until function complete
	//mErrorMessage = "mReadC function incomplete";
	//return false;
	
	ifstream ifile;
	ifile.open(mInput, std::ifstream::in);
	if (!ifile)
	{
		mErrorMessage = "File not found: ";
		mErrorMessage += (string)mInput;
		return false;
	}
	
	tempchar = ifile.get();
	//read file and convert to token vector
	while (ifile.good())
	{
		if (tempchar == '#')
		{
			linecomment = true;
		}
		if (tempchar == ';' 
			&& !linecomment
			&& tempstring != "")
		{
			Token temptoken;
			temptoken.setLineNumber(linecount);
			temptoken.setRect(temprect);
			if (!temptoken.stringToObject(tempstring))
			{
				mErrorMessage = temptoken.getError();
				return false;
			}
			temprect = temptoken.rectShift();//modifies the rectangle based on block placement
			mToken.push_back(temptoken);
			tempstring = "";
			
		}
		if (tempchar == '\n' || tempchar == '\r')
		{
			linecount++;
			linecomment = false;
		}
		if (!linecomment)
			tempstring += tempchar;
		
		tempchar = ifile.get();
	}
	ifile.close();

	//sort token vector
	//stable sort to preserve order of backgrounds and others
	stable_sort(mToken.begin(), mToken.end());
	
	//output the data, and add "255" to separate sections
	for (int i = 0, j = mToken.size(); i < j; ++i)
	{
		//push token data onto mdata vector
		vector<Uint8> tempvec = mToken[i].getObject();
		mData.insert(mData.end(), tempvec.begin(), tempvec.end());
		
		//check for section splits
		int next = i + 1;
		if (next == j)//int i is at last index
		{
			mData.push_back((Uint8)255);
		}
		else if ( mToken[i].getSection() != kAttributes //attribute is a singleton, does not need a separator
			&&(mToken[i] < mToken[next]))//if next token is a different value (section)
		{
			//loop in case more than one section is skipped
			//it's a bit long, but all it does is count the number of sections between i and next
			for (int k = (int)mToken[i].getSection(), l = (int)mToken[next].getSection(); k < l; ++k)
				mData.push_back((Uint8)255);
		}
	}
	
	
	return true;
}

bool Compiler::mReadT()
{
	ifstream ifile;
	
	ifile.open(mInput, std::ifstream::in);
	if (!ifile)
	{
		mErrorMessage = "File not found: ";
		mErrorMessage += (string)mInput;
		return false;
	}
	
	char tempchar = ifile.get();
	string numstring = "";
	bool linecomment = false;
	while (ifile.good())
	{
		//number reading logic
		//reads one character at a time to build a string to convert to integer
		if (tempchar >= '0' && tempchar <= '9' && !linecomment)
			numstring += tempchar;
		else if (numstring != "")//when enough characters make a number string
		{
			mData.push_back((Uint8)stoi(numstring, nullptr, 10));
			numstring = "";
		}
	
		//line comment logic
		if (tempchar =='n' || tempchar == '\r')//endline ends a linecomment
			linecomment = false;
		else if (tempchar == kLineCommentSymbol)
			linecomment = true;
		
		tempchar = ifile.get();
	}
	
	//read one last character
	if (tempchar >= '0' && tempchar <= '9' && !linecomment)
		numstring += tempchar;
	else if (numstring != "")//when enough characters make a number string
	{
		mData.push_back((Uint8)stoi(numstring, nullptr, 10));
		numstring = "";
	}
	
	return true;
}

void Compiler::mWriteB()
{
	ofstream ofile(mOutput, std::ofstream::out | std::ofstream::binary);
	for (int i = 0, j = mData.size(); i < j; ++i)
	{
		ofile.put((char)mData[i]);
	}
	return;
}

void Compiler::mWriteC()
{
	ofstream ofile(mOutput);
	section sec = kAttributes;
	int argcount = kAttributeArg;
	int i = 0;
	int j = mData.size();
	while (i < j)
	{
		argcount = getargcount(sec);
		if (mData[i] == 255 && sec != kAttributes) //next section check
		{
			sec = (section)((int)sec + 1); //++sec
			++i;
			continue;
		}
		
		switch (sec)
		{
			case kAttributes:
				ofile << "attribute(";
				break;
			case kBackgrounds:
				ofile << "background(";
				break;
			case kBlocks:
				ofile << "block(";
				break;
			case kEnemies:
				ofile << "enemy(";
				break;
			case kDoors:
				ofile << "door(";
				break;
			case kUpDoors:
				ofile << "updoor(";
				break;
			default: //section invalid or undefined type
				ofile << "ERROR: Invalid Type\n";
				return;
		}
		
		for (int k = 0; k < argcount; ++k)
		{
			if (checkchain(sec, k))//index needs to be the same as previous if chaining
			{
				--i;
			}
			argtype atype = getargtype(sec, k);
			string decomp = datatostring(mData, i, atype);//note that datatostring incremements i
			if (decomp == "")
			{
				ofile << "ERROR: in datatostring of argument " << k << "\n";
				return;
			}
			else 
			{
				ofile << decomp;
			}
			if (k < argcount - 1) //in all but the last argument
			{
				ofile << ", ";
			}
			if (i >= j)
				return;
		}
		
		ofile << ");" << endl;
		
		//since an attribute is a singleton and does not need a 255 separator
		if (sec == kAttributes)
			sec = (section)((int)sec + 1); //++sec
	}
	return;
}

void Compiler::mWriteT()
{
	ofstream ofile(mOutput);
	int sec = kAttributes;
	int width = kAttributeWidth;
	for (int i = 0, j = mData.size(); i < j; ++i)
	{
		//check for section break
		if (mData[i] == 255 && sec != kAttributes)
		{
			switch(sec)//move on to the next section
			{
				//atributes cannot move on to the next section, it is required
				case kBackgrounds:
					sec = kBlocks;
					width = kBlockWidth;
					break;
				case kBlocks:
					sec = kEnemies;
					width = kEnemyWidth;
					break;
				case kEnemies:
					sec = kDoors;
					width = kDoorWidth;
					break;
				case kDoors:
					sec = kUpDoors;
					width = kUpDoorWidth;
					break;
				case kUpDoors:
					sec = kSectionDone;
					width = 1;
					break;
				
			}
			//write 255 and add line break
			ofile.write(to_string(mData[i]).c_str(),to_string(mData[i]).size());
			ofile.put('\n');
			continue;
		}
		
		if (sec == kSectionDone)
			break;
		
		//write each number
		int k = 0;
		for (; k < width && (i + k) < j; ++k)//stop at end of object or end of data
		{
			ofile.write(to_string(mData[i + k]).c_str(),to_string(mData[i + k]).size());
			ofile.put(' ');
		}
		i += k - 1;
		ofile.put('\n');
		
		if (sec == kAttributes)
		{
			sec = kBackgrounds;
			width = kBackgroundWidth;
		}
	}
	return;
}
