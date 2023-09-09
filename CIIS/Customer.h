#pragma once

#include <string>
#include <vector>

class Customer
{
private:
	std::string m_customerIdentificationNumber{ "" };
	std::string m_name{ "" };
	std::vector<std::string> m_addresses;
	std::vector<std::string> m_phones;

	class Builder {
	private:
		friend Customer;
		std::string m_customerIdentificationNumber{ "" };
		std::string m_name{ "" };
		std::vector<std::string> m_addresses;
		std::vector<std::string> m_phones;

	private:
		Builder() {

		}

		std::string getCustomerIdentification() {

		}

		std::string getName() {

		}

		std::vector<std::string> getAddresses() {

		}

		std::vector<std::string> getPhoneNumbers() {

		}

		static bool isValidCustomerIdentification(std::string & customerIdentity) {

		}

		static bool isValidName(std::string & name) {

		}

		static bool isValidAddress(std::vector<std::string> & addresses) {

		}

		static bool isValidPhone(std::vector<std::string> & phones) {

		}
	public:
		std::shared_ptr<Builder> setCustomerIdentification(std::string customerIdentity) {
			this->m_customerIdentificationNumber = customerIdentity;
			return std::make_shared<Builder>(this);
		}

		std::shared_ptr<Builder> setName(std::string name) {
			this->m_name = name;
			return std::make_shared<Builder>(this);
		}

		std::shared_ptr<Builder> setAddress(std::string address) {
			this->m_addresses.push_back(address);
			return std::make_shared<Builder>(this);
		}

		std::shared_ptr<Builder> setPhoneNumber(std::string phone) {
			this->m_phones.push_back(phone);
			return std::make_shared<Builder>(this);
		}

		std::shared_ptr<Customer> build() {
			if (isValidName(this->m_name) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidCustomerIdentification(this->m_customerIdentificationNumber) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}

			// 3 more such validations
			return std::make_shared<Customer>(this);
		}
	};

private:
	Customer(std::shared_ptr<Builder> builder) {
		this->m_customerIdentificationNumber = builder->getCustomerIdentification();
		this->m_name = builder->getName();
		this->m_addresses = builder->getAddresses();
		this->m_phones = builder->getPhoneNumbers();
	}

public:
	static std::shared_ptr<Builder> getBuilder() {

	}

	std::string getCustomerIdentification() {

	}

	std::string getName() {

	}

	std::vector<std::string> getAddresses() {

	}

	std::vector<std::string> getPhoneNumbers() {

	}

	void setCustomerIdentification(std::string) {

	}

	void setName(std::string) {

	}

	void setAddress(std::string) {

	}

	void setPhoneNumbers(std::string phone) {

	}
};

