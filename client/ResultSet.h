#pragma once

#include <vector>

#include "Entry.h"

namespace KeyValueStorageCpp {

	template <typename K, typename V>
	using ResultSetConstIterator = typename std::vector<Entry<K, V>>::const_iterator;

	template <typename K, typename V>
	class ResultSet {
	public:
		ResultSetConstIterator<K, V> begin() const;
		ResultSetConstIterator<K, V> end() const;
	};
}

