#pragma once

#include <string>
#include <vector>
#include <memory>

class Customer
{
public:
	class Builder {
		friend class Customer;
	private:
		std::string m_customerIdentificationNumber{ "" };
		std::string m_name{ "" };
		std::vector<std::string> m_addresses;
		std::vector<std::string> m_phones;


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

		static bool isValidAddress(std::vector<std::string> & addresses) {
			return true;
		}

		static bool isValidPhone(std::vector<std::string> & phones) {
			return true;
		}
	public:
		Builder() {

		}

		Builder(const Builder& other) {

		}

		Builder* setCustomerIdentification(std::string customerIdentity) {
			this->m_customerIdentificationNumber = customerIdentity;
			return this;
		}

		Builder* setName(std::string name) {
			this->m_name = name;
			return this;
		}

		Builder* setAddress(std::string address) {
			this->m_addresses.push_back(address);
			return this;
		}

		Builder* setPhoneNumber(std::string phone) {
			this->m_phones.push_back(phone);
			return this;
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
	std::string m_customerIdentificationNumber{ "" };
	std::string m_name{ "" };
	std::vector<std::string> m_addresses;
	std::vector<std::string> m_phones;
	std::shared_ptr<Builder> m_builder;

private:

	Customer() {

	}

	Customer(Customer&&) noexcept{

	}

	Customer(const Customer&) {

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
		return *this;
	}

public:
	friend class Builder;
	Customer(Builder* builder) {
		m_builder.reset(builder);
		this->m_customerIdentificationNumber = builder->m_customerIdentificationNumber;
		this->m_name = builder->m_name;
		this->m_addresses = builder->m_addresses;
		this->m_phones = builder->m_phones;
	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<Customer::Builder>();
	}

	std::string getCustomerIdentification() {
		return m_builder->m_customerIdentificationNumber;
	}

	std::string getName() {
		return m_builder->m_name;
	}

	std::vector<std::string> getAddresses() {
		return m_builder->m_addresses;
	}

	std::vector<std::string> getPhoneNumbers() {
		return m_builder->m_phones;
	}

	void setCustomerIdentification(std::string customerIdentity) {
		if (Builder::isValidCustomerIdentification(customerIdentity) == true) {
			m_builder->m_customerIdentificationNumber = customerIdentity;
		}
	}

	void setName(std::string) {

	}

	void setAddress(std::string) {

	}

	void setPhoneNumbers(std::string phone) {

	}
};

