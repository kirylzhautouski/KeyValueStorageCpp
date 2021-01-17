#include <vector>
#include <sstream>

#include "Utils.h"

std::vector<char> Slice(const std::vector<char>& v, int i, int j) {
	std::vector<char> result(j - i);
	std::copy(v.begin() + i, v.begin() + j, result.begin());
	return result;
}

std::vector<char> SerializeInt(int i) {
	std::stringstream ss;
	ss.write(reinterpret_cast<char const*>(&i), sizeof(i));
	std::string result = ss.str();
	return std::vector<char>(result.begin(), result.end());
}

int DeserializeInt(const std::vector<char>& bytes) {
	std::stringstream ss(std::string(bytes.begin(), bytes.end()));
	int i;
	ss.read(reinterpret_cast<char*>(&i), sizeof(int));
	return i;
}
