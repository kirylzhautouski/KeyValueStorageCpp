#include <exception>
#include <fstream>
#include <iostream>
#include <experimental/filesystem>

#include "KVStorage.h"
#include "SegmentFileEntry.h"
#include "Utils.h"

const std::string KeyValueStorageCpp::KVStorage::SEGMENTS_PATH_ = "segments/";
const std::string KeyValueStorageCpp::KVStorage::SEGMENT_FILE_NAME_ = "segment_";
const std::string KeyValueStorageCpp::KVStorage::SEGMENT_FILE_EXTENSION = ".seg";

KeyValueStorageCpp::SegmentFileEntry ReadEntry_(std::ifstream& segment_file) {
	int entry_size;
	segment_file.read((char*)&entry_size, 4);

	std::vector<char> entry_bytes(entry_size);
	segment_file.read(entry_bytes.data(), entry_size);

	return KeyValueStorageCpp::SegmentFileEntry::FromBytes(entry_bytes);
}

KeyValueStorageCpp::SegmentFileEntry ReadEntryFromPos_(std::ifstream& segment_file, int position) {
	segment_file.seekg(position);

	return ReadEntry_(segment_file);
}

std::string KeyValueStorageCpp::KVStorage::BuildKeySpaceSegmentsPath_(const std::string& key_space) {
	return SEGMENTS_PATH_ + key_space + "/";
}

std::string KeyValueStorageCpp::KVStorage::BuildSegmentFileName_(int segment_number) {
	return SEGMENT_FILE_NAME_ + std::to_string(segment_number) + SEGMENT_FILE_EXTENSION;
}

std::string KeyValueStorageCpp::KVStorage::BuildSegmentFilePath_(const std::string& key_space, int segment_number) {
	return BuildKeySpaceSegmentsPath_(key_space) + BuildSegmentFileName_(segment_number);
}

KeyValueStorageCpp::Index KeyValueStorageCpp::KVStorage::RestoreKeySpace_(const std::string& key_space_segments_path, const std::string& key_space) {
	KeyValueStorageCpp::Index index;

	std::ifstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary);

	int position = 0;
	while (segment_file && segment_file.peek() != EOF) {
		SegmentFileEntry entry = ReadEntry_(segment_file);
		if (entry.GetEntryType() == SegmentFileEntry::EntryType::ADD) {
			index.SetKeyPosition(entry.GetKey(), position);
		}
		else if (entry.GetEntryType() == SegmentFileEntry::EntryType::REMOVE) {
			index.RemoveKey(entry.GetKey());
		}

		position = segment_file.tellg();
	}

	segment_file.close();

	return index;
}

void KeyValueStorageCpp::KVStorage::RestoreIndex_() {
	if (std::experimental::filesystem::exists(SEGMENTS_PATH_)) {
		for (const auto & entry : std::experimental::filesystem::directory_iterator(SEGMENTS_PATH_)) {
			index_for_key_space_[entry.path().filename().string()] = RestoreKeySpace_(entry.path().string(), entry.path().filename().string());
		}
	}
}

KeyValueStorageCpp::KVStorage::KVStorage() {
	this->RestoreIndex_();
}

void KeyValueStorageCpp::KVStorage::CreateKeySpace(const std::string& key_space) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	if (index_for_key_space_.find(key_space) != index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " already exists.");
	}

	index_for_key_space_[key_space] = Index();

	const std::string key_space_segments_path = BuildKeySpaceSegmentsPath_(key_space);
	std::experimental::filesystem::create_directories(key_space_segments_path);
	
	std::ofstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary);
	segment_file.close();
}

void KeyValueStorageCpp::KVStorage::DeleteKeySpace(const std::string& key_space) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	if (index_for_key_space_.find(key_space) == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	index_for_key_space_.erase(key_space);

	std::experimental::filesystem::remove_all(BuildKeySpaceSegmentsPath_(key_space));
}

std::vector<std::string> KeyValueStorageCpp::KVStorage::ListAllKeySpaces() {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	std::vector<std::string> key_spaces;

	std::transform(
		index_for_key_space_.begin(),
		index_for_key_space_.end(),
		std::back_inserter(key_spaces),
		[](const std::unordered_map<std::string, Index>::value_type &pair) { return pair.first; });

	return key_spaces;
}

int KeyValueStorageCpp::KVStorage::CountKeys(const std::string& key_space) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);
	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	return index_it->second.CountKeys();
}

bool KeyValueStorageCpp::KVStorage::Exists(const std::string& key_space, const std::vector<char>& key) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	return index_it->second.KeyExists(key);
}

void KeyValueStorageCpp::KVStorage::Set(const std::string& key_space, 
	const std::vector<char>& key, 
	const std::vector<char>& value) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	SegmentFileEntry entry = SegmentFileEntry::NewAddEntry(key, value);
	std::vector<char> entry_bytes = entry.ToBytes();
	int entry_size = entry_bytes.size();

	std::ofstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary | std::fstream::app);
	
	segment_file.write((char*) &entry_size, 4);
	segment_file.write(entry_bytes.data(), entry_bytes.size());
	int position = static_cast<int>(segment_file.tellp()) - entry_size - 4;
	index_it->second.SetKeyPosition(key, position);

	segment_file.close();
}

bool KeyValueStorageCpp::KVStorage::Remove(const std::string& key_space, const std::vector<char>& key) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	SegmentFileEntry entry = SegmentFileEntry::NewRemoveEntry(key);
	std::vector<char> entry_bytes = entry.ToBytes();
	int entry_size = entry_bytes.size();

	if (index_it->second.KeyExists(key)) {
		std::ofstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary | std::fstream::app);

		segment_file.write((char*) &entry_size, 4);
		segment_file.write(entry_bytes.data(), entry_bytes.size());

		index_it->second.RemoveKey(key);

		segment_file.close();

		return true;
	}
	else {
		return false;
	}
}

std::vector<char> KeyValueStorageCpp::KVStorage::Get(const std::string& key_space, const std::vector<char>& key) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	if (index_it->second.KeyExists(key)) {
		std::ifstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary);
		int position = index_it->second.GetKeyPosition(key);
		SegmentFileEntry entry = ReadEntryFromPos_(segment_file, position);
		segment_file.close();
		return entry.GetValue();
	}
	else {
		throw new std::invalid_argument("specified key does not exist.");
	}
}

void KeyValueStorageCpp::KVStorage::RemoveAll(const std::string& key_space) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	index_it->second.RemoveAllKeys();
	const std::string key_space_segments_path = BuildKeySpaceSegmentsPath_(key_space);
	std::experimental::filesystem::remove_all(key_space_segments_path);
	std::experimental::filesystem::create_directories(key_space_segments_path);
}

std::vector<std::pair<std::vector<char>, std::vector<char>>> KeyValueStorageCpp::KVStorage::GetAllKVs(const std::string& key_space) {
	std::lock_guard<std::mutex> lock(storage_mutex_);

	auto index_it = index_for_key_space_.find(key_space);

	if (index_it == index_for_key_space_.end()) {
		throw new std::invalid_argument("key_space " + key_space + " does not exist.");
	}

	std::ifstream segment_file(BuildSegmentFilePath_(key_space, 0), std::ios::binary);

	std::vector<std::pair<std::vector<char>, std::vector<char>>> result;
	for (const auto& key : index_it->second.GetAllKeys()) {
		int position = key.second;
		SegmentFileEntry entry = ReadEntryFromPos_(segment_file, position);
		result.push_back(std::make_pair(key.first, entry.GetValue()));
	}
	
	segment_file.close();

	return result;
}
