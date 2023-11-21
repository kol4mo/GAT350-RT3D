#pragma once
#include <string>
#include <sstream>

namespace nc
{
	inline static int uniqstr = 0;
	inline std::string ToUpper(std::string input) {
		for (int i = 0; i < input.length(); i++)
		{
			input[i] = ((int)input[i] > 96 && (int)input[i] < 123) ? (char)((int)input[i] - 32) : input[i];
		}
		return input;
	}

	inline std::string ToLower(std::string input) {
		for (int i = 0; i < input.length(); i++)
		{
			input[i] = ((int)input[i] > 64 && (int)input[i] < 91) ? (char)((int)input[i] + 32) : input[i];
		}
		return input;
	}

	inline bool IsEqualIgnoreCase(std::string a, std::string b) {
		if (a.length() != b.length()) {
			return false;
		}
		for (int i = 0; i < a.length(); i++)
		{
			if (a[i] + 32 == b[i] || a[i] == b[i] || a[i] - 32 == b[i]) {
				
			}
			else {
				return false;
			}
		}
		return true;
	}

	inline std::string CreateUniqueStr(std::string input) {
		std::stringstream ss;
		ss << uniqstr;
		std::string string = ss.str();

		std::string newString = input.append(" " + string);

		uniqstr++;

		return newString;
	}

}