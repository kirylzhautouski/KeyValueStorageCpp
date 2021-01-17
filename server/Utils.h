#pragma once

#include <vector>

std::vector<char> Slice(const std::vector<char>& v, int i, int j);

std::vector<char> SerializeInt(int i);
int DeserializeInt(const std::vector<char>& bytes);