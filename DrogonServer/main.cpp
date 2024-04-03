#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include "ConfigProvider.h"
#include <boost/log/trivial.hpp>
#include <drogon/drogon.h>
#include "DbContext.h"

int main()
{
#if defined _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    std::cout << drogon::banner;
    BOOST_LOG_TRIVIAL(info) << "Drogon server starting...";

    //Set HTTP listener address and port

    drogon::app().addListener( "0.0.0.0", 443, true, "certificate.crt", "private.key");
    drogon::app().setLogPath("");
    drogon::app().setLogLevel( trantor::Logger::kTrace );
    drogon::app().setThreadNum( 16 );
    drogon::app().enableGzip(true);
    drogon::app().enableBrotli(false);
    drogon::app().enableDateHeader(true);
    drogon::app().enableServerHeader(true);
    drogon::app().setCustom404Page(drogon::HttpResponse::newFileResponse("public/404.html", "", drogon::CT_TEXT_HTML));

    //Load config file
    //Run HTTP framework,the method will block in the internal event loop
    ConfigProvider::GetInstance()->Initialize();


   

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
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
        resp->addHeader("Access-Control-Allow-Headers", "*");
        // Add other CORS headers you need
        stop(resp); // stops processing the request and sends the response
    });

    drogon::app().registerPostHandlingAdvice([](const drogon::HttpRequestPtr& req, const drogon::HttpResponsePtr& resp)
    {
        resp->addHeader("Access-Control-Allow-Origin", "*");
        resp->addHeader("Access-Control-Allow-Methods", "GET, POST, OPTIONS, PUT, PATCH");
        resp->addHeader("Access-Control-Allow-Headers", "*");
    });

    BOOST_LOG_TRIVIAL(info) << "Drogon server started at 127.0.0.1:443";
    BOOST_LOG_TRIVIAL(info) << "Drogon server running with 16 thread";
    drogon::app().run();
#if defined _DEBUG
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}