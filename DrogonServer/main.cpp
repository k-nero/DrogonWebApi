#if defined _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include <iostream>
#include "ConfigProvider.h"
#include <boost/log/trivial.hpp>
#include <drogon/drogon.h>


int main()
{
#if defined _DEBUG
    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode(_CRT_WARN, _CRTDBG_MODE_DEBUG);
#endif
    //Set HTTP listener address and port
    drogon::app().addListener( "127.0.0.1", 443, true, "certificate.crt", "private.key");
    drogon::app().setLogPath("", "", 100000000, 0, true);
    drogon::app().setLogLevel( trantor::Logger::kInfo );
    drogon::app().setThreadNum( 16 );
    drogon::app().enableGzip(true);
    drogon::app().enableBrotli(false);
    drogon::app().enableDateHeader(true);
    drogon::app().enableServerHeader(true);

    //Load config file
    //drogon::app().loadConfigFile("../config.json");
    //Run HTTP framework,the method will block in the internal event loop
    ConfigProvider::GetInstance()->Initialize();
    BOOST_LOG_TRIVIAL(info) << "Drogon server started at 127.0.0.1:443";
    drogon::app().run();
#if defined _DEBUG
    _CrtDumpMemoryLeaks();
#endif
    return 0;
}