#pragma once
#include <utility>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace KeyValueStorageCpp {

	template <typename K, typename V>
	class Entry {
	public:

		Entry() {};
		std::pair<K, V> pair;
		Entry<K, V>(K k, V v) {
			pair = std::pair<K, V>(k, v);
		}


		template<class Archive>
		void serialize(Archive& ar, const unsigned int version)
		{
			ar& this->pair.first;
			ar& this->pair.second;
		}
	};
}
