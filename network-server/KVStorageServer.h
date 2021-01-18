#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include "Request.h"
#include "Response.h"
#include <iostream>
#include "Entry.h"
#include "ResultSet.h"
#include <utility>
#include "KVStorage.h"

using std::cout;
using std::endl;
using std::string;
using boost::asio::ip::tcp;

namespace KeyValueStorageCpp {
	class KVStorageServer {
	public:
		KVStorageServer(const tcp::endpoint& endpoint) : io_context_(), acceptor_(io_context_, endpoint) {
			do_accept();
		};

	private:
		KeyValueStorageCpp::KVStorage kv_storage;
		void do_accept();
		void do_process_request(tcp::socket& socket, const std::string& message);
		string do_read(tcp::socket& socket);
		void do_write(tcp::socket& socket, const string& message);
		boost::asio::io_context io_context_;
		tcp::acceptor acceptor_;

		std::map<std::string, ResponseMessage::Response(*)(RequestMessage::Request, KeyValueStorageCpp::KVStorage&)> request_map;
	};
}
