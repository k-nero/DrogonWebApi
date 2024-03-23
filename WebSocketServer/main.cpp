#include <uwebsockets/App.h>
#include "CoreHelper.h"
#include <jwt-cpp/jwt.h> 
#include "ConfigProvider.h"
#include "unordered_map"
#include <json/json.h>


struct us_listen_socket_t* global_listen_socket;

struct PerSocketData
{
	std::string user_id;
};

static std::unordered_map<std::string, uWS::WebSocket<true, true, PerSocketData>* > user_socket_map;

int main()
{

	ConfigProvider::GetInstance()->Initialize();

	/* ws->getUserData returns one of these */

	uWS::SocketContextOptions options;
	options.cert_file_name = "certificate.crt";
	options.key_file_name = "private.key";

	auto app = std::make_unique<uWS::SSLApp>(options);

	auto ws_upgrade = [](auto* res, auto* req, auto* context)
	{
		struct UpgradeData
		{
			std::string secWebSocketKey;
			std::string secWebSocketProtocol;
			std::string secWebSocketExtensions;
			struct us_socket_context_t* context;
			decltype(res) httpRes;
			bool aborted = false;
		};

		auto upgradeData = new UpgradeData{
			std::string(req->getHeader("sec-websocket-key")),
			std::string(req->getHeader("sec-websocket-protocol")),
			std::string(req->getHeader("sec-websocket-extensions")),
			context,
			res
		};

		std::string user_id;

		auto publicKey = ConfigProvider::GetInstance()->GetPublicRSAKey();
		try
		{
			std::string access_token(req->getHeader("authorization"));
			if (access_token.empty())
			{
				upgradeData->aborted = true;
				std::cout << "Unauthorized";
				res->writeStatus("401 Unauthorized")->end("Unauthorized");
				return;
			}

			auto decoded = jwt::decode(access_token);
			jwt::verify().allow_algorithm(jwt::algorithm::rs512(publicKey)).with_issuer("auth0").verify(decoded);
			user_id = decoded.get_payload_claim("sub").as_string();
		}
		catch (...)
		{
			upgradeData->aborted = true;
			std::cout << "Unknow exception";
			res->writeStatus("401 Unauthorized")->end("Unauthorized");
			return;
		}
		/* We have to attach an abort handler for us to be aware
		 * of disconnections while we perform async tasks */
		res->onAborted([=]()
		{
			upgradeData->aborted = true;
			std::cout << "HTTP socket was closed before we upgraded it!" << std::endl;
		});

		if (!upgradeData->aborted)
		{
			std::cout << "Async task done, upgrading to WebSocket now!" << std::endl;

			/* If you don't want to upgrade you can instead respond with custom HTTP here,
			* such as res->writeStatus(...)->writeHeader(...)->end(...); or similar.*/

			/* This call will immediately emit .open event */

			upgradeData->httpRes->cork([&]()
			{
				upgradeData->httpRes->upgrade<PerSocketData>({
					std::string(user_id)
					/* We initialize PerSocketData struct here */
					}, upgradeData->secWebSocketKey,
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
	};

	auto ws_open = [](auto* ws)
	{
		/* Open event here, you may access ws->getUserData() which points to a PerSocketData struct */
		auto perSocketData = ws->getUserData();
		user_socket_map[perSocketData->user_id] = ws;
		//ws->subscribe("global_soc");
	};

	auto ws_message = [&app](auto* ws, std::string_view message, uWS::OpCode opCode)
	{
		auto perSocketData = ws->getUserData();
		auto mess = std::string(message);
		auto json = CoreHelper::ParseJson(mess);


		if (json["type"] == "message")
		{
			ws->publish(json["channel"].asString(), json["message"].asString(), opCode);
		}
		else if (json["type"] == "subscribe")
		{
			ws->subscribe(json["channel"].asString());
			app->publish(json["channel"].asString(), "subscribed", uWS::OpCode::TEXT);
		}
		else if (json["type"] == "unsubscribe")
		{
			ws->unsubscribe(json["channel"].asString());
		}
		else if (json["type"] == "publish")
		{

		}
		else if (json["type"] == "broadcast")
		{

		}
		else if (json["type"] == "ping")
		{
			for (auto& [key, value] : user_socket_map)
			{
				value->send("ping all socket", uWS::OpCode::TEXT);
			}
		}

		app->publish("global_soc", message, opCode);
	};

	auto ws_close = [](auto* ws, int code, std::string_view message)
	{
		auto perSocketData = ws->getUserData();
		user_socket_map.erase(perSocketData->user_id);
	};

	app->ws<PerSocketData>("/*",
		{
		.compression = uWS::DISABLED,
		.maxPayloadLength = 16 * 1024 * 1024,
		.idleTimeout = 60,
		.maxBackpressure = 16 * 1024 * 1024,
		.resetIdleTimeoutOnSend = true,
		.upgrade = ws_upgrade,
		.open = ws_open,
		.message = ws_message,
		.close = ws_close
		}).listen(9001, [](auto* listen_s)
		{
			if (listen_s)
			{
				std::cout << "Listening on port " << 9001 << std::endl;
			}
		});
		app->run();
		uWS::Loop::get()->free();
}