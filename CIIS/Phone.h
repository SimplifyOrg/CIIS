#pragma once

#include <string>
#include <vector>
#include <memory>

class Phone
{
public:
	class Builder {
		friend class Phone;
	private:
		std::string m_phoneNumber{ "" };
		std::string m_countryCode{ "" };

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

		static bool isValidPhoneNumber(std::string& phoneNumber) {
			return true;
		}

		static bool isValidCountryCode(std::string& countryCode) {
			return true;
		}

	public:
		Builder() {

		}

		Builder(const Builder& other) {

		}

		Builder* setPhoneNumber(std::string phoneNumber) {
			this->m_phoneNumber = phoneNumber;
			return this;
		}

		Builder* setCountryCode(std::string countryCode) {
			this->m_countryCode = countryCode;
			return this;
		}

		std::shared_ptr<Phone> build() {
			if (isValidPhoneNumber(this->m_phoneNumber) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidCountryCode(this->m_countryCode) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}

			// 3 more such validations
			return std::make_shared<Phone>(this);
		}
	};


private:
	std::string m_phoneNumber{ "" };
	std::string m_countryCode{ "" };
	std::shared_ptr<Builder> m_builder;

private:

	Phone() {

	}	

	Phone& operator=(Phone&& other) noexcept // move assignment
	{
		//std::swap(cstring, other.cstring);
		return *this;
	}

public:
	Phone(Builder* builder) {
		m_builder.reset(builder);
		this->m_phoneNumber = builder->m_phoneNumber;
		this->m_countryCode = builder->m_countryCode;
	}

	Phone(const Phone& other) {
		this->m_phoneNumber = other.m_phoneNumber;
		this->m_countryCode = other.m_countryCode;
	}

	Phone(Phone&&) noexcept {

	}

	Phone& operator=(const Phone& other) // copy assignment
	{
		this->m_phoneNumber = other.m_phoneNumber;
		this->m_countryCode = other.m_countryCode;

		return *this;//Phone(other);
	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<Phone::Builder>();
	}

	std::string getPhoneNumber() {
		return m_builder->m_phoneNumber;
	}

	std::string getCountryCode() {
		return m_builder->m_countryCode;
	}

	void setPhoneNumber(std::string phoneNumber) {
		if (Builder::isValidPhoneNumber(phoneNumber) == true) {
			m_builder->m_phoneNumber = phoneNumber;
		}
	}

	void setCountryCode(std::string countryCode) {
		if (Builder::isValidCountryCode(countryCode) == true) {
			m_builder->m_countryCode = countryCode;
		}
	}
};

