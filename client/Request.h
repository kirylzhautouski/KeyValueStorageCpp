#pragma once
#include <string>

namespace RequestMessage {
	class Request {
	public:
		enum { header_length = 16 };
		enum { max_body_length = 512 };

		Request() {};
		Request(const std::string& type);
		Request(const std::string& type, const std::string& key_space);
		Request(const std::string& type, const std::string& key_space, const std::string& key);
		Request(const std::string& type, const std::string& key_space, const std::string& key, const std::string& value);

		char* data() { return data_; };
	private:
		char data_[header_length + max_body_length];
	};
}