#include <iostream>
#include <vector>
#include <string>

using namespace std;

using byte = unsigned char;
using bytes = vector<byte>;

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

template<typename IteratorT, typename FunctionT>
void for_each_indexed(IteratorT first, IteratorT last, FunctionT action);

vector<bytes> hexesToBytes(const vector<string> &hexes) 
{
	vector<bytes> result;
	result.reserve(hexes.size());

	for_each(hexes.begin(), hexes.end(), 
		[&] (string hex) {
			result.push_back(hexToBytes(hex));
		});

	return result;
}

vector<int> testAgainst(
	const vector<bytes> &ciphers, 
	int ethalonIndex,
	int byteIndex,
	bool (*predicate)(byte, byte))
{
	auto ethalonCipher = ciphers[ethalonIndex];
	byte ethalonByte = ethalonCipher[byteIndex];

	vector<int> results;
	results.reserve(ciphers.size());

	for_each_indexed(ciphers.begin(), ciphers.end(), 
		[&] (vector<byte> other, int index) {
			if (index == ethalonIndex) {
				results.push_back(0);
				return;
			}

			if (byteIndex >= other.size()) {
				results.push_back(0);
				return;
			}

			byte otherByte = other[byteIndex];
			results.push_back(predicate(ethalonByte, otherByte) ? 1 : -1);
		});

	return results;
}

string interpretTestResult(int testResult);
void printTestResults(const vector<int> &testResults)
{
	for_each_indexed(testResults.begin(), testResults.end(), 
		[&] (int result, int testIndex) {
			cout << "test " << testIndex << ": " << interpretTestResult(result) << endl;
		});
}

string interpretTestResult(int testResult) 
{
	switch (testResult)
	{
		case 1: return "passed"; 
		case -1: return "failed"; 
		default: return "undefined";
	}
}

int bytesInHex(const string);
vector<byte> zeroify(const vector<byte> data, bool (*predicate)(byte));

bool isLetter(byte b) 
{
	int capitalAOffset = b - 'A';
	int lowercaseAOffset = b - 'a';

	return (capitalAOffset >= 0 && capitalAOffset <= 26)
		|| (lowercaseAOffset >=0 && lowercaseAOffset <= 26);
}

int sum(const vector<int>& ints)
{
	int sum = 0;
	for (int val : ints) {
		sum += val;
	}

	return sum;
}

int findCipherWithSpaceAt(
	int byteIndex, 
	const vector<bytes>& ciphers)
{
	int maxAdvantage = -1 * ciphers.size();
	int resultCipherIndex = -1;

	for_each_indexed(ciphers.begin(), ciphers.end(), 
		[&] (vector<byte> cipher, int cipherIndex) {
			auto testResults = testAgainst(
				ciphers,
				cipherIndex,
				byteIndex,
				[] (byte first, byte second) {
					auto b = first ^ second;
					return isLetter(b);
				});

			auto cipherAdvantage = sum(testResults);
			if (cipherAdvantage > maxAdvantage)
			{
				resultCipherIndex = cipherIndex;
				maxAdvantage = cipherAdvantage;
			}
		});

	cout << "Advantage is: " << maxAdvantage << endl;

	return resultCipherIndex;
}

byte getNthByteOfKey(int byteIndex, const vector<bytes>& ciphers)
{
	int cipherIndex = findCipherWithSpaceAt(byteIndex, ciphers);

	byte space = 0x20;
	byte encodedSpace = ciphers[cipherIndex][byteIndex];

	return encodedSpace ^ space;
}

int main(int argc, char* argv[])
{
	string cipher0 = "315c4eeaa8b5f8aaf9174145bf43e1784b8fa00dc71d885a804e5ee9fa40b16349c146fb778cdf2d3aff021dfff5b403b510d0d0455468aeb98622b137dae857553ccd8883a7bc37520e06e515d22c954eba5025b8cc57ee59418ce7dc6bc41556bdb36bbca3e8774301fbcaa3b83b220809560987815f65286764703de0f3d524400a19b159610b11ef3e";
	string cipher1 = "234c02ecbbfbafa3ed18510abd11fa724fcda2018a1a8342cf064bbde548b12b07df44ba7191d9606ef4081ffde5ad46a5069d9f7f543bedb9c861bf29c7e205132eda9382b0bc2c5c4b45f919cf3a9f1cb74151f6d551f4480c82b2cb24cc5b028aa76eb7b4ab24171ab3cdadb8356f";
	string cipher2 = "32510ba9a7b2bba9b8005d43a304b5714cc0bb0c8a34884dd91304b8ad40b62b07df44ba6e9d8a2368e51d04e0e7b207b70b9b8261112bacb6c866a232dfe257527dc29398f5f3251a0d47e503c66e935de81230b59b7afb5f41afa8d661cb";
	string cipher3 = "32510ba9aab2a8a4fd06414fb517b5605cc0aa0dc91a8908c2064ba8ad5ea06a029056f47a8ad3306ef5021eafe1ac01a81197847a5c68a1b78769a37bc8f4575432c198ccb4ef63590256e305cd3a9544ee4160ead45aef520489e7da7d835402bca670bda8eb775200b8dabbba246b130f040d8ec6447e2c767f3d30ed81ea2e4c1404e1315a1010e7229be6636aaa";
	string cipher4 = "3f561ba9adb4b6ebec54424ba317b564418fac0dd35f8c08d31a1fe9e24fe56808c213f17c81d9607cee021dafe1e001b21ade877a5e68bea88d61b93ac5ee0d562e8e9582f5ef375f0a4ae20ed86e935de81230b59b73fb4302cd95d770c65b40aaa065f2a5e33a5a0bb5dcaba43722130f042f8ec85b7c2070";
	string cipher5 = "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd2061bbde24eb76a19d84aba34d8de287be84d07e7e9a30ee714979c7e1123a8bd9822a33ecaf512472e8e8f8db3f9635c1949e640c621854eba0d79eccf52ff111284b4cc61d11902aebc66f2b2e436434eacc0aba938220b084800c2ca4e693522643573b2c4ce35050b0cf774201f0fe52ac9f26d71b6cf61a711cc229f77ace7aa88a2f19983122b11be87a59c355d25f8e4";
	string cipher6 = "32510bfbacfbb9befd54415da243e1695ecabd58c519cd4bd90f1fa6ea5ba47b01c909ba7696cf606ef40c04afe1ac0aa8148dd066592ded9f8774b529c7ea125d298e8883f5e9305f4b44f915cb2bd05af51373fd9b4af511039fa2d96f83414aaaf261bda2e97b170fb5cce2a53e675c154c0d9681596934777e2275b381ce2e40582afe67650b13e72287ff2270abcf73bb028932836fbdecfecee0a3b894473c1bbeb6b4913a536ce4f9b13f1efff71ea313c8661dd9a4ce";
	string cipher7 = "315c4eeaa8b5f8bffd11155ea506b56041c6a00c8a08854dd21a4bbde54ce56801d943ba708b8a3574f40c00fff9e00fa1439fd0654327a3bfc860b92f89ee04132ecb9298f5fd2d5e4b45e40ecc3b9d59e9417df7c95bba410e9aa2ca24c5474da2f276baa3ac325918b2daada43d6712150441c2e04f6565517f317da9d3";
	string cipher8 = "271946f9bbb2aeadec111841a81abc300ecaa01bd8069d5cc91005e9fe4aad6e04d513e96d99de2569bc5e50eeeca709b50a8a987f4264edb6896fb537d0a716132ddc938fb0f836480e06ed0fcd6e9759f40462f9cf57f4564186a2c1778f1543efa270bda5e933421cbe88a4a52222190f471e9bd15f652b653b7071aec59a2705081ffe72651d08f822c9ed6d76e48b63ab15d0208573a7eef027";
	string cipher9 = "466d06ece998b7a2fb1d464fed2ced7641ddaa3cc31c9941cf110abbf409ed39598005b3399ccfafb61d0315fca0a314be138a9f32503bedac8067f03adbf3575c3b8edc9ba7f537530541ab0f9f3cd04ff50d66f1d559ba520e89a2cb2a83";


	string passPhrase = "32510ba9babebbbefd001547a810e67149caee11d945cd7fc81a05e9f85aac650e9052ba6a8cd8257bf14d13e6f0a803b54fde9e77472dbff89d71b57bddef121336cb85ccb8f3315f4b52e301d16e9f52f904";
	cout << "Bytes in pass phrase: " << (passPhrase.size() / 2) << endl;

	vector<string> ciphers = {
		cipher0,
		cipher1,
		cipher2,
		cipher3,
		cipher4,
		cipher5,
		cipher6,
		cipher7, 
		cipher8,
		cipher9
	};

	vector<byte> key(6, 0);
	key[0] = 0x66;
	key[1] = 0x39;
	key[2] = 0x6e;
	key[3] = 0x89;
	key[4] = 0xc9;
	key[5] = 0xdb;

	string phrase(passPhrase.begin(), passPhrase.begin() + 6);
	auto phraseBytes = hexToBytes(phrase);

	auto decyphered = doXor(phraseBytes, key);
	cout << "Deciphered: " << asText(decyphered) << endl;
	
	vector<bytes> cipherBytes = hexesToBytes(ciphers);

	int byteIndex = 7;
	byte keyByte = getNthByteOfKey(byteIndex, cipherBytes);
	cout << "Nth byte is: " << byteToHex(keyByte) << endl; 

	int cipherIndexWithSpace = findCipherWithSpaceAt(0, cipherBytes);
	cout << "Cipher with space is: " << cipherIndexWithSpace << endl;

	auto testResults = testAgainst(
		cipherBytes,
		1, 	// cipher index
		5,	// byte index
		[] (byte first, byte second) {
			auto b = first ^ second;
			return isLetter(b);
		});

	printTestResults(testResults);

	string first = cipher9;
	string second = cipher1;

	cout << "Ciphered bytes of first are: " << asHex(hexToBytes(first), true) << endl;
	cout << "------------------" << endl;

	auto xoredBytes = doXor(hexToBytes(first), hexToBytes(second));
	auto zeroified = zeroify(xoredBytes, 
		[](byte b) {
			return !isLetter(b);
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
	for_each(first, last, 
		[&action, &index] (typename iterator_traits<IteratorT>::value_type current) {
			action(current, index);
			++index;
		});
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

bool isVisibleChar(byte b)
{
	return b >= 0x20 && b <= 0x7e;
}

string asText(const vector<byte> bytes)
{ 
	string result;
	result.reserve(bytes.size());

	for (byte b : bytes) 
	{
		result.push_back(isVisibleChar(b) ? (char)b : '?');
	}

	return result;
}

int bytesInHex(const string hexString)
{
	return hexString.size() / 2;
}

