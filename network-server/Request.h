#pragma once
#include <string>

namespace RequestMessage {
	class Request {
	public:
		enum { header_length = 16 };
		enum { max_body_length = 512 };

		Request() {};
		Request(const std::string& data);
		char* data();
		const std::string string_data();

	private:
		char data_[header_length + max_body_length];
	};
}