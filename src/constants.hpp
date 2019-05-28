#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

/*
	This file contains constants and typedefs 
	used by different parts of the project
*/

typedef unsigned char Uint8;
typedef char* const* stringref; //big nasty-looking 2-D pointer

const char kLineCommentSymbol = '#';
const int kMinFileSize = 15; //arbitrary number to check for errors, may change later

//As of Ninja v0.8, there are 6 parts in a map
enum section
{
	kAttributes, kBackgrounds, kBlocks, kEnemies, kDoors, kUpDoors, kSectionDone, kSectionInvalid
};

//amount of data used by single object in specific section
enum bytewidth
{
	kAttributeWidth = 8,
	kBackgroundWidth = 11,
	kBlockWidth = 4,
	kEnemyWidth = 3,
	kDoorWidth = 7,
	kUpDoorWidth = 8
};

//amount of arguments per object
enum argcount
{
	kAttributeArg = 8,
	kBackgroundArg = 8,
	kBlockArg = 5,
	kEnemyArg = 3,
	kDoorArg = 7,
	kUpDoorArg = 8
};

//flags which represent what the argument is
enum argtype
{
	//numbers 0-7 are binary flags, read from right bit to left bit
	kBinArg0,
	kBinArg1,
	kBinArg2,
	kBinArg3,
	kBinArg4,
	kBinArg5,
	kBinArg6,
	kBinArg7,
	
	//half-byte, left to right
	kHalfByteArg0,
	kHalfByteArg1,
	
	//other useful types
	kUInt8Arg,
	kSInt16Arg,
	kFloatArg,//I may add some floating points if I ever fix my background system
	kSubPArg,// My subpixel 2-byte data type which I plan on adding by Ninja v0.9
	
	//special names, will check a list for it
	kSongNameArg,
	kBackgroundNameArg,
	kBlockNameArg,
	kBlockDirectionArg,//not an actual argtype, but used for name lookup
	kEnemyNameArg,
	kDoorTextureArg,
	
	//error handling
	kInvalidType
};

//arrays for each section, describes what each argument is
const int AttributeType[kAttributeArg] = {
	kUInt8Arg,//1
	kUInt8Arg,//2
	kUInt8Arg,//3
	kUInt8Arg,//4
	kUInt8Arg,//5
	kUInt8Arg,//6
	kUInt8Arg,//7
	kSongNameArg//8
};
	
const int BackgroundType[kBackgroundArg] = {
	kBackgroundNameArg,//1
	kSInt16Arg,//2
	kSInt16Arg,//3
	kUInt8Arg,//4
	kSubPArg,//5
	kSubPArg,//6
	kBinArg1,//7
	kBinArg0,//8
};

const int BlockType[kBlockArg] = {
	kBlockNameArg,//1
	kUInt8Arg,//2
	kUInt8Arg,//3
	kHalfByteArg0,//4
	kHalfByteArg1,//5
};

const int EnemyType[kEnemyArg] = {
	kEnemyNameArg,//1
	kUInt8Arg,//2
	kUInt8Arg,//3
};

const int DoorType[kDoorArg] = {
	kUInt8Arg,//1
	kUInt8Arg,//2
	kUInt8Arg,//3
	kUInt8Arg,//4
	kUInt8Arg,//5
	kUInt8Arg,//6
	kUInt8Arg//7
};

const int UpDoorType[kUpDoorArg] = {
	kUInt8Arg,//1
	kUInt8Arg,//2
	kUInt8Arg,//3
	kUInt8Arg,//4
	kUInt8Arg,//5
	kUInt8Arg,//6
	kUInt8Arg,//7
	kDoorTextureArg//8
};

//for use with Flag object
enum flaginput : Uint8
{
	kCompileInput = 0x00,//default option
	kTupleInput = 0x01,
	kBinaryInput = 0x02,
	kInputMask = 0x0f
};

enum flagoutput : Uint8
{
	kCompileOutput = 0x00,
	kTupleOutput = 0x10,//default option
	kBinaryOutput = 0x20,//will eventually become the default option
	kOutputMask = 0xf0
};

//single character delimiters, including separators, parentheses and terminator
const int kDelimiterSize = 4;
const char kDelimiter[] = "(),;";

const char kLineComment = '#';//symbol for a line comment

const int kLineEndSize = 2;
const char kLineEnd[] = "\r\n";

//if found outside the bounds of a line comment, ignore these characters during parsing
const int kIgnoreSize = 4;
const char kIgnore[] = "\r\n\t ";

const int kOperatorSize = 7;
const char kOperator[] = "/*-+:()";

const int kSongNameSize = 6;
char* const kSongName[]={
	(char*)"00",
	(char*)"01",
	(char*)"02",
	(char*)"03",
	(char*)"04",
	(char*)"05"
};

const int kBackgroundNameSize = 22;
char* const kBackgroundName[]={
	(char*)"bluesky",
	(char*)"snowmtn",
	(char*)"grasshills",
	(char*)"nightynight",
	(char*)"skyline",
	(char*)"redsky",
	(char*)"snow",
	(char*)"castlehill",
	(char*)"castlewall",
	(char*)"stormclouds",
	(char*)"moonskull",
	(char*)"castleinterior",
	(char*)"beachhill",
	(char*)"seasky",
	(char*)"castlesky",
	(char*)"beanstalk",
	(char*)"houses",
	(char*)"templeinterior",
	(char*)"wavetex",
	(char*)"traininterior",
	(char*)"tunnel",
	(char*)"tree"
};

const int kBlockNameSize = 5;
char* const kBlockName[]={
	(char*)"dirt",
	(char*)"sand",
	(char*)"stonebrick",
	(char*)"cloud",
	(char*)"train",
};

const int kBlockDirectionSize = 9;
char* const kBlockDirection[]={
	(char*)"0",
	(char*)"n",
	(char*)"e",
	(char*)"s",
	(char*)"w",
	(char*)"ne",
	(char*)"nw",
	(char*)"sw",
	(char*)"se",
};

const int kEnemyNameSize = 4;
char* const kEnemyName[]={
	(char*)"pirate",
	(char*)"ghost",
	(char*)"demon",
	(char*)"cannon",
	(char*)"samurai",
};

const int kDoorTextureSize = 4;
char* const kDoorTexture[]={
	(char*)"caveexit",
	(char*)"caveenter",
};

#endif
