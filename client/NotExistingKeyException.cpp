#include "NotExistingKeyException.h"

namespace KeyValueStorageCpp {
	template <typename K>
	NotExistingKeyException<K>::NotExistingKeyException(const K& key)
		: key(key) {
	}

	template <typename K>
	const char* NotExistingKeyException<K>::what() const throw () {
		return (key.ToString() + " doesn't exist.").c_str();
	}

}