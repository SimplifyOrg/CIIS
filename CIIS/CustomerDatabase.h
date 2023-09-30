#pragma once
#include "Customer.h"
#include "PostgresDatabase.h"
#include <map>

static std::map<std::string, std::shared_ptr<Customer>> customerMap;


class CustomerDatabase {
public:
	static Customer* GetCustomerWithID(const std::string& id) {
		if (customerMap.find(id) != customerMap.end()) {
			/*Customer* cus = Customer::getBuilder()
				->setCustomerIdentification(customerMap[id].getCustomerIdentification())
				->setName(customerMap[id].getName())
				->build();*/
			return customerMap[id].get();
		}
		std::unique_ptr<PostgresDatabase> pDB = std::make_unique<PostgresDatabase>();
		s_dbConfig config {"postgres", "127.0.0.1", "kislay", "customerdb", "5432"};
		pDB->connect(config);
		std::ostringstream s;
		s << "SELECT * FROM customer_schema.customers WHERE cid = '" << id << "';";
		std::string query = s.str();
		pqxx::result res = pDB->executeQuery(query);
		Customer* newCustomer = Customer::getBuilder()
			->setCustomerIdentification(res[0][0].c_str())
			->setName(res[0][1].c_str())
			->build();
		customerMap[id] = std::make_shared<Customer>(*newCustomer);

		return customerMap[id].get();
	}

	static void InsertNewCustomer(const std::shared_ptr<Customer> customer) {
		std::unique_ptr<PostgresDatabase> pDB = std::make_unique<PostgresDatabase>();
		s_dbConfig config{ "postgres", "localhost", "kislay", "customerdb", "5432" };
		pDB->connect(config);

		std::ostringstream s;
		s << "INSERT INTO customer_schema.customers (cid, name) " 
			<< "VALUES ('" << customer->getCustomerIdentification() 
			<<"','" << customer->getName() << "');";
		std::string command = s.str();

		pDB->executeCommand(command);
	}

	static void UpdateCustomer(const std::shared_ptr<Customer> customer) {

	}

	static std::string CreateCustomerIdentity(std::string trainingPath) {
		
		// Create new id number
		int index = 0;
		for (const auto& entry : std::filesystem::directory_iterator(trainingPath)) {
			if (entry.is_directory()) {
				++index;
			}
		}
		auto t = std::time(nullptr);
		auto tm = *std::localtime(&t);

		std::ostringstream oss;
		oss << std::put_time(&tm, "%d-%m-%Y_%H-%M-%S");
		auto str = oss.str();
		return std::to_string(index) + "_" + str;

	}

	static bool CheckCustomerDatabase() {
		return CheckCustomerTable();
	}

private:
	static bool CheckCustomerTable() {
		std::unique_ptr<PostgresDatabase> pDB = std::make_unique<PostgresDatabase>();
		s_dbConfig config{ "postgres", "localhost", "kislay", "customerdb", "5432" };
		int ret = pDB->connect(config);
		if (ret != 0) {
			return false;
		}

		std::ostringstream s;
		s << "SELECT column_name FROM information_schema.columns WHERE table_name = 'customers';";
		std::string query = s.str();
		pqxx::result res = pDB->executeQuery(query);
		if (res.size() != 2) {
			return false;
		}
		std::string cid(res[0][0].c_str());
		std::string name(res[1][0].c_str());
		if (cid == "cid" && name == "name") {
			return true;
		}
		//else {
		//	// Create table if it does not exist
		//	s.clear();
		//	s << "CREATE TABLE IF NOT EXISTS customers"
		//		<< "( cid \"char\"[] NOT NULL, name \"char\"[], CONSTRAINT customers_pkey PRIMARY KEY (cid) )";
		//	std::string command = s.str();

		//	pDB->executeCommand(command);
		//	return true;
		//}

		return false;

	}
};
