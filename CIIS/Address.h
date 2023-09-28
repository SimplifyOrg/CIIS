#pragma once

#include <string>
#include <vector>
#include <memory>

class Address
{
public:
	class Builder {
		friend class Address;
	private:
		std::string m_addressLine1{ "" };
		std::string m_addressLine2{ "" };
		std::string m_addressLine3{ "" };
		std::string m_city{ "" };
		std::string m_state{ "" };
		std::string m_country{ "" };
		std::string m_pin{ "" };

	private:
		Builder(Builder&& other) noexcept {

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

		static bool isValidAddressLine1(std::string& addressLine1) {
			return true;
		}

		static bool isValidAddressLine2(std::string& addressLine2) {
			return true;
		}

		static bool isValidAddressLine3(std::string& addressLine3) {
			return true;
		}

		static bool isValidCity(std::string& city) {
			return true;
		}

		static bool isValidState(std::string& state) {
			return true;
		}

		static bool isValidCountry(std::string& country) {
			return true;
		}

		static bool isValidPIN(std::string& pin) {
			return true;
		}
	public:
		Builder() {

		}

		Builder(const Builder& other) {

		}

		Builder* setAddressLine1(std::string addressLine1) {
			this->m_addressLine1 = addressLine1;
			return this;
		}

		Builder* setAddressLine2(std::string addressLine2) {
			this->m_addressLine2 = addressLine2;
			return this;
		}

		Builder* setAddressLine3(std::string addressLine3) {
			this->m_addressLine3 = addressLine3;
			return this;
		}

		Builder* setCity(std::string city) {
			this->m_city = city;
			return this;
		}

		Builder* setState(std::string state) {
			this->m_state = state;
			return this;
		}

		Builder* setCountry(std::string country) {
			this->m_country = country;
			return this;
		}

		Builder* setPIN(std::string pin) {
			this->m_pin = pin;
			return this;
		}

		std::shared_ptr<Address> build() {
			if (isValidAddressLine1(this->m_addressLine1) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidAddressLine2(this->m_addressLine2) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}
			if (isValidAddressLine3(this->m_addressLine3) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}

			// 3 more such validations
			return std::make_shared<Address>(this);
		}
	};


private:
	std::string m_addressLine1{ "" };
	std::string m_addressLine2{ "" };
	std::string m_addressLine3{ "" };
	std::string m_city{ "" };
	std::string m_state{ "" };
	std::string m_country{ "" };
	std::string m_pin{ "" };
	std::shared_ptr<Builder> m_builder;

private:

	Address() {

	}

	Address& operator=(Address&& other) noexcept // move assignment
	{
		//std::swap(cstring, other.cstring);
		return *this;
	}

public:
	Address(Builder* builder) {
		m_builder.reset(builder);
		this->m_addressLine1 = builder->m_addressLine1;
		this->m_addressLine2 = builder->m_addressLine2;
		this->m_addressLine3 = builder->m_addressLine3;
		this->m_city = builder->m_city;
		this->m_state = builder->m_state;
		this->m_country = builder->m_country;
		this->m_pin = builder->m_country;
	}

	Address(const Address& other) {
		this->m_addressLine1 = other.m_addressLine1;
		this->m_addressLine2 = other.m_addressLine2;
		this->m_addressLine3 = other.m_addressLine3;
		this->m_city = other.m_city;
		this->m_state = other.m_state;
		this->m_country = other.m_country;
		this->m_pin = other.m_country;
	}

	Address(Address&&) noexcept {

	}

	Address& operator=(const Address& other) // copy assignment
	{
		this->m_addressLine1 = other.m_addressLine1;
		this->m_addressLine2 = other.m_addressLine2;
		this->m_addressLine3 = other.m_addressLine3;
		this->m_city = other.m_city;
		this->m_state = other.m_state;
		this->m_country = other.m_country;
		this->m_pin = other.m_country;

		return *this;//Address(other);
	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<Address::Builder>();
	}

	std::string getAddressLine1() {
		return m_builder->m_addressLine1;
	}

	std::string getAddressLine2() {
		return m_builder->m_addressLine2;
	}

	std::string getAddressLine3() {
		return m_builder->m_addressLine3;
	}

	std::string getCity() {
		return m_builder->m_city;
	}

	std::string getState() {
		return m_builder->m_state;
	}

	std::string getCountry() {
		return m_builder->m_country;
	}

	std::string getPIN() {
		return m_builder->m_pin;
	}

	void setAddressLine1(std::string addressLine1) {
		if (Builder::isValidAddressLine1(addressLine1) == true) {
			m_builder->m_addressLine1 = addressLine1;
		}
	}

	void setAddressLine2(std::string addressLine2) {
		if (Builder::isValidAddressLine2(addressLine2) == true) {
			m_builder->m_addressLine2 = addressLine2;
		}
	}

	void setAddressLine3(std::string addressLine3) {
		if (Builder::isValidAddressLine3(addressLine3) == true) {
			m_builder->m_addressLine3 = addressLine3;
		}
	}

	void setCity(std::string city) {
		if (Builder::isValidCity(city) == true) {
			m_builder->m_city = city;
		}
	}

	void setState(std::string state) {
		if (Builder::isValidState(state) == true) {
			m_builder->m_state = state;
		}
	}

	void setCountry(std::string country) {
		if (Builder::isValidCountry(country) == true) {
			m_builder->m_country = country;
		}
	}

	void setPIN(std::string pin) {
		if (Builder::isValidPIN(pin) == true) {
			m_builder->m_pin = pin;
		}
	}
};

