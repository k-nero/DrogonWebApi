#pragma once

#ifdef APPLICATION_EXPORTS
#define APPLICATION_API __declspec(dllexport)
#else
#define APPLICATION_API __declspec(dllimport)
#endif

#ifndef PKEY
#define PKEY "Id"
#endif // !PKEY

#define ASYNC

#define EQ(x) _EQ(x, x)
#define NEQ(x) _NEQ(x, x)
#define GT(x) _GT(x, x)
#define LT(x) _LT(x, x)
#define GTE(x) _GTE(x, x)
#define LTE(x) _LTE(x, x)
#define LIKE(x) _LIKE(x, x)
#define NOT_LIKE(x) _NOT_LIKE(x, x)
#define _IN(x) _IN_(x, x)
#define NOT_IN(x) _NOT_IN(x, x)
#define IS_NULL(x) _IS_NULL(x)

#define IS_NOT_NULL(x) #x " IS NOT NULL"
#define AND(x, y) x _AND y
#define OR(x, y) x _OR y
#define _AND " AND "
#define _OR " OR "
#define INCLUDE(x) #x

#define _EQ(x, y) #x " = '" + std::format("{}", y) + "'"
#define _NEQ(x, y) #x " != '" + std::format("{}", y) + "'"
#define _GT(x, y) #x " > '" + std::format("{}", y) + "'"
#define _LT(x, y) #x " < '" + std::format("{}", y) + "'"
#define _GTE(x, y) #x " >= '" + std::format("{}", y) + "'"
#define _LTE(x, y) #x " <= '" + std::format("{}", y) + "'"
#define _LIKE(x, y) #x " LIKE '%" + std::format("{}", y) + "%'"
#define _NOT_LIKE(x, y) #x " NOT LIKE '%" + std::format("{}", y) + "%'"
#define _IN_(x, y) #x " IN (" + y + ")"
#define _NOT_IN(x, y) #x " NOT IN (" + y + ")"

#define SELECT(x) "SELECT " + std::format("{}", x)
#define FROM(x) " FROM " + std::format("{}", x)

#define LOG_SQL_COMMAND