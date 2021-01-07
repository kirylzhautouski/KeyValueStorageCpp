#include "NotExistingKeySpaceException.h"

namespace KeyValueStorageCpp {
	NotExistingKeySpaceException::NotExistingKeySpaceException(const std::string& key_space)
		: key_space(key_space) {
	}

	const char* NotExistingKeySpaceException::what() const throw () {
		return (key_space + "doesn't exist.").c_str();
	}

}