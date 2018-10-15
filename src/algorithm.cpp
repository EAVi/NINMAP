#include "algorithm.h"

using namespace std;

Uint8 nameint(const string & s, const argtype & a)
{
	int asize = 0;
	stringref namearray = NULL;
	namearray = getnamearray(a, asize);
	
	for (int i = 0; i < asize; ++i)
	{
		if (s == namearray[i])
			return (Uint8)i;
	}
	return 255;
}

string namestring(const int & i, const argtype & a)
{
	int asize = 0;
	stringref namearray = NULL;
	namearray = getnamearray(a, asize);
	
	if (i >= asize)//seg fault
		return "";
	else 
		return namearray[i];
}

stringref getnamearray(const argtype & a, int & asize)
{
	switch(a)
	{
		case kSongNameArg:
			asize = kSongNameSize;
			return kSongName;
		case kBackgroundNameArg:
			asize = kBackgroundNameSize;
			return kBackgroundName;
		case kBlockNameArg:
			asize = kBlockNameSize;
			return kBlockName;
		case kBlockDirectionArg:
			asize = kBlockDirectionSize;
			return kBlockDirection;
		case kEnemyNameArg:
			asize = kEnemyNameSize;
			return kEnemyName;
		case kDoorTextureArg:
			asize = kDoorTextureSize;
			return kDoorTexture;
		default: //argtype is non-name type
			asize = 0;
			return NULL;
	}
}

#include <iostream>
argtype getargtype(const section & s, const int & i)
{
	int size;
	const int* typearray = NULL;
	typearray = getargarray(s, size);
	
	if (i < 0 || i >= size)
	{
		cout << "invalid type, i,size = " << i << ',' << size << endl;
		return kInvalidType;
	}
	
	return (argtype)typearray[i];
	
}


const int* getargarray(const section & s, int & size)
{
	switch (s)
	{
		case kAttributes:
			size = (int)kAttributeArg;
			return AttributeType;
		case kBackgrounds:
			size = (int)kBackgroundArg;
			return BackgroundType;
		case kBlocks:
			size = (int)kBlockArg;
			return BlockType;
		case kEnemies:
			size = (int)kEnemyArg;
			return EnemyType;
		case kDoors:
			size = (int)kDoorArg;
			return DoorType;
		case kUpDoors:
			size = (int)kUpDoorArg;
			return UpDoorType;
		default: //section invalid or undefined type
			size = 0;
			return NULL;
	}

}

int getargcount(const section & s)
{
	int size = 0;
	getargarray(s,size);
	return size;
}


bool checkchain(const section & s, const int & i)
{
	int h = i - 1;
	if (h < 0)
		return false;
	
	argtype current = getargtype(s, i);
	argtype prev = getargtype(s, h);
	
	if (current == kInvalidType || prev == kInvalidType)
		return false;
	
	//if current and previous are in the same range
	if (current >= kBinArg0 && current <= kBinArg7
		&& prev >= kBinArg0 && prev <= kBinArg7)
		return true;
	
	//if in half byte range
	else if (current >= kHalfByteArg0 && current <= kHalfByteArg1
		&& prev >= kHalfByteArg0 && prev <= kHalfByteArg1)
		return true;

	return false;
}

string datatostring(const vector<Uint8> & data, int & i, const argtype & a)
{
	if (a >= kSongNameArg && a < kInvalidType
		&& a != kBlockNameArg)//blockname gets a special function
	{
		string s = namestring(data[i], a);
		++i;
		return s;
	}
	switch (a)
	{
		case kBlockNameArg:
			return blockname(data, i);
		case kUInt8Arg:
			return datauint8(data, i);
		case kSInt16Arg:
			return datasint16(data, i);
		case kFloatArg:
			return datafloat(data, i);
		case kSubPArg:
			return datasubp(data, i);
	}
	
	//chaining types
	if (a >= kBinArg0 && a <= kBinArg7)
		return databool(data, i, a);
	if (a >= kHalfByteArg0 && a <= kHalfByteArg1)
		return datahalfbyte(data, i, a);
	return "";
}

string blockname(const vector<Uint8> & data, int & i)
{
	int j = data.size();
	string s = "";
	
	if (i >= j || i < 0)//segfault error
	{
		++i;
		return "";
	}
	
	int block = (int)data[i] / 9;
	int direction = (int)data[i] % 9;
	s += namestring(block, kBlockNameArg);
	
	if (s == "")//if namestring returned error
	{
		++i;
		return to_string((int)data[i]);//int to string, still valid to the compiler, but no name
	}
	
	if (direction != 0)//the special part about the block string is that it has 2 parts
	{
		s += " + ";
		s += namestring(direction, kBlockDirectionArg);
	}
	
	++i;
	return s;
}

string datauint8(const vector<Uint8> & data, int & i)
{
	int j = data.size();
	
	if (i >= j || i < 0)//segfault error
	{
		++i;
		return "";
	}
	int dataint = (int)data[i];
	
	++i;
	return to_string(dataint);
}

string datasint16(const vector<Uint8> & data, int & i)
{
	int j = data.size();
	
	if (i >= (j - 1) || i < 0)//segfault error
	{
		++i;
		return "";
	}
	
	int dataint = (int)data[i];
	dataint = dataint << 8;
	dataint |= (int)data[i + 1];
	
	i += 2;
	return to_string(dataint);
}

string datafloat(const vector<Uint8> & data, int & i)
{
	int j = data.size();
	
	if (i >= (j - 3) || i < 0)//segfault error
	{
		++i;
		return "";
	}
	
	int dataint = (int)data[i];
	dataint = dataint << 8;
	dataint |= (int)data[i + 1];
	dataint = dataint << 8;
	dataint |= (int)data[i + 2];
	dataint = dataint << 8;
	dataint |= (int)data[i + 3];
	
	float datafloat = *(float*) &dataint;//typecasting pointers to convert the int binary to float
	
	i += 4;
	return to_string(datafloat);
}

string datasubp(const vector<Uint8> & data, int & i)
{
	int j = data.size();
	
	if (i >= (j - 1) || i < 0)//segfault error
	{
		++i;
		return "";
	}
	
	float datafloat = (float)data[i];
	datafloat += (float)data[i+1]/256.f;
	
	i += 2;
	return to_string(datafloat);
}

string datahalfbyte(const vector<Uint8> & data, int & i, const argtype & a)
{
	Uint8 mask;
	switch(a)
	{
		case kHalfByteArg0:
			mask = 0x0f;
			break;
		case kHalfByteArg1:
			mask = 0xf0;
			break;
	}
	int dataint = data[i] & mask;
	++i;
	if (a == kHalfByteArg1)
		dataint = dataint >> 4;
	return to_string(dataint);
}

string databool(const vector<Uint8> & data, int & i, const argtype & a)
{
	Uint8 mask = 0x01;
	int shift = (a - kBinArg0); //up to 7 shifts
	mask = mask << shift;
	
	bool databool = (bool)(data[i] & mask);
	
	++i;
	if (databool)
		return "true";
	else return "false";
}
