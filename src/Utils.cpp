#include "Utils.hpp"
#include <string>
#include <cstring>

using std::string;

string Utils::toUpper(const string str){
	char* result = (char*)str.c_str();

	for (int i=0; i<strlen(result); i++)
		result[i] = toupper(result[i]);

	return string(result);
}

string Utils::toLower(const string str){
	char* result = (char*)str.c_str();

	for (int i=0; i<strlen(result); i++)
		result[i] = tolower(result[i]);

	return string(result);
}

string Utils::toString(const int num){
	return std::to_string(num);
}

string Utils::toString(const unsigned int num){
	return std::to_string(num);
}
