#include <iostream>
#include <vector>
#include <string>

using byte = unsigned char;

using namespace std;

vector<byte> doXor(const vector<byte>, const vector<byte>);

vector<byte> toBytes(const string);
string asText(const vector<byte>);

string asHex(const vector<byte>);

vector<byte> hexToBytes(const string);
byte hexPairToByte(const string);
int hexToDigit(char);

string byteToHex(byte);
char digitToHex(int);

int main(int argc, char* argv[])
{
	auto duskBytes = toBytes("dusk");
	cout << "Dusk bytes are: " << asHex(duskBytes) << endl;

	auto dawnBytes = toBytes("dawn");
	auto encryptedDawnBytes = hexToBytes("e7e53f36");

	auto keyBytes = doXor(dawnBytes, encryptedDawnBytes);
	cout << "Key bytes are: " << asHex(keyBytes) << endl;

	auto encryptedDuskBytes = doXor(keyBytes, duskBytes);

	cout << "Encrypted dusk is: " << asHex(encryptedDuskBytes) << endl;
}

vector<byte> doXor(const vector<byte> first, const vector<byte> second)
{
	vector<byte> result(first.size());
	
	auto currentFirst = first.begin();
	auto currentSecond = second.begin();

	auto currentResult = result.begin();

	for (;
		currentFirst != first.end() && currentSecond != second.end(); 
		currentFirst++, currentSecond++, currentResult++)
	{
		*currentResult = *currentFirst ^ *currentSecond;
	}

	return result;
}

vector<byte> toBytes(const string text) 
{
	vector<byte> result;
	result.reserve(text.size());

	for (auto character : text) 
	{
		result.push_back((byte)character);
	}

	return result;
}

string asHex(const vector<byte> data)
{
	string result;

	for (byte b: data)
	{
		result += byteToHex(b) + " ";
	}

	return result;
}

string byteToHex(byte b)
{
	int major = b / 16;
	int minor = b % 16;

	return { digitToHex(major), digitToHex(minor) };
}

char digitToHex(int digit)
{
	if (digit <= 9) 
	{
		return '0' + digit;
	}
	else 
	{
		return 'a' + (digit - 10);
	}
}

vector<byte> hexToBytes(const string hexString) 
{
	vector<byte> result;
	result.reserve(hexString.size()/2);

	for (auto currentChar = hexString.begin(); 
		currentChar != hexString.end();)
	{
		string hexPair = { *currentChar, *(currentChar + 1) };
		
		result.push_back(hexPairToByte(hexPair));

		currentChar += 2;
	}

	return result;
}

byte hexPairToByte(string hexPair) 
{
	char major = hexPair[0];
	char minor = hexPair[1];

	return hexToDigit(major) * 16 + hexToDigit(minor);
}

int hexToDigit(char hex)
{
	if (hex - '0' <= 9)
	{
		return hex - '0';
	}
	else 
	{
		return 10 + hex - 'a';
	}
}

string asText(const vector<byte> bytes)
{
	string result;
	result.reserve(bytes.size());

	for (byte b : bytes) 
	{
		result.push_back((char)b);
	}

	return result;
}

