//#include "CoreHelper.h"
#include <uwebsockets/App.h>

struct us_listen_socket_t* global_listen_socket;

int main()
{

	/* ws->getUserData returns one of these */
	struct PerSocketData
	{
		/* Fill with user data */
		std::vector<std::string> topics;
		std::string socket_id;
	};

	uWS::SocketContextOptions options;
	options.cert_file_name = "certificate.crt";
	options.key_file_name = "private.key";

	auto app = std::make_unique<uWS::SSLApp>(options);

	app->ws<PerSocketData>("/*",
		{
				.compression = uWS::DISABLED,
				.maxPayloadLength = 16 * 1024 * 1024,
				.idleTimeout = 60,
				.maxBackpressure = 16 * 1024 * 1024,
				.resetIdleTimeoutOnSend = true,
				.upgrade = [](auto* res, auto* req, auto* context)
				{

				/* HttpRequest (req) is only valid in this very callback, so we must COPY the headers
				 * we need later on while upgrading to WebSocket. You must not access req after first return.
				 * Here we create a heap allocated struct holding everything we will need later on. */

					struct UpgradeData
					{
						std::string secWebSocketKey;
						std::string secWebSocketProtocol;
						std::string secWebSocketExtensions;
						struct us_socket_context_t* context;
						decltype(res) httpRes;
						bool aborted = false;
					};
				
					auto upgradeData = new UpgradeData {
						std::string(req->getHeader("sec-websocket-key")),
						std::string(req->getHeader("sec-websocket-protocol")),
						std::string(req->getHeader("sec-websocket-extensions")),
						context,
						res
					};

					/* We have to attach an abort handler for us to be aware
					 * of disconnections while we perform async tasks */
					res->onAborted([=]()
					{
						/* We don't implement any kind of cancellation here,
						 * so simply flag us as aborted */
						upgradeData->aborted = true;
						std::cout << "HTTP socket was closed before we upgraded it!" << std::endl;
					});

					if (!upgradeData->aborted)
					{
						std::cout << "Async task done, upgrading to WebSocket now!" << std::endl;

							/* If you don't want to upgrade you can instead respond with custom HTTP here,
							* such as res->writeStatus(...)->writeHeader(...)->end(...); or similar.*/

							/* This call will immediately emit .open event */
						upgradeData->httpRes->cork([upgradeData]()
						{
							upgradeData->httpRes->template upgrade<PerSocketData>({
								.topics = {},
								.socket_id = "1234"
									/* We initialize PerSocketData struct here */
							},	upgradeData->secWebSocketKey,
								upgradeData->secWebSocketProtocol,
								upgradeData->secWebSocketExtensions,
								upgradeData->context);
						});
					}
					else
					{
						std::cout << "Async task done, but the HTTP socket was closed. Skipping upgrade to WebSocket!" << std::endl;
					}
					 delete upgradeData;
				},
				.open = [](auto* ws)
				{
				/* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
					auto perSocketData = ws->getUserData();
					int i = 100;
					{
						std::string topic = std::to_string(i);
						perSocketData->topics.push_back(topic);
						ws->subscribe(topic);
					}
				},
				.message = [&app](auto* ws, std::string_view message, uWS::OpCode opCode)
				{
					auto perSocketData = ws->getUserData();
					app->publish(perSocketData->topics[0], message, opCode);
					//ws->publish(perSocketData->topics[0], message, opCode);
					
				},
				.close = [](auto*/*ws*/, int /*code*/, std::string_view /*message*/)
				{
					/* You may access ws->getUserData() here */
				}}).listen(9001, [](auto* listen_s)
					{
					   if (listen_s)
					   {
						   std::cout << "Listening on port " << 9001 << std::endl;
						   //listen_socket = listen_s;
					   }
					});
	   app->run();
	   uWS::Loop::get()->free();
}