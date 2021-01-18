#pragma once

#include <vector>
#include "Entry.h"

namespace KeyValueStorageCpp {

	template <typename K, typename V>
	using ResultSetConstIterator = typename std::vector<Entry<K, V>>::const_iterator;

	template <typename K, typename V>
	class ResultSet {
	public:
		ResultSet() {};
		ResultSet(std::vector<Entry<K, V>> vec);

		ResultSetConstIterator<K, V> begin() const;
		ResultSetConstIterator<K, V> end() const;

		std::vector<Entry<K, V>> entries;

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version);
	};
}

