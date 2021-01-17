#include <sstream>
#include <string>

#include "SegmentFileEntry.h"
#include "Utils.h"

KeyValueStorageCpp::SegmentFileEntry::SegmentFileEntry(EntryType entryType, 
	const std::vector<char>& key, const std::vector<char>& value) : entryType_(entryType), key_(key), value_(value) {
	
}

KeyValueStorageCpp::SegmentFileEntry 
KeyValueStorageCpp::SegmentFileEntry::NewAddEntry(const std::vector<char>& key, const std::vector<char>& value) {
	return SegmentFileEntry(EntryType::ADD, key, value);
}

KeyValueStorageCpp::SegmentFileEntry 
KeyValueStorageCpp::SegmentFileEntry::NewRemoveEntry(const std::vector<char>& key) {
	return SegmentFileEntry(EntryType::REMOVE, key, std::vector<char>());
}

bool KeyValueStorageCpp::SegmentFileEntry::operator==(const SegmentFileEntry& other) const {
	return this->key_ == other.key_ && this->value_ == other.value_ && this->entryType_ == other.entryType_;
}

std::vector<char> KeyValueStorageCpp::SegmentFileEntry::ToBytes() {
	std::vector<char> type_bytes = SerializeInt(static_cast<int>(entryType_));
	std::vector<char> key_size_bytes = SerializeInt(key_.size());

	std::vector<char> bytes;
	
	bytes.insert(bytes.end(), type_bytes.begin(), type_bytes.end());

	bytes.insert(bytes.end(), key_size_bytes.begin(), key_size_bytes.end());
	bytes.insert(bytes.end(), key_.begin(), key_.end()); 

	if (entryType_ == EntryType::ADD) {
		std::vector<char> value_size_bytes = SerializeInt(value_.size());
		bytes.insert(bytes.end(), value_size_bytes.begin(), value_size_bytes.end());
		bytes.insert(bytes.end(), value_.begin(), value_.end());
	}

	return bytes;
}

std::vector<char> KeyValueStorageCpp::SegmentFileEntry::GetValue() {
	return value_;
}

std::vector<char> KeyValueStorageCpp::SegmentFileEntry::GetKey() {
	return key_;
}

KeyValueStorageCpp::SegmentFileEntry::EntryType KeyValueStorageCpp::SegmentFileEntry::GetEntryType() {
	return entryType_;
}

KeyValueStorageCpp::SegmentFileEntry KeyValueStorageCpp::SegmentFileEntry::FromBytes(const std::vector<char>& bytes) {
	std::vector<char> type_bytes = Slice(bytes, 0, 4);
	EntryType entryType = static_cast<EntryType>(DeserializeInt(type_bytes));
	
	std::vector<char> size_of_key_bytes = Slice(bytes, 4, 8);
	int size_of_key = DeserializeInt(size_of_key_bytes);

	std::vector<char> key_bytes = Slice(bytes, 8, 8 + size_of_key);

	if (entryType == EntryType::ADD) {
		std::vector<char> size_of_value_bytes = Slice(bytes, 8 + size_of_key, 8 + size_of_key + 4);
		int size_of_value = DeserializeInt(size_of_value_bytes);

		std::vector<char> value_bytes = Slice(bytes, 8 + size_of_key + 4, 8 + size_of_key + 4 + size_of_value);

		return NewAddEntry(key_bytes, value_bytes);
	}

	return NewRemoveEntry(key_bytes);
}