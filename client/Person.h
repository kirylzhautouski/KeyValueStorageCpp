#pragma once

#include <string>
#include <iostream>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <boost/serialization/string.hpp>

using std::cout;
using std::endl;

namespace KeyValueStorageCpp {
	class Person {
	public:
		Person() {};
		Person(std::string name, int age) {
			this->name_ = name;
			this->age_ = age;
		}

		void print() {
			std::cout << "Name: " << this->name_ << std::endl;
			std::cout << "Age: " << this->age_ << std::endl;
		}

		template<class Archive>
		void serialize(Archive& ar, const unsigned int version){
			ar& this->name_;
			ar& this->age_;
		}

	private:
		std::string name_;
		int age_;
	};

}

