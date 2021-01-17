#pragma once

#include <unordered_map>
#include <vector>

namespace KeyValueStorageCpp {
	struct VectorHasher {
		int operator()(const std::vector<char> &V) const {
			int hash = V.size();
			for (auto &i : V) {
				hash ^= i + 0x9e3779b9 + (hash << 6) + (hash >> 2);
			}
			return hash;
		}
	};

	class Index {
	private:
		std::unordered_map<std::vector<char>, int, VectorHasher> index_;
	public:
		Index();

		int GetKeyPosition(const std::vector<char>& key);
		void SetKeyPosition(const std::vector<char>& key, int position);
		void RemoveKey(const std::vector<char>& key);
		void RemoveAllKeys();
		bool KeyExists(const std::vector<char>& key);

		std::unordered_map<std::vector<char>, int, VectorHasher> GetAllKeys();

		int CountKeys();
	};
}
