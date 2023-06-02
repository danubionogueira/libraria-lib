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
