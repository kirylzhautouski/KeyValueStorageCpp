#pragma once
#include <string>

namespace ResponseMessage {
	class Response {
	public:
		enum { header_length = 16 };
		enum { max_body_length = 512 };

		Response() {};
		Response(const std::string& type, const std::string& key, const std::string& value) {
			/*char* ch_type = new char[type.length() + 1];
			strcpy(ch_type, type.c_str());
			std::memcpy(data_, ch_type + ':', type.size());*/
		}

	private:
		char data_[header_length + max_body_length];
	};
}