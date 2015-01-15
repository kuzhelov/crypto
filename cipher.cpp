#include <iostream>
#include <vector>
#include <string>

using byte = unsigned char;

using namespace std;

vector<byte> doXor(const vector<byte>, const vector<byte>);

vector<byte> toBytes(const string);
string asText(const vector<byte>);

string asHex(const vector<byte>, bool withIndex=false);

vector<byte> hexToBytes(const string);
byte hexPairToByte(const string);
int hexToDigit(char);

string byteToHex(byte);
char digitToHex(int);

string nthByte(const vector<byte>, int);

int bytesInHex(const string);
vector<byte> zeroify(const vector<byte> data, bool (*predicate)(byte));

int main(int argc, char* argv[])
{
	string cipher1 = "315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b16349c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc41556bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d524400a19b159610b11ef3e";
	string cipher2 = "234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b028aa76eb7b4ab24171ab3cdadb8356f";
	
	auto xoredBytes = doXor(hexToBytes(cipher1), hexToBytes(cipher2));
	cout << "Xored bytes are: " << asHex(xoredBytes, true) << endl;
	cout << "Byte with index 12 is: " << nthByte(xoredBytes, 12) << endl;
	

	auto zeroified = zeroify(xoredBytes, 
		[](byte b) {
			return ((int)(b) - 65) < 0 || ((int)(b) - 122) > 0;
		});
	cout << asHex(zeroified, true) << endl;
}

string nthByte(const vector<byte> data, int index)
{
	return byteToHex(data[index]);
}

vector<byte> zeroify(const vector<byte> data, bool (*predicate)(byte))
{
	vector<byte> result;
	result.reserve(data.size());

	for (byte b : data)
	{	
		result.push_back(predicate(b) ? 0 : b);
	}

	return result;
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

template<typename IteratorT, typename FunctionT>
void for_each_indexed(IteratorT first, IteratorT last, FunctionT action)
{
	int index = 0;
	for (; first != last; ++index, ++first)
	{
		action(*first, index);
	}
}

string asHex(const vector<byte> data, bool withIndex)
{
	string result;
	result.reserve(data.size() * 2);

	for_each_indexed(data.begin(), data.end(), 
		[&withIndex, &result](byte b, int index) 
		{
			if (withIndex) {
				result += to_string(index) + ":";
			}

			result += byteToHex(b) + "   ";
		});
	
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

int bytesInHex(const string hexString)
{
	return hexString.size() / 2;
}

