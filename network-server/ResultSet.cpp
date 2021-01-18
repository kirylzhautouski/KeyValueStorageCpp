#include "ResultSet.h"

template <typename K, typename V>
KeyValueStorageCpp::ResultSet<K, V>::ResultSet(std::vector<Entry<K, V>> vec) {
	this->entries = vec;
}

template <typename K, typename V>
template<class Archive>
void KeyValueStorageCpp::ResultSet<K, V>::serialize(Archive& ar, const unsigned int version){
	ar& this->entries;
}
