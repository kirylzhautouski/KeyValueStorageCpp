#pragma once

#include <string>
#include <exception>

namespace KeyValueStorageCpp {
	class NotExistingKeySpaceException: public std::exception {
	private:
		std::string key_space;
	public:
		NotExistingKeySpaceException(const std::string& key_space);
		const char* what() const throw();
	};
}

