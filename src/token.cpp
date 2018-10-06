#include "token.h"

using namespace std;

Token::Token()
{
	mObjectString = string();
	mData = vector<Uint8>();
	mArgS = vector<string>();
	mErrorMessage = string();
	mRect = {1,2,3,4};
	mType = kSectionInvalid;
	mLineNumber = -1;
}

bool Token::stringToObject(string s)
{
	mObjectString = s;
	return ( 
		mParse() 
		&& mCheckType() 
		&& mEvaluate()
	);//short circuit and return false on failure
}

vector<Uint8> Token::getObject()
{
	return mData;
}

void Token::printError()
{
	if (mLineNumber != -1)
		cout << '(' << mLineNumber << "): ";
	cout << mErrorMessage << endl;
}

string Token::getError()
{
	if (mLineNumber != -1)
	{
		string linemessage = "line ";
		linemessage += to_string(mLineNumber);
		linemessage += ", ";
		linemessage += mErrorMessage;
		return linemessage;
	}
	return mErrorMessage;
}

void Token::setLineNumber(int i)
{
	mLineNumber = i;
}

section Token::getSection()
{
	return mType;
}

void Token::setRect(expRect r)
{
	mRect = r;
}

expRect Token::rectShift()
{
	if (mType == kBlocks)
	{
		int x = mData[1];
		int y = mData[2];
		//in the future, I plan on making block allocations larger
		//these following two lines will change
		int width = mData[3] & 0x0f;
		int height = (mData[3] & 0xf0) >> 4;
		
		mRect.x1 = x;
		mRect.y1 = y;
		mRect.x2 = x + width;
		mRect.y2 = y + height;
	}
	return mRect;
}


bool Token::mParse()
{
	bool linecomment = false;
	string s = "";//substring
	for (int i = 0, j = mObjectString.length(); i < j; ++i)
	{
		char c = mObjectString[i];
		
		//add a new argument (non-empty string) to the argument list if delimiter
		if (mIsDelimiter(c)
			&& s != ""
			&& !linecomment)
		{
			mArgS.push_back(s);
			s = "";//empty the string
		}
		//add a character to the substring if the character is not ignored
		if (!mIsIgnore(c)
			&& !linecomment
			&& !mIsLineCommentBegin(c)
			&& !mIsDelimiter(c))
			s += c;
		
		//linecomments should have been handled by the terminator parser
		//but it isn't hard to do a second time
		if (mIsLineCommentEnd(c))
			linecomment = false;
		
		if (mIsLineCommentBegin(c))
			linecomment = true;
	}

	return true;
}

bool Token::mCheckType()
{
	if (mArgS.size() == 0)
	{
		mErrorMessage = "String has not been parsed, or object string is unsubstantial";
		return false;
	}
	//can't switch/case a string
	//detect the object type (and which section of the map file the object belongs)
	else if (mArgS[0] == "attribute") mType = kAttributes;
	else if (mArgS[0] == "background") mType = kBackgrounds;
	else if (mArgS[0] == "block") mType = kBlocks;
	else if (mArgS[0] == "enemy") mType = kEnemies;
	else if (mArgS[0] == "door") mType = kDoors;
	else if (mArgS[0] == "updoor") mType = kUpDoors;
	else 
	{
		mErrorMessage = "Invalid object name: ";
		mErrorMessage += mArgS[0];
		return false;
	}
	
	//check if enough arguments are passed for the object
	int requiredArgC = -1;
	switch (mType)
	{
		case kAttributes:
			requiredArgC = kAttributeArg;
			break;
		case kBackgrounds:
			requiredArgC = kBackgroundArg;
			break;
		case kBlocks:
			requiredArgC = kBlockArg;
			break;
		case kEnemies:
			requiredArgC = kEnemyArg;
			break;
		case kDoors:
			requiredArgC = kDoorArg;
			break;
		case kUpDoors:
			requiredArgC = kUpDoorArg;
			break;
	}
	//error if object did not get the correct number of arguments
	//minus 1 because args includes the object call
	if (mArgS.size() - 1 != requiredArgC)
	{
		mErrorMessage = "Object type ";
		mErrorMessage += mArgS[0];
		mErrorMessage += " takes ";
		mErrorMessage += to_string(requiredArgC);
		mErrorMessage += " arguments";
		return false;
	}
	return true;
}

bool Token::mEvaluate()
{
	for(int i = 1, j = mArgS.size(); i < j; ++i)
	{
		if (!mEvaluate(i))
			return false;
	}
	return true;
}

bool Token::mEvaluate(int i)
{
	Uint8 temp = 0;
	Expression e;
	e.setRect(mRect);
	e.setType(mGetType(i - 1));
	e.setLineNumber(mLineNumber);
	
	if (!e.expr(mArgS[i]))
	{
		mErrorMessage = "arg[" + to_string(i - 1) + "]: ";
		mErrorMessage += "in expression: ";
		mErrorMessage += e.getError();
		return false;
	}
	
	//return data from evaluation
	vector<Uint8> tempvec = e.getData();
	if (tempvec.size() == 0)
	{
		mErrorMessage = "arg[" + to_string(i - 1) + "]: ";
		mErrorMessage += "Evaluation returned empty array";
		return false;
	}
	
	//returned data can either chain or not chain
	if (mCheckChain(i - 1))
	{
		if (tempvec.size() != 1)
		{
			mErrorMessage = "arg[" + to_string(i - 1) + "]: ";
			mErrorMessage += "attempting to chain to vector of size greater than 1";
			return false;
		}
		mChain(tempvec[0]);
	}
	else 
	{
		mData.insert(mData.end(), tempvec.begin(), tempvec.end());
	}
	
	//print warnings if there are any
	if (e.checkWarning())e.printWarning();
	return true;
}

bool Token::mIsDelimiter(const char & c)
{
	for (int i = 0; i < kDelimiterSize; ++i)
	{
		if (c == kDelimiter[i])
			return true;
	}
	return false;
}

bool Token::mIsIgnore(const char & c)
{
	for (int i = 0; i < kIgnoreSize; ++i)
	{
		if (c == kIgnore[i])
			return true;
	}
	return false;
}

bool Token::mIsLineCommentBegin(const char & c)
{
	return (c == kLineComment);
}

bool Token::mIsLineCommentEnd(const char & c)
{
	for (int i = 0; i < kLineEndSize; ++i)
	{
		if (c == kLineEnd[i])
			return true;
	}
	return false;
}

argtype Token::mGetType(int i)
{
	if (i < 0)
		return kInvalidType;
	
	switch(mType)
	{
		case kAttributes:
			return (argtype)AttributeType[i];
		case kBackgrounds:
			return (argtype)BackgroundType[i];
		case kBlocks:
			return (argtype)BlockType[i];
		case kEnemies:
			return (argtype)EnemyType[i];
		case kDoors:
			return (argtype)DoorType[i];
		case kUpDoors:
			return (argtype)UpDoorType[i];
		default:
			return kInvalidType;
	}
	return kInvalidType;
}

bool Token::mCheckChain(int i)
{
	//previous index is invalid index
	//can't chain to previous index if it's the first index
	if (i <= 0)
		return false;
	
	argtype type = mGetType(i);//argument type
	argtype prevtype = mGetType(i - 1);//previous argument type

	
	if (type >= kBinArg0 && type <= kBinArg7
		&& prevtype >= kBinArg0 && prevtype <= kBinArg7)
	{
		return true;
	}
	else if (type >= kHalfByteArg0 && type <= kHalfByteArg1
		&& prevtype <= kHalfByteArg0 && prevtype <= kHalfByteArg0)
	{
		return true;
	}
	return false;
}

bool Token::mChain(Uint8 i)
{
	int last = mData.size() - 1;
	if (last < 0) 
	{
		mErrorMessage = "Chain attempted on empty data";
		return false;
	}
	mData[last] |= i;
	return true;
}

bool operator<(Token lhs, Token rhs)
{
	return (lhs.mType < rhs.mType);
}