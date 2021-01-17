#pragma once

#include <vector>

namespace KeyValueStorageCpp {
	class SegmentFileEntry {
	public:
		enum class EntryType {
			ADD = 0,
			REMOVE = 1
		};

		std::vector<char> ToBytes();

		std::vector<char> GetValue();
		std::vector<char> GetKey();
		EntryType GetEntryType();

		static SegmentFileEntry FromBytes(const std::vector<char>& bytes);
		static SegmentFileEntry NewAddEntry(const std::vector<char>& key, const std::vector<char>& value);
		static SegmentFileEntry NewRemoveEntry(const std::vector<char>& key);

		bool operator==(const SegmentFileEntry& other) const;
	private:
		SegmentFileEntry(EntryType entryType, const std::vector<char>& key, const std::vector<char>& value);

		// stored as <type|size_of_key|key|size_of_value|value>
		// type is EntryType
		// size is measured in bytes and are ints
		EntryType entryType_;

		std::vector<char> key_;
		std::vector<char> value_;

	};
}
