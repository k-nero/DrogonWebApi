interface PaginatedType<T>
{
    m_data: T[];
    m_pageSize: number;
    m_currentPage: number;
    m_totalPages: number;
}

export default PaginatedType;
