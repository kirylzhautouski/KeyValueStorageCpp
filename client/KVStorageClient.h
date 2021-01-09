#pragma once

#include <string>
#include <vector>

#include "ResultSet.h"

namespace KeyValueStorageCpp {

	// TODO: change templating here
	// KVStorageClient can be a cleint for different entities
	// template just methods for CRUD
	template <typename K, typename V>
	class KVStorageClient {
	public:
		KVStorageClient(const std::string& host, int port);
		~KVStorageClient();

		// throws exception if keyspace already exists
		void CreateKeySpace(const std::string& key_space);
		// throws exception if keyspace does not exist
		void DeleteKeySpace(const std::string& key_space);

		std::vector<std::string> ListAllKeySpaces();

		int CountKeys(const std::string& key_space);

		bool Exists(const std::string& key_space, const K& key) throw();
		void Set(const std::string& key_space, const K& key, const V& value) throw();
		// returns true if key existed, false otherwise
		bool Remove(const std::string& key_space, const K& key) throw();
		// throws if the specified key does not exist
		V Get(const std::string& key_space, const K& key) throw();
		void RemoveAll(const std::string& key_space);

		ResultSet<K, V> GetAllKVs(const std::string& key_space);
		
	};
}
