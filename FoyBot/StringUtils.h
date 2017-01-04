#pragma once
#include <string>
#include <sstream>
#include <vector>

using namespace std;

inline void split(const string &s, char delim, vector<string> &elems) {
	stringstream ss(s);
	string item;
	while (getline(ss, item, delim)) {
		elems.push_back(item);
	}
}


vector<string> split(const string &s, char delim) {
	vector<string> elems;
	split(s, delim, elems);
	return elems;
}


inline int char2int(char input)
{
	if (input >= '0' && input <= '9')
		return input - '0';
	if (input >= 'A' && input <= 'F')
		return input - 'A' + 10;
	if (input >= 'a' && input <= 'f')
		return input - 'a' + 10;
	throw std::invalid_argument("Invalid input string");
}


inline int hex2bin(const char* src, char* target)
{
	auto size = 0;
	while (*src && src[1])
	{
		++size;
		*(target++) = char2int(*src) * 16 + char2int(src[1]);
		src += 2;
	}
	return size;
}