#pragma once

#include <string>
#include <vector>
#include <memory>
#include "Address.h"
#include "Phone.h"

class Customer
{
public:
	class Builder {
		friend class Customer;
	private:
		std::string m_customerIdentificationNumber{ "" };
		std::string m_name{ "" };
		std::vector<Address> m_addresses;
		std::vector<Phone> m_phones;


	private:
		Builder(Builder&& other) noexcept{

		}

		Builder& operator=(const Builder& other) // copy assignment
		{
			return *this = Builder(other);
		}

		Builder& operator=(Builder&& other) noexcept // move assignment
		{
			//std::swap(cstring, other.cstring);
			return *this;
		}

		static bool isValidCustomerIdentification(std::string & customerIdentity) {
			return true;
		}

		static bool isValidName(std::string & name) {
			if (name.size() < 1) {
				throw std::exception("Name is too samll");
			}
			if (name.size() > 100) {
				throw std::exception("Name is too big");
			}
			return true;
		}

		static bool isValidAddress(Address & addresses) {
			return true;
		}

		static bool isValidPhone(Phone & phones) {
			return true;
		}
	public:
		Builder() {

		}

		~Builder() {

		}

		Builder(const Builder& other) {
			this->m_customerIdentificationNumber = other.m_customerIdentificationNumber;
			this->m_name = other.m_name;
			this->m_addresses = other.m_addresses;
			this->m_phones = other.m_phones;
		}

		Builder* setCustomerIdentification(std::string customerIdentity) {
			this->m_customerIdentificationNumber = customerIdentity;
			return this;
		}

		Builder* setName(std::string name) {
			this->m_name = name;
			return this;
		}

		Builder* setAddress(Address& address) {
			this->m_addresses.push_back(address);
			return this;
		}

		Builder* setPhoneNumber(Phone& phone) {
			this->m_phones.push_back(phone);
			return this;
		}

		Customer * build() {
			if (isValidName(this->m_name) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidCustomerIdentification(this->m_customerIdentificationNumber) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}

			// 3 more such validations
			return new Customer(this);
		}
	};


private:
	std::string m_customerIdentificationNumber{ "" };
	std::string m_name{ "" };
	std::vector<Address> m_addresses;
	std::vector<Phone> m_phones;
	std::shared_ptr<Builder> m_builder;

private:

	Customer() {

	}

public:
	Customer(Customer::Builder* builder) {
		m_builder = std::make_shared<Customer::Builder>(*builder);
		this->m_customerIdentificationNumber = builder->m_customerIdentificationNumber;
		this->m_name = builder->m_name;
		this->m_addresses = builder->m_addresses;
		this->m_phones = builder->m_phones;
	}

	Customer& operator=(const Customer& other) // copy assignment
	{
		this->m_customerIdentificationNumber = other.m_customerIdentificationNumber;
		this->m_name = other.m_name;

		return *this;//Customer(other);
	}

	Customer& operator=(Customer&& other) noexcept // move assignment
	{
		//std::swap(cstring, other.cstring);
		std::swap(this->m_customerIdentificationNumber, other.m_customerIdentificationNumber);
		std::swap(this->m_name, other.m_name);
		return *this;
	}

	Customer(Customer&& other) noexcept {
		std::swap(this->m_customerIdentificationNumber, other.m_customerIdentificationNumber);
		std::swap(this->m_name, other.m_name);
	}

	Customer(const Customer& other) {
		this->m_customerIdentificationNumber = other.m_customerIdentificationNumber;
		this->m_name = other.m_name;
	}

	~Customer() {

	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<Customer::Builder>();
	}

	std::string getCustomerIdentification() {
		return m_customerIdentificationNumber;
	}

	std::string getName() {
		return m_name;
	}

	std::vector<Address> getAddresses() {
		return m_addresses;
	}

	std::vector<Phone> getPhoneNumbers() {
		return m_phones;
	}

	void setCustomerIdentification(std::string customerIdentity) {
		if (Builder::isValidCustomerIdentification(customerIdentity) == true) {
			m_builder->m_customerIdentificationNumber = customerIdentity;
			m_customerIdentificationNumber = customerIdentity;
		}
	}

	void setName(std::string name) {
		if (Builder::isValidName(name) == true) {
			m_builder->m_name = name;
			m_name = name;
		}
	}

	void setAddress(Address & address) {
		if (Builder::isValidAddress(address) == true) {
			m_builder->m_addresses.push_back(address);
			m_addresses.push_back(address);
		}
	}

	void setPhoneNumber(Phone & phone) {
		if (Builder::isValidPhone(phone) == true) {
			m_builder->m_phones.push_back(phone);
			m_phones.push_back(phone);
		}
	}
};

