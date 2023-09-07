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

		bool isValidCustomerIdentification() {

		}

		bool isValidName() {

		}

		bool isValidAddress() {

		}

		bool isValidPhone() {

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
	std::shared_ptr<Builder> getBuilder() {

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

	void setAddresses(std::vector<std::string>) {

	}

	void setPhoneNumbers(std::vector<std::string>) {

	}
};

