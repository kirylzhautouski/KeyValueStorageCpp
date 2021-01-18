#include "KVStorageServer.h"

std::string getOneRequestParameter(RequestMessage::Request request) {
	std::string request_data = request.string_data();
	boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(request_data, ":", 0);
	boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(request_data, ":", 1);
	std::string body = request_data.substr(std::distance(request_data.begin(), r1.begin()) + 1, r2.begin() - r1.begin() - 1);
	std::cout << "parameter: " << body << std::endl;
	return body;
}

ResponseMessage::Response CreateKeySpaceHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "CreateKeySpaceHandler" << endl;
	std::string body = getOneRequestParameter(request);
	std::string request_result = "Ok";
	try {
		kv_storage.CreateKeySpace(body);
	}
	catch(const std::invalid_argument& ia)
	{
		request_result = "KeySpaceAlreadyExists";
	}
	return ResponseMessage::Response("CreateKeySpaceResponse", request_result);
}

ResponseMessage::Response DeleteKeySpaceHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "DeleteKeySpaceHandler" << endl;
	std::string body = getOneRequestParameter(request);
	std::string request_result = "Ok";
	try {
		kv_storage.DeleteKeySpace(body);
	}
	catch (const std::invalid_argument& ia)
	{
		request_result = "KeySpaceDoesntExist";
	}

	return ResponseMessage::Response("DeleteKeySpaceResponse", request_result);
}

ResponseMessage::Response ListAllKeySpacesHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "ListAllKeySpacesHandler" << endl;

	std::stringstream ss; {
		boost::archive::text_oarchive oa(ss);
		std::vector<std::string> vec = kv_storage.ListAllKeySpaces();
		oa << vec;
	}
	std::string res = ss.str().data();
	return ResponseMessage::Response("ListAllKeySpacesResponse", res);
}

ResponseMessage::Response CountKeysHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "CountKeysHandler" << endl;
	std::string body = getOneRequestParameter(request);
	int n_keys = kv_storage.CountKeys(body);
	std::string n_str = std::to_string(n_keys);
	return ResponseMessage::Response("CountKeysResponse", n_str);
}

std::pair<std::string, std::string> key_space_key(RequestMessage::Request request) 
{
	std::string request_data = request.string_data();
	boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(request_data, ":", 0);
	boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(request_data, ":", 1);
	boost::iterator_range<std::string::iterator> r3 = boost::algorithm::find_nth(request_data, "\n", 0);
	std::string key_space = request_data.substr(std::distance(request_data.begin(), r1.begin()) + 1, r2.begin() - r1.begin() - 1);
	std::string key = request_data.substr(std::distance(request_data.begin(), r2.begin()) + 1, r3.begin() - r2.begin() - 1);
	return std::pair<std::string, std::string>(key_space, key);
}

ResponseMessage::Response ExistsHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "ExistsHandler" << endl;
	std::pair<std::string, std::string> space_key = key_space_key(request);
	std::cout << "key_space: " << space_key.first << std::endl;
	std::cout << "key: " << space_key.second << std::endl;

	std::vector<char> key(space_key.second.begin(), space_key.second.end());
	bool res = kv_storage.Exists(space_key.first, key);
	std::string response_ok = res ? "Ok" : "Fail";
	return ResponseMessage::Response("ExistsResponse", response_ok);
}

ResponseMessage::Response SetHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "SetHandler" << endl;
	std::string request_data = request.string_data();
	boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(request_data, ":", 0);
	boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(request_data, ":", 1);
	boost::iterator_range<std::string::iterator> r3 = boost::algorithm::find_last(request_data, "|");
	boost::iterator_range<std::string::iterator> r4 = boost::algorithm::find_last(request_data, "\n");
	std::string key_space = request_data.substr(std::distance(request_data.begin(), r1.begin()) + 1, r2.begin() - r1.begin()-1);
	std::string key = request_data.substr(std::distance(request_data.begin(), r2.begin()) + 1, r3.begin() - r2.begin()-2);
	std::string value = request_data.substr(std::distance(request_data.begin(), r3.begin()) + 1, r4.begin() - r3.begin());

	std::cout << "key_space: " << key_space << std::endl;
	std::cout << "key: " << key << std::endl;
	std::cout << "value: " << value << std::endl;

	std::vector<char> key_vec(key.begin(), key.end());
	std::vector<char> value_vec(value.begin(), value.end());

	kv_storage.Set(key_space, key_vec, value_vec);

	bool res = true;
	std::string response_ok = res ? "Ok" : "Fail";
	return ResponseMessage::Response("SetResponse", "Ok");
}

ResponseMessage::Response RemoveHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "RemoveHandler" << endl;
	std::pair<std::string, std::string> space_key = key_space_key(request);
	std::cout << "key_space: " << space_key.first << std::endl;
	std::cout << "key: " << space_key.second << std::endl;

	std::vector<char> key_vec(space_key.second.begin(), space_key.second.end());
	bool res = kv_storage.Remove(space_key.first, key_vec);

	std::string response_ok = res ? "Ok" : "Fail";
	return ResponseMessage::Response("RemoveResponse", response_ok);
}

ResponseMessage::Response GetHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "GetHandler" << endl;

	std::pair<std::string, std::string> space_key = key_space_key(request);
	std::cout << "key_space: " << space_key.first << std::endl;
	std::cout << "key: " << space_key.second << std::endl;

	std::vector<char> key_vec(space_key.second.begin(), space_key.second.end());
	std::vector<char> v;
	std::string result;

	try {
		v = kv_storage.Get(space_key.first, key_vec);
		result = std::string(v.begin(), v.end());
	}
	catch (const std::invalid_argument& ia) {
		result = "NoSuchKey";
	}

	return ResponseMessage::Response("GetResponse", result);
}

ResponseMessage::Response RemoveAllHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "RemoveAllHandler" << endl;
	std::string body = getOneRequestParameter(request);
	//do some
	kv_storage.RemoveAll(body);
	return ResponseMessage::Response("RemoveAllResponse", "Ok");
}

ResponseMessage::Response GetAllKVsHandler(RequestMessage::Request request, KeyValueStorageCpp::KVStorage &kv_storage)
{
	cout << "GetAllKVsHandler" << endl;
	std::string body = getOneRequestParameter(request);

	std::vector<std::pair<std::vector<char>, std::vector<char>>> vec = kv_storage.GetAllKVs(body);

	std::stringstream ss; {
		boost::archive::text_oarchive oa(ss);
		oa << vec;
	}

	std::string res = ss.str().data();
	return ResponseMessage::Response("GetAllKVsResponse", res);
}

void KeyValueStorageCpp::KVStorageServer::do_accept()
{
	this->request_map = {
			{"CreateKeySpace", CreateKeySpaceHandler},
			{"DeleteKeySpace", DeleteKeySpaceHandler},
			{"ListAllKeySpaces", ListAllKeySpacesHandler },
			{"CountKeys", CountKeysHandler },
			{"Exists", ExistsHandler },
			{"Set", SetHandler },
			{"Remove", RemoveHandler },
			{"Get", GetHandler },
			{"RemoveAll", RemoveAllHandler },
			{"GetAllKVs", GetAllKVsHandler }
	};

	while (true)
	{
		tcp::socket socket_(this->io_context_);
		this->acceptor_.accept(socket_);
		string msg = do_read(socket_);
		do_process_request(socket_, msg);
	}

}

void KeyValueStorageCpp::KVStorageServer::do_process_request(tcp::socket& socket, const std::string& message)
{
	int request_type_pos = message.find_first_of(":");
	string request = message.substr(0, request_type_pos);

	if (request_map.find(request) != request_map.end()) {
		auto f = request_map[request];
		RequestMessage::Request req(message);
		ResponseMessage::Response response = f(req, this->kv_storage);
		do_write(socket, response.string_data());
	}
	else {
		cout << "problem" << endl;
	}
}

string KeyValueStorageCpp::KVStorageServer::do_read(tcp::socket& socket)
{
	boost::asio::streambuf buf;
	boost::asio::read_until(socket, buf, "\n");
	string data = boost::asio::buffer_cast<const char*>(buf.data());
	return data;
}

void KeyValueStorageCpp::KVStorageServer::do_write(tcp::socket& socket, const string& message)
{
	const string msg = message + "\n";
	boost::asio::write(socket, boost::asio::buffer(msg));
}


