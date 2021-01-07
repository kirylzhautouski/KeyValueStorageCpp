#pragma once

#include <string>
#include <exception>

namespace KeyValueStorageCpp {
	template <typename K>
	class NotExistingKeyException {
	private:
		K key;
	public:
		NotExistingKeyException(const K& key);
		const char* what() const throw();
	};
}
