#if defined _DEBUG
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include "ConfigProvider.h"
#include <boost/log/trivial.hpp>
#include <drogon/drogon.h>
#include "CoreHelper.h"

constexpr auto ip = "0.0.0.0";

int main()
{
    int cpus = 8;
#if defined _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    std::cout << drogon::banner;
    BOOST_LOG_TRIVIAL(info) << "Drogon server starting...";
    ConfigProvider::GetInstance()->Initialize();

    //Set HTTP listener address and port

    drogon::app().addListener(ip, 443, true, "certificate.crt", "certificate.key");
    drogon::app().setLogPath("");
#if defined _DEBUG
    drogon::app().setLogLevel( trantor::Logger::kTrace );
    drogon::app().setThreadNum(cpus);
#else
    cpus = std::thread::hardware_concurrency();
    drogon::app().setLogLevel( trantor::Logger::kInfo );
    drogon::app().setThreadNum(0);
#endif   
    drogon::app().enableSession(std::chrono::days(30), drogon::Cookie::SameSite::kNull, "SESSIONID");
    drogon::app().enableGzip(true);
    drogon::app().enableBrotli(false);
    drogon::app().enableDateHeader(true);
    drogon::app().enableServerHeader(true);
    drogon::app().setCustom404Page(drogon::HttpResponse::newFileResponse("public/404.html", "", drogon::CT_TEXT_HTML));

    drogon::app().registerPreRoutingAdvice([](const drogon::HttpRequestPtr& req, drogon::FilterCallback&& stop, drogon::FilterChainCallback&& pass)
    {
        // Let anything not starting with /api or not a preflight request through
        if (!req->path().starts_with("/api") || req->method() != drogon::Options)
        {
            pass();
            return;
        }

        auto resp = drogon::HttpResponse::newHttpResponse();
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, PATCH");
        resp->addHeader("Access-Control-Allow-Headers", "*");
        // Add other CORS headers you need
        stop(resp); // stops processing the request and sends the response
    });

    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr& req, const drogon::HttpResponsePtr& resp)
    {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS, PATCH");
        resp->addHeader("Access-Control-Allow-Headers", "*");
    });

    auto ips = CoreHelper::GetAllListenAddresses(ip);
    for (const auto& i : ips)
    {
		BOOST_LOG_TRIVIAL(info) << "Listening on: https://" << i;
	}

    BOOST_LOG_TRIVIAL(info) << "Drogon server is running with " << cpus << " threads";
    drogon::app().run();
#if defined _DEBUG
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}