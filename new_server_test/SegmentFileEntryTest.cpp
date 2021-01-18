#include <vector>

#include "pch.h"

#include "SegmentFileEntry.h"

TEST(TestSegmentFileEntry, TestSerializationAddEntry) {
	std::vector<char> key = { 'k', 'e', 'y' };
	std::vector<char> value = { 'v', 'a', 'l', 'u', 'e' };

	KeyValueStorageCpp::SegmentFileEntry entry = KeyValueStorageCpp::SegmentFileEntry::NewAddEntry(key, value);
	EXPECT_EQ(KeyValueStorageCpp::SegmentFileEntry::FromBytes(entry.ToBytes()), entry);
}

TEST(TestSegmentFileEntry, TestSerializationRemoveEntry) {
	std::vector<char> key = { 'k', 'e', 'y' };

	KeyValueStorageCpp::SegmentFileEntry entry = KeyValueStorageCpp::SegmentFileEntry::NewRemoveEntry(key);
	EXPECT_EQ(KeyValueStorageCpp::SegmentFileEntry::FromBytes(entry.ToBytes()), entry);
}
