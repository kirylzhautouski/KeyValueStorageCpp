#include "Request.h"

RequestMessage::Request::Request(const std::string& data) {
	int len = data.length();
	std::snprintf(data_, len + 1, "%s", data.c_str());
}

char* RequestMessage::Request::data() {
	return data_;
}


const std::string RequestMessage::Request::string_data() { 
	return std::string(data_); 
}