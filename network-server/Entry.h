#pragma once
#include <utility>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>

namespace KeyValueStorageCpp {

	template <typename K, typename V>
	class Entry {
	public:
		std::pair<K, V> pair;

		Entry() {};
		Entry<K, V>(K k, V v);

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};
}

