#include "Entry.h"

template <typename K, typename V>
KeyValueStorageCpp::Entry<K,V>::Entry(K k, V v) { 
	pair = std::pair<K, V>(k, v);
}

template <typename K, typename V>
template <class Archive>
void KeyValueStorageCpp::Entry<K, V>::serialize(Archive& ar, const unsigned int version) {
	ar& this->pair.first;
	ar& this->pair.second;
}
