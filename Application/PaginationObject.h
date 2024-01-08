#pragma once
#include <iostream>
#include <vector>
#include <string>
#include <memory>
#include <boost/describe.hpp>
template <class T>
class PaginationObject
{
	BOOST_DESCRIBE_CLASS(PaginationObject, (), (), (), (m_data, m_pageSize, m_currentPage, m_totalPages))
public:
	PaginationObject() = default;
	//explicit PaginationObject(std::vector<std::shared_ptr<T>> data, int pageSize);
	//explicit PaginationObject(std::vector<std::shared_ptr<T>> data, int pageSize, int currentPage);
	explicit PaginationObject(std::vector<std::shared_ptr<T>> data, int pageSize, int currentPage, int totalPages)
	{
		m_data = data;
		m_pageSize = pageSize;
		m_currentPage = currentPage;
		m_totalPages = totalPages;
	}
	std::vector<std::shared_ptr<T>> getData()
	{
		return m_data;
	}
	void setData(std::vector<std::shared_ptr<T>> data)
	{
		m_data = data;
	}
	int getPageSize()
	{
		return m_pageSize;
	}
	void setPageSize(int pageSize)
	{
		m_pageSize = pageSize;
	}
	~PaginationObject() = default;
private:
	std::vector<std::shared_ptr<T>> m_data;
	int m_pageSize;
	int m_currentPage;
	int m_totalPages;
};

