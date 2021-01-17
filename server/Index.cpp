#include "Index.h"

KeyValueStorageCpp::Index::Index() : index_() {

}

int KeyValueStorageCpp::Index::GetKeyPosition(const std::vector<char>& key) {
	auto position_it = index_.find(key);

	if (position_it == index_.end()) {
		return -1;
	}

	return position_it->second;
}

void KeyValueStorageCpp::Index::SetKeyPosition(const std::vector<char>& key, int position) {
	index_[key] = position;
}

void KeyValueStorageCpp::Index::RemoveKey(const std::vector<char>& key) {
	index_.erase(key);
}

bool KeyValueStorageCpp::Index::KeyExists(const std::vector<char>& key) {
	if (index_.find(key) == index_.end()) {
		return false;
	}

	return true;
}

int KeyValueStorageCpp::Index::CountKeys() {
	return index_.size();
}
