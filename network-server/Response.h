#pragma once
#include <string>

namespace ResponseMessage {
	class Response {
	public:
		enum { header_length = 16 };
		enum { max_body_length = 1024 };

		Response() {};
		Response(const std::string& type, const std::string& body);
		char* data();
		const std::string string_data();

	private:
		char data_[header_length + max_body_length];
	};
}
