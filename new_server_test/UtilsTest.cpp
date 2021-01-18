#include <vector>

#include "pch.h"

#include "Utils.h"

TEST(TestUtils, TestSlice) {
	std::vector<char> v = { 'a', 'b', 'c', 'd', 'e' };
	std::vector<char> expected = { 'c', 'd', 'e' };
	EXPECT_EQ(Slice(v, 2, 5), expected);
}

TEST(TestUtils, TestSerialization) {
	int i = 5;
	EXPECT_EQ(DeserializeInt(SerializeInt(i)), i);
}

TEST(TestUtils, TestSerializationNegatives) {
	int i = -128;
	EXPECT_EQ(DeserializeInt(SerializeInt(i)), i);
}
