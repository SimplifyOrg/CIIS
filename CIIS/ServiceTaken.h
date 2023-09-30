#pragma once

#include <string>
#include <vector>
#include <memory>

class ServiceTaken
{
public:
	class Builder {
		friend class ServiceTaken;
	private:
		std::string m_serviceType{ "" };
		std::string m_billAmount{ "" };
		std::string m_details{ "" };
		std::string m_date{ "" };

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

		static bool isValidServiceType(std::string& serviceType) {
			return true;
		}

		static bool isValidBillAmount(std::string& billAmount) {
			return true;
		}

		static bool isValidDetails(std::string& details) {
			return true;
		}

		static bool isValidDate(std::string& date) {
			return true;
		}

	public:
		Builder() {

		}

		Builder(const Builder& other) {

		}

		Builder* setServiceType(std::string serviceType) {
			this->m_serviceType = serviceType;
			return this;
		}

		Builder* setBillAmount(std::string billAmount) {
			this->m_billAmount = billAmount;
			return this;
		}

		Builder* setDetails(std::string details) {
			this->m_details = details;
			return this;
		}

		Builder* setDate(std::string date) {
			this->m_date = date;
			return this;
		}

		std::shared_ptr<ServiceTaken> build() {
			if (isValidServiceType(this->m_serviceType) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidBillAmount(this->m_billAmount) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}
			if (isValidDate(this->m_date) == false) {
				throw new std::exception("name should be not null, not blank and <= 100 chars long");//IllegalArgumentException("name should be not null, not blank and <= 100 chars long");
			}
			if (isValidDetails(this->m_details) == false) {
				throw new std::exception("age should be >= 20 and <= 50");
			}

			// 3 more such validations
			return std::make_shared<ServiceTaken>(this);
		}
	};


private:
	std::string m_serviceType{ "" };
	std::string m_billAmount{ "" };
	std::string m_details{ "" };
	std::string m_date{ "" };
	std::shared_ptr<Builder> m_builder;

private:

	ServiceTaken() {

	}

	ServiceTaken& operator=(ServiceTaken&& other) noexcept // move assignment
	{
		//std::swap(cstring, other.cstring);
		return *this;
	}

public:
	ServiceTaken(Builder* builder) {
		m_builder.reset(builder);
		this->m_serviceType = builder->m_serviceType;
		this->m_billAmount = builder->m_billAmount;
		this->m_details = builder->m_details;
		this->m_date = builder->m_date;
	}

	ServiceTaken(const ServiceTaken& other) {
		this->m_serviceType = other.m_serviceType;
		this->m_billAmount = other.m_billAmount;
		this->m_details = other.m_details;
		this->m_date = other.m_date;
	}

	ServiceTaken(ServiceTaken&&) noexcept {

	}

	ServiceTaken& operator=(const ServiceTaken& other) // copy assignment
	{
		this->m_serviceType = other.m_serviceType;
		this->m_billAmount = other.m_billAmount;
		this->m_details = other.m_details;
		this->m_date = other.m_date;

		return *this;//ServiceTaken(other);
	}

	static std::shared_ptr<Builder> getBuilder() {
		return std::make_shared<ServiceTaken::Builder>();
	}

	std::string getServiceType() {
		return m_builder->m_serviceType;
	}

	std::string getBillAmount() {
		return m_builder->m_billAmount;
	}

	std::string getDetails() {
		return m_builder->m_details;
	}

	std::string getDate() {
		return m_builder->m_date;
	}

	void setServiceType(std::string serviceType) {
		if (Builder::isValidServiceType(serviceType) == true) {
			m_builder->m_serviceType = serviceType;
		}
	}

	void setBillAmount(std::string billAmount) {
		if (Builder::isValidBillAmount(billAmount) == true) {
			m_builder->m_billAmount = billAmount;
		}
	}

	void setDetails(std::string details) {
		if (Builder::isValidDetails(details) == true) {
			m_builder->m_details = details;
		}
	}

	void setDate(std::string date) {
		if (Builder::isValidDate(date) == true) {
			m_builder->m_date = date;
		}
	}
};

