#include <boost/asio.hpp>
#include <iostream>
#include "KVStorageClient.h"
#include <boost/any.hpp>
#include <boost/archive/text_oarchive.hpp>
#include "ResultSet.h"
#include "Entry.h"
#include <vector>
#include "Person.h"


using boost::asio::ip::tcp;
using namespace std;

#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <iostream>
#include <fstream>


int main() {

	try {

		
		tcp::endpoint endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 1337);
		KeyValueStorageCpp::KVStorageClient client(endpoint);


		client.CreateKeySpace("key_space_1");
		client.CreateKeySpace("key_space_2");
		client.CreateKeySpace("key_space_3");
		std::vector<std::string> key_spaces = client.ListAllKeySpaces();
		
		for (auto i : key_spaces) {
			std::cout <<" "<< i << std::endl;
		}
		std::cout << std::endl;

		client.DeleteKeySpace("key_space_3");

		std::vector<std::string> key_spaces_upd = client.ListAllKeySpaces();

		for (auto i : key_spaces_upd) {
			std::cout << " " << i << std::endl;
		}
		std::cout << std::endl;

		client.Set<std::string, int>("key_space_1", "key1", 1337);
		client.Set<std::string, int>("key_space_1", "key2", 1488);

		int n_keys = client.CountKeys("key_space_1");
		std::cout << "Number of keys in key_space_1: " << n_keys << std::endl <<std::endl;

		KeyValueStorageCpp::Person p("Vova", 21);
		client.Set<std::string, KeyValueStorageCpp::Person>("key_space_2", "key1", p);
		bool if_person_exists = client.Exists<std::string>("key_space_2", "key1");
		if (if_person_exists) {
			std::cout << "Person p successfully inserted" << std::endl;
		}
		std::cout << std::endl;

		KeyValueStorageCpp::Person p_ = p;
		p_ = client.Get<std::string, KeyValueStorageCpp::Person>("key_space_2", "key1");
		p_.print();
		std::cout << std::endl;
		
		bool if_person_removed = client.Remove<std::string>("key_space_2", "key1");
		if_person_exists = client.Exists<std::string>("key_space_2", "key1");
		if (!if_person_exists) {
			std::cout << "Person p was successfully removed" << std::endl;
		}
		std::cout << std::endl;

		std::vector<std::pair<std::string, int>> result = client.GetAllKVs<std::string, int>("key_space_1");
		for (auto p : result) {
			std::cout << p.first << " " << p.second << std::endl;
		}
		std::cout << std::endl;

		client.RemoveAll("key_space_1");
		n_keys = client.CountKeys("key_space_1");
		std::cout << "Number of keys in key_space_1: " << n_keys << std::endl;

	}
	catch (std::exception& e)
	{
		std::cerr << "Exception: " << e.what() << "\n";
	}
}