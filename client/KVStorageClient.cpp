#include "KVStorageClient.h"

KeyValueStorageCpp::KVStorageClient::~KVStorageClient()
{
}

std::string processOneParameterResponse(std::string response) {
	boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(response, ":", 0);
	boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(response, "\n", 1);
	std::string result = response.substr(std::distance(response.begin(), r1.begin()) + 1, r2.begin() - r1.begin() - 1);
	return result;
}

void KeyValueStorageCpp::KVStorageClient::CreateKeySpace(const std::string& key_space)
{
	RequestMessage::Request req("CreateKeySpace", key_space);
	std::string result = do_request(req, "CreateKeySpace");
	if (result != "Ok") {
		std::cout << "Result: " << result << std::endl;
	}
}

void KeyValueStorageCpp::KVStorageClient::DeleteKeySpace(const std::string& key_space)
{
	RequestMessage::Request req("DeleteKeySpace", key_space);
	std::string result = do_request(req, "DeleteKeySpace");
	if (result != "Ok") {
		std::cout << "Result: " << result << std::endl;
	}
}

std::vector<std::string> KeyValueStorageCpp::KVStorageClient::ListAllKeySpaces()
{
	RequestMessage::Request req("ListAllKeySpaces", "");
	std::string result = do_request(req, "ListAllKeySpaces");

	std::stringstream s;
	s << result;
	boost::archive::text_iarchive ia(s);
	std::vector<std::string> res;
	ia >> res;
	/*for (auto i : res)
		std::cout << i << ' ';*/
	return res;
}

int KeyValueStorageCpp::KVStorageClient::CountKeys(const std::string& key_space)
{
	RequestMessage::Request req("CountKeys", key_space);
	std::string result = do_request(req, "CountKeys");

	//std::cout << "Result: " << std::stoi(result) << std::endl;
	return std::stoi(result);
}

void KeyValueStorageCpp::KVStorageClient::RemoveAll(const std::string& key_space)
{
	RequestMessage::Request req("RemoveAll", key_space);
	std::string result = do_request(req, "RemoveAll");

	//std::cout << "Result: " << result << std::endl;
}

std::string KeyValueStorageCpp::KVStorageClient::doOneRequestParameter(const std::string& type, const std::string& param)
{
	this->socket_.connect(this->endpoint_);
	boost::system::error_code error;
	RequestMessage::Request req(type, param);
	boost::asio::write(this->socket_, boost::asio::buffer(req.data(), 500), error);
	if (!error) {
		cout << "successfully send " + type << endl;
	}
	else {
		cout << "error while sending " + type << endl;
	}
	boost::asio::streambuf buf;
	boost::asio::read_until(socket_, buf, "\n");
	string data = boost::asio::buffer_cast<const char*>(buf.data());
	this->socket_.close();

	return data;
}


std::string KeyValueStorageCpp::KVStorageClient::do_request(RequestMessage::Request req, const std::string& type)
{
	this->socket_.connect(this->endpoint_);
	boost::system::error_code error;
	boost::asio::write(this->socket_, boost::asio::buffer(req.data(), 500), error);
	if (!error) {
		cout << "successfully send " + type << endl;
	}
	else {
		cout << "error while sending " + type << endl;
	}

	boost::asio::streambuf buf;
	boost::asio::read_until(socket_, buf, "\n");
	string data = boost::asio::buffer_cast<const char*>(buf.data());
	this->socket_.close();

	boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(data, ":", 0);
	boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(data, "\n", 0);
	std::string result = data.substr(std::distance(data.begin(), r1.begin()) + 1, r2.begin() - r1.begin() - 1);

	return result;
}





