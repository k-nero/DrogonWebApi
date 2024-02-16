#pragma once
#define REGTYPE(type) std::string(typeid(type).name()).substr(std::string(typeid(type).name()).find(" ") + 1)
#define REGMIDDLEWARE(TYPE) REGTYPE(TYPE)