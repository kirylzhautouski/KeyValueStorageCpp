#include "KVStorageServer.h"
#include <boost/asio.hpp>
#include <iostream>


using boost::asio::ip::tcp;
using namespace std;

int main() {

	try
	{
		tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1337);
		KeyValueStorageCpp::KVStorageServer server(endpoint);
	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}