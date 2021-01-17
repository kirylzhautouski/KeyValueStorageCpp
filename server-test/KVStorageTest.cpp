#include <vector>
#include <experimental/filesystem>

#include "pch.h"

#include "KVStorage.h"

void CleanStorage() {
	system("RMDIR /S /Q segments");
}

TEST(TestKVStorage, TestCreateKeySpace) {
	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");

	std::vector<std::string> key_spaces = storage.ListAllKeySpaces();
	EXPECT_TRUE(std::find(key_spaces.begin(), key_spaces.end(), "key_space1") != key_spaces.end());

	CleanStorage();
}

TEST(TestKVStorage, TestCreateExistingKeySpace) {
	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");

	EXPECT_ANY_THROW(storage.CreateKeySpace("key_space1"));

	CleanStorage();
}

TEST(TestKVStorage, TestDeleteKeySpace) {
	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");

	std::vector<std::string> key_spaces = storage.ListAllKeySpaces();
	EXPECT_TRUE(std::find(key_spaces.begin(), key_spaces.end(), "key_space1") != key_spaces.end());

	storage.DeleteKeySpace("key_space1");
	key_spaces = storage.ListAllKeySpaces();
	EXPECT_TRUE(std::find(key_spaces.begin(), key_spaces.end(), "key_space1") == key_spaces.end());

	CleanStorage();
}

TEST(TestKVStorage, TestDeleteNonExistentKeySpace) {
	KeyValueStorageCpp::KVStorage storage;

	EXPECT_ANY_THROW(storage.DeleteKeySpace("key_space1"));

	CleanStorage();
}

TEST(TestKVStorage, TestEmptyKeySpaceCountKeys) {
	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");

	EXPECT_EQ(storage.CountKeys("key_space1"), 0);

	CleanStorage();
}

TEST(TestKVStorage, TestSet) {
	std::vector<char> key = { 'a', 'b', 'c' };
	std::vector<char> value = { 'q', 'w', 'e', 'r', 't' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	storage.Set("key_space1", key, value);

	EXPECT_EQ(storage.Get("key_space1", key), value);
	EXPECT_EQ(storage.CountKeys("key_space1"), 1);

	CleanStorage();
}

TEST(TestKVStorage, TestMultipleSet) {
	std::vector<char> key1 = { 'a', 'b', 'c' };
	std::vector<char> value1 = { 'q', 'w', 'e', 'r', 't' };

	std::vector<char> key2 = { 'z', 'x', 'c' };
	std::vector<char> value2 = { 'v', 'b', 'n', 'm', 'q' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	storage.Set("key_space1", key1, value1);
	storage.Set("key_space1", key2, value2);

	EXPECT_EQ(storage.Get("key_space1", key1), value1);
	EXPECT_EQ(storage.Get("key_space1", key2), value2);
	EXPECT_EQ(storage.CountKeys("key_space1"), 2);

	CleanStorage();
}

TEST(TestKVStorage, TestUpdate) {
	std::vector<char> key = { 'a', 'b', 'c' };
	std::vector<char> value1 = { 'q', 'w', 'e', 'r', 't' };
	std::vector<char> value2 = { 'v', 's', 'a', 'b', '1' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	storage.Set("key_space1", key, value1);
	storage.Set("key_space1", key, value2);

	EXPECT_EQ(storage.Get("key_space1", key), value2);
	EXPECT_EQ(storage.CountKeys("key_space1"), 1);

	CleanStorage();
}

TEST(TestKVStorage, TestExists) {
	std::vector<char> key = { 'a', 'b', 'c' };
	std::vector<char> value1 = { 'q', 'w', 'e', 'r', 't' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	storage.Set("key_space1", key, value1);

	EXPECT_TRUE(storage.Exists("key_space1", key));
	EXPECT_FALSE(storage.Exists("key_space1", { 'z', 'f' }));

	CleanStorage();
}

TEST(TestKVStorage, TestGetUnesitent) {
	std::vector<char> key = { 'a', 'b', 'c' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	EXPECT_FALSE(storage.Exists("key_space1", key));

	CleanStorage();
}

TEST(TestKVStorage, TestRemove) {
	std::vector<char> key = { 'a', 'b', 'c' };
	std::vector<char> value1 = { 'q', 'w', 'e', 'r', 't' };

	KeyValueStorageCpp::KVStorage storage;

	storage.CreateKeySpace("key_space1");
	storage.Set("key_space1", key, value1);

	EXPECT_TRUE(storage.Exists("key_space1", key));

	storage.Remove("key_space1", key);
	EXPECT_FALSE(storage.Exists("key_space1", key));
	EXPECT_EQ(storage.CountKeys("key_space1"), 0);

	CleanStorage();
}
