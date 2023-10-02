#pragma once
#include "Phone.h"
#include "PostgresDatabase.h"
#include <map>

static std::map<std::string, std::vector<std::shared_ptr<Phone>>> phoneMap;

class PhoneDatabase {
public:
	static std::vector<std::shared_ptr<Phone>> GetPhoneWithCustomerID(const std::string& id) {
		if (phoneMap.find(id) != phoneMap.end()) {
			return phoneMap[id];
		}
		std::unique_ptr<PostgresDatabase> pDB = std::make_unique<PostgresDatabase>();
		s_dbConfig config{ "postgres", "127.0.0.1", "kislay", "customerdb", "5432" };
		pDB->connect(config);
		std::ostringstream s;
		s << "SELECT * FROM customer_schema.phone WHERE cid = '" << id << "';";
		std::string query = s.str();
		pqxx::result res = pDB->executeQuery(query);
		for (auto row : res) {
			phoneMap[id].push_back(Phone::getBuilder()
									->setCountryCode(res[0][0].c_str())
									->setPhoneNumber(res[0][1].c_str())
									->build());
		}
		return phoneMap[id];
	}

	static void InsertNewPhone(const std::shared_ptr<Phone> phone) {
		std::unique_ptr<PostgresDatabase> pDB = std::make_unique<PostgresDatabase>();
		s_dbConfig config{ "postgres", "localhost", "kislay", "customerdb", "5432" };
		pDB->connect(config);

		std::ostringstream s;
		s << "INSERT INTO customer_schema.phone (countrycode, phonenumber, cid, isprimary) "
			<< "VALUES ('" << phone->getCountryCode()
			<< "','" << phone->getPhoneNumber() 
			<< "','" << phone->getIsPrimary() << "');";
		std::string command = s.str();

		pDB->executeCommand(command);
	}
};

