#include <vector>

#include "pch.h"

#include "Index.h"

TEST(IndexTest, TestSetPosition) {
	std::vector<char> key = { 'a', 'b', 'c' };

	KeyValueStorageCpp::Index index = KeyValueStorageCpp::Index();
	index.SetKeyPosition(key, 5);
	EXPECT_EQ(index.GetKeyPosition(key), 5);
	EXPECT_TRUE(index.KeyExists(key));
	EXPECT_EQ(index.CountKeys(), 1);
}

TEST(IndexTest, TestRemoveKey) {
	std::vector<char> key = { 'a', 'b', 'c' };

	KeyValueStorageCpp::Index index = KeyValueStorageCpp::Index();
	index.SetKeyPosition(key, 5);
	EXPECT_EQ(index.GetKeyPosition(key), 5);
	EXPECT_TRUE(index.KeyExists(key));
	EXPECT_EQ(index.CountKeys(), 1);

	index.RemoveKey(key);
	EXPECT_FALSE(index.KeyExists(key));
	EXPECT_EQ(index.CountKeys(), 0);
}
