#pragma once

#include <string>
#include <vector>
#include <unordered_map>

#include "Index.h"

namespace KeyValueStorageCpp {
	class KVStorage {
	private:
		static const std::string SEGMENTS_PATH_;
		static const std::string SEGMENT_FILE_NAME_;
		static const std::string SEGMENT_FILE_EXTENSION;

		std::unordered_map<std::string, Index> index_for_key_space_;

		std::string BuildKeySpaceSegmentsPath_(const std::string& key_space);
		std::string BuildSegmentFileName_(int segment_number);
		std::string BuildSegmentFilePath_(const std::string& key_space, int segment_number);
	public:
		KVStorage();

		// throws exception if keyspace already exists
		void CreateKeySpace(const std::string& key_space);
		// throws exception if keyspace does not exist
		void DeleteKeySpace(const std::string& key_space);

		std::vector<std::string> ListAllKeySpaces();

		int CountKeys(const std::string& key_space);

		bool Exists(const std::string& key_space, const std::vector<char>& key);
		void Set(const std::string& key_space, const std::vector<char>& key, const std::vector<char>& value);
		// returns true if key existed, false otherwise
		bool Remove(const std::string& key_space, const std::vector<char>& key);
		// throws if the specified key does not exist
		std::vector<char> Get(const std::string& key_space, const std::vector<char>& key);
		void RemoveAll(const std::string& key_space);

		std::vector<std::pair<std::vector<char>, std::vector<char>>> GetAllKVs(const std::string& key_space);

	};
}
