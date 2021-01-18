#include "Response.h"

ResponseMessage::Response::Response(const std::string& type, const std::string& body) {
	std::string res = type + ":" + body;
	int len = res.length();
	std::snprintf(data_, len + 1, "%s", res.c_str());
	data_[len + 1] = '\n';
}

char* ResponseMessage::Response::data() {
	return data_;
}

const std::string ResponseMessage::Response::string_data() {
	return std::string(data_);
}
