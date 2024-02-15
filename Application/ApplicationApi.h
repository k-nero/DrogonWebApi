#pragma once

#ifdef APPLICATION_EXPORTS
#define APPLICATION_API __declspec(dllexport)
#else
#define APPLICATION_API __declspec(dllimport)
#endif

#define EQ(x) #x " = '" + x + "'"
#define NEQ(x) #x " != '" + x + "'"
#define GT(x) #x " > '" + x + "'"
#define LT(x) #x " < '" + x + "'"
#define GTE(x) #x " >= '" + x + "'"
#define LTE(x) #x " <= '" + x + "'"
#define LIKE(x) #x " LIKE '%" + x + "%'"
#define NOT_LIKE(x) #x " NOT LIKE '%" + x + "%'"
#define IN(x) #x " IN (" + x + ")"
#define NOT_IN(x) #x " NOT IN (" + x + ")"
#define IS_NULL(x) #x " IS NULL"
#define IS_NOT_NULL(x) #x " IS NOT NULL"
#define AND(x, y) x _AND y
#define OR(x, y) x _OR y
#define _AND " AND "
#define _OR " OR "