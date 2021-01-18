#pragma once

#include <string>
#include <vector>
#include <iostream>
#include "ResultSet.h"
#include <boost/asio.hpp>
#include <boost/core/ref.hpp>
#include <boost/make_shared.hpp>
#include <boost/bind.hpp>
#include <boost/algorithm/string/find.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/serialization/base_object.hpp>
#include <boost/serialization/binary_object.hpp>
#include <boost/serialization/string.hpp>
#include <boost/serialization/list.hpp>
#include <boost/serialization/vector.hpp>
#include <boost/serialization/utility.hpp>
#include "Request.h"


using std::cout;
using std::endl;
using std::string;
using boost::asio::ip::tcp;

namespace KeyValueStorageCpp {

	class KVStorageClient {
	public:
		KVStorageClient(const tcp::endpoint& endpoint) : io_context_(), socket_(io_context_), endpoint_(endpoint) {};
		~KVStorageClient();

		// throws exception if keyspace already exists
		void CreateKeySpace(const std::string& key_space);
		// throws exception if keyspace does not exist
		void DeleteKeySpace(const std::string& key_space);

		std::vector<std::string> ListAllKeySpaces();

		int CountKeys(const std::string& key_space);

		template <typename K>
		bool Exists(const std::string& key_space, const K& key) throw();

		template <typename K, typename V>
		void Set(const std::string& key_space, const K& key, const V& value) throw();

		// returns true if key existed, false otherwise
		template <typename K>
		bool Remove(const std::string& key_space, const K& key) throw();

		// throws if the specified key does not exist
		template <typename K, typename V>
		V Get(const std::string& key_space, const K& key) throw();
		void RemoveAll(const std::string& key_space);

		template <typename K, typename V>
		std::vector<std::pair<K, V>> GetAllKVs(const std::string& key_space);

	private:
		boost::asio::io_context io_context_;
		tcp::socket socket_;
		tcp::endpoint endpoint_;

		std::string do_request(RequestMessage::Request req, const std::string& type);
		std::string doOneRequestParameter(const std::string& type, const std::string& param);
	};
	template<typename K>
	inline bool KVStorageClient::Exists(const std::string& key_space, const K& key) throw()
	{
		std::stringstream ss; {
			boost::archive::text_oarchive oa(ss);
			oa << key;
		}
		std::string res = ss.str().data();
		RequestMessage::Request req("Exists", key_space, res);
		std::string result = do_request(req, "Exists");

		if (result == "Ok") {
			return true;
		}
		return false;
	}
	template<typename K, typename V>
	inline void KVStorageClient::Set(const std::string& key_space, const K& key, const V& value) throw()
	{
		std::stringstream ss_key; {
			boost::archive::text_oarchive oa_key(ss_key);
			oa_key << key;
		}
		std::string res_key = ss_key.str().data();

		std::stringstream ss_value; {
			boost::archive::text_oarchive oa_value(ss_value);
			oa_value << value;
		}
		std::string res_value = ss_value.str().data();

		RequestMessage::Request req("Set", key_space, res_key, res_value);
		std::string result = do_request(req, "Set");

		//std::cout << "Result: " << result << std::endl;
	}
	template<typename K>
	inline bool KVStorageClient::Remove(const std::string& key_space, const K& key) throw()
	{

		std::stringstream ss; {
			boost::archive::text_oarchive oa(ss);
			oa << key;
		}
		std::string res = ss.str().data();
		RequestMessage::Request req("Remove", key_space, res);
		std::string result = do_request(req, "Remove");

		if (result == "Ok") {
			return true;
		}
		return false;
	}
	template<typename K, typename V>
	inline V KVStorageClient::Get(const std::string& key_space, const K& key) throw()
	{

		std::stringstream ss; {
			boost::archive::text_oarchive oa(ss);
			oa << key;
		}
		std::string res = ss.str().data();
		RequestMessage::Request req("Get", key_space, res);
		std::string result = do_request(req, "Get");

		if (result == "NoSuchKey") {
			throw std::invalid_argument("no such key");
		}

		std::stringstream s;
		s << result;
		boost::archive::text_iarchive ia(s);
		V value;
		ia >> value;
		return value;
	}
	template<typename K, typename V>
	inline std::vector<std::pair<K, V>> KVStorageClient::GetAllKVs(const std::string& key_space)
	{
		std::string response = doOneRequestParameter("GetAllKVs", key_space);

		boost::iterator_range<std::string::iterator> r1 = boost::algorithm::find_nth(response, ":", 0);
		boost::iterator_range<std::string::iterator> r2 = boost::algorithm::find_nth(response, "\n", 0);
		std::string result = response.substr(std::distance(response.begin(), r1.begin()) + 1, r2.begin() - r1.begin() - 1);

		std::vector<std::pair<std::vector<char>, std::vector<char>>> vec; {
			std::stringstream s;
			s << result;
			boost::archive::text_iarchive ia(s);
			ia >> vec;
		}

		std::vector<std::pair<K, V>> result_vec;

		for (auto pair : vec) {
			K k; 
			{
				std::string k_str(pair.first.begin(), pair.first.end());
				std::stringstream s;
				s << k_str;
				boost::archive::text_iarchive ia(s);
				ia >> k;
			}

			V v;
			{
				std::string v_str(pair.second.begin(), pair.second.end());
				std::stringstream s;
				s << v_str;
				boost::archive::text_iarchive ia(s);
				ia >> v;
			}
			result_vec.push_back(std::pair<K,V>(k, v));
		}

		return result_vec;
	}
}
