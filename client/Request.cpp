#include "Request.h"


RequestMessage::Request::Request(const std::string& type) {
	int len = type.length();
	std::snprintf(data_, len + 1, "%s", (type + ":").c_str());
	data_[len + 1] = '\n';
}

RequestMessage::Request::Request(const std::string& type, const std::string& key_space) {
	std::string req = type + ":" + key_space + ":";
	int len = req.length();
	std::snprintf(data_, len + 1, "%s", req.c_str());
	data_[len + 1] = '\n';
}

RequestMessage::Request::Request(const std::string& type, const std::string& key_space, const std::string& key) {
	std::string req = type + ":" + key_space + ":" + key;
	int len = req.length();
	std::snprintf(data_, len + 1, "%s", req.c_str());
	data_[len + 1] = '\n';
}

RequestMessage::Request::Request(const std::string& type, const std::string& key_space, const std::string& key, const std::string& value) {
	std::string req = type + ":" + key_space + ":" + key + "|" + value;
	int len = req.length();
	std::snprintf(data_, len + 1, "%s", req.c_str());
	data_[len + 1] = '\n';
}