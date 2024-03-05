#pragma once
#include <string>
#include <vector>
#include <map>

//namespace SwaggerConfig
//{
//	struct Swagger
//	{
//		std::string swagger = "2.0";
//		std::string host = "127.0.0.1";
//		std::string basePath = "/";
//		std::vector<std::string> schemes = { "http", "https"};
//		Info info;
//		std::vector<Tags> tags;
//		
//	};
//
//	struct Info
//	{
//		std::string description = "Swagger API";
//		std::string version = "1.0.0";
//		std::string title = "Swagger API";
//		std::string termsOfService = "http://swagger.io/terms/";
//		Contact contact;
//		License license;
//	};
//
//	struct Contact
//	{
//		std::string email = " ";
//	};
//
//	struct License
//	{
//		std::string name = "MIT";
//		std::string url = "";
//	};
//
//	struct Tags
//	{
//		std::string name = " ";
//		std::string description = " ";
//		ExternalDocs externalDocs;
//	};
//
//	struct ExternalDocs
//	{
//		std::string description = "";
//		std::string url = "";
//	};
//
//
//	struct Method
//	{
//		std::vector<Tags> tags;
//		std::string summary = "";
//		std::string description = "";
//		std::string operationId = "";
//		std::vector<std::string> comsumes = { "application/json", "application/xml" };
//		std::vector<std::string> produces = { "application/json", "application/xml" };
//		std::vector<Parameter> parameters;
//
//	};
//
//	struct Parameter
//	{
//		std::string name = "";
//		std::string in = "";
//		std::string description = "";
//		bool required = false;
//		std::string type = "";
//	};
//
//	struct Responses
//	{
//		std::string description = "";
//		std::map<std::string, Response> responses;
//	};
//
//	struct Response
//	{
//		std::string description = "";
//	};
//
//
//};
//
//
//
