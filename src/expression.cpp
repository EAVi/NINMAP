#include "expression.h"

using namespace std;

Expression::Expression()
{
	mString = string();
	mIndex = 0;
	mStringSize = 0;
	mData = 0.0;
	mLineNumber = -1;
	mRect = {0,0,0,0};
	mArgType = kUInt8Arg;
	mErrorMessage = string();
	mWarningMessage = string();
	mError = false;
	mWarning = false;
}

void Expression::setLineNumber(int i)
{
	mLineNumber = i;
}

void Expression::setRect(expRect r)
{
	mRect = r;
}

void Expression::setType(argtype a)
{
	mArgType = a;
}

bool Expression::expr(string s)
{
	mString = s;
	mIndex = 0;
	mStringSize = s.size();
	mData = sum();
	return !mError;
}

vector<Uint8> Expression::getData()
{
	int wide;
	Uint8 byte;
	vector<Uint8> result = vector<Uint8>();
	
	//binary arguments
	if (mArgType >= kBinArg0 && mArgType <= kBinArg7)
	{
		int shift = mArgType - kBinArg0;
		byte = (Uint8)mData;
		if (byte != 0)
		{
			if (byte != 1)
			{
				mWarning = true;
				mWarningMessage = "Numerical value passed to bool";
				byte = 1;
			}
			byte = byte<<shift;
			result.push_back(byte);
		}
	}
	//half-byte arguments
	else if (mArgType == kHalfByteArg0 || mArgType == kHalfByteArg1)
	{
		byte = (Uint8)mData;
		if (byte >= 16)
		{
			mWarning = true;
			mWarningMessage = "Half byte argument given value above 15; data will be lost";
			byte &= 0x0f;//truncate to 4 bits
		}
		if (mArgType == kHalfByteArg1)
			byte = byte<<4;
		
		result.push_back(byte);
	}
	//2-byte integer argument
	else if(mArgType == kSInt16Arg || mArgType == kSubPArg)
	{
		wide = (mArgType == kSInt16Arg)//a little ternary fella
			?(int)mData
			:(int)(256.0 * mData);//for the subpixel data type, just multiply float by 256
		byte = (Uint8)(wide>>8);
		result.push_back(byte);
		byte = (Uint8)wide;
		result.push_back(byte);
		
		if (wide >= 0xffff)
		{
			mWarning = true;
			mWarningMessage = "2-byte argument given value above 65535; data will be lost";
			//no need to truncate, but some data may be lost
		}
	}
	//float type
	else if (mArgType == kFloatArg)
	{
		float tempf = (float)mData;
		wide = *(int*)&tempf;//dereference a pointer of float to int, it's all the same bits, but now int type
		byte = (Uint8)(wide>>24);
		result.push_back(byte);
		byte = (Uint8)(wide>>16);
		result.push_back(byte);
		byte = (Uint8)(wide>>8);
		result.push_back(byte);
		byte = (Uint8)wide;
		result.push_back(byte);
	}
	//the rest are just 8-bits
	else
	{
		byte = (Uint8)mData;
		if (mData >= 256)
		{
			mWarning = true;
			mWarningMessage = "Uint8 given a value above 255; data will be lost";
		}
		result.push_back(byte);
	}
	
	return result;
}

bool Expression::checkWarning()
{
	return mWarning;
}

void Expression::printWarning()
{
	cout << "Expression Warning";
	if (mLineNumber != -1)
		cout << '(' << mLineNumber << "): ";
	cout << mWarningMessage << endl;
}

void Expression::printError()
{
	cout << "Expression Error:";
	if (mLineNumber != -1)
		cout << '(' << mLineNumber << "): ";
	cout << mErrorMessage << endl;
}

string Expression::getError()
{
	return mErrorMessage;
}

void Expression::printData()
{
	cout << mData << endl;
}

double Expression::factor()
{
	double result = 0;
	if (mString[mIndex] >= '0' && mString[mIndex] <= '9' || mString[mIndex] == '-')
	{
		result = number();
	}
	else if(mString[mIndex] >= 'a' && mString[mIndex] <= 'z')
	{
		result = name();
	}
	else if (mString[mIndex] == '(')
	{
		mIndex++;// skip (
		result = sum();
		if (mString[mIndex++] != ')'
			&& !mError)//throw only on first error
		{
			mError = true;
			mErrorMessage = "expected ')'";
		}
		mIndex++;//skip )
	}
	else if (!mError)
	{
		mError = true;
		mErrorMessage = "unexpected factor character read: " + mString[mIndex];
		return 0.0;
	}
	return result;
}

double Expression::term()
{
	double lval = factor();
	while (mString[mIndex] == '*' || mString[mIndex] == '/')
	{
		double rval = 0;
		switch (mString[mIndex++])
		{
			case '*': 
				lval = lval * factor();
				break;
			case '/': 
				rval = factor();
				lval = lval / rval;
				if (rval == 0.0
					&& !mError)//NAN error
				{
					mError = true;
					mErrorMessage = "Division by 0";
				}
				break;
		}
	}
	return lval;
}

double Expression::sum()
{
	double lval = term();
	while (mString[mIndex] == '+' || mString[mIndex] == '-' || mString[mIndex] == ':')
	{
		switch(mString[mIndex++])
		{
			case '+': lval += term(); break;
			case '-': lval -= term(); break;
			case ':': lval = lval*256 + term(); break; //with integers, x:y === (x<<8)+y
		}
	}
	return lval;
}

double Expression::number()
{
	//currently does not accept 0x or 0b formats 
	//or scientific notation
	string ds = "";
	ds+= mString[mIndex++];
	while (mString[mIndex] >= '0' && mString[mIndex] <= '9' || mString[mIndex] == '.')
	{
		ds += mString[mIndex++];
	}
	return (stod(ds));
}

double Expression::name()
{
	string sub = "";
	sub += mString[mIndex++];
	while (mString[mIndex] >= 'a' && mString[mIndex] <= 'z')
	{
		sub += mString[mIndex++];
	}
	//rectangle names
	if (sub == "l")
		return mRect.x1;
	if (sub == "r")
		return mRect.x2;
	if (sub == "u")
		return mRect.y1;
	if (sub == "d")
		return mRect.y2;
	if (sub == "true")
		return 1;
	if (sub == "false")
		return 0;
	
	//name lookup
	Uint8 lookup = nameint(sub, mArgType);
	
	if (lookup == 255)//nameint function returns 255 on error
	{
		mError = true;
		mErrorMessage = "Name does not exist: " + sub;
		return 0.0;
	}
	
	return (double)lookup;
}