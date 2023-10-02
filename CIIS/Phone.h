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
		bool m_isPrimary{ false };

	private:
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

		static bool isValidPrimary() {
			return true;
		}

	public:
		Builder() {

		}

		Builder(const Builder& other) {
			this->m_phoneNumber = other.m_phoneNumber;
			this->m_countryCode = other.m_countryCode;
			this->m_isPrimary = other.m_isPrimary;
		}

		Builder(Builder&& other) noexcept {
			std::swap(this->m_phoneNumber, other.m_phoneNumber);
			std::swap(this->m_countryCode, other.m_countryCode);
			std::swap(this->m_isPrimary, other.m_isPrimary);
		}


		Builder* setPhoneNumber(std::string phoneNumber) {
			this->m_phoneNumber = phoneNumber;
			return this;
		}

		Builder* setCountryCode(std::string countryCode) {
			this->m_countryCode = countryCode;
			return this;
		}

		Builder* setPrimary(bool isPrimary) {
			this->m_isPrimary = isPrimary;
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
	bool m_isPrimary{ false };
	std::shared_ptr<Builder> m_builder;

private:

	Phone() {

	}

public:
	Phone(Builder* builder) {
		m_builder = std::make_shared<Phone::Builder>(*builder);
		this->m_phoneNumber = builder->m_phoneNumber;
		this->m_countryCode = builder->m_countryCode;
		this->m_isPrimary = builder->m_isPrimary;
	}

	Phone(const Phone& other) {
		this->m_phoneNumber = other.m_phoneNumber;
		this->m_countryCode = other.m_countryCode;
		this->m_isPrimary = other.m_isPrimary;
	}

	Phone(Phone&& other) noexcept {
		std::swap(this->m_phoneNumber, other.m_phoneNumber);
		std::swap(this->m_countryCode, other.m_countryCode);
		std::swap(this->m_isPrimary, other.m_isPrimary);
	}

	Phone& operator=(const Phone& other) // copy assignment
	{
		this->m_phoneNumber = other.m_phoneNumber;
		this->m_countryCode = other.m_countryCode;
		this->m_isPrimary = other.m_isPrimary;

		return *this;//Phone(other);
	}

	Phone& operator=(Phone&& other) noexcept // move assignment
	{
		std::swap(this->m_phoneNumber, other.m_phoneNumber);
		std::swap(this->m_countryCode, other.m_countryCode);
		std::swap(this->m_isPrimary, other.m_isPrimary);
		return *this;
	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<Phone::Builder>();
	}

	std::string getPhoneNumber() {
		return m_phoneNumber;
	}

	std::string getCountryCode() {
		return m_countryCode;
	}

	bool getIsPrimary() {
		return m_isPrimary;
	}

	void setPhoneNumber(std::string phoneNumber) {
		if (Builder::isValidPhoneNumber(phoneNumber) == true) {
			m_builder->m_phoneNumber = phoneNumber;
			m_phoneNumber = phoneNumber;
		}
	}

	void setCountryCode(std::string countryCode) {
		if (Builder::isValidCountryCode(countryCode) == true) {
			m_builder->m_countryCode = countryCode;
			m_countryCode = countryCode;
		}
	}

	void setPrimary(bool isPrimary) {
		if (Builder::isValidPrimary() == true) {
			m_builder->m_isPrimary = isPrimary;
			m_isPrimary = isPrimary;
		}
	}
};

