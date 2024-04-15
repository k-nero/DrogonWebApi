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
	Json::Value contacts;
};

static std::unordered_map<std::string, uWS::WebSocket<true, true, PerSocketData>*> user_socket_map;

int main()
{

	ConfigProvider::GetInstance()->Initialize();

	/* ws->getUserData returns one of these */

	uWS::SocketContextOptions options;
	options.cert_file_name = "certificate.crt";
	options.key_file_name = "private.key";

	auto app = std::make_unique<uWS::SSLApp>(options);

	auto ws_upgrade = [](auto* res, uWS::HttpRequest* req, auto* context)
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
			std::string access_token(req->getQuery("authorization"));
			if (access_token.empty())
			{
				upgradeData->aborted = true;
				std::cout << "No token provied" << std::endl;
				res->writeStatus("401 Unauthorized")->end("No token provied");
				return;
			}

			auto decoded = jwt::decode(access_token);
			jwt::verify().allow_algorithm(jwt::algorithm::rs512(publicKey)).with_issuer("auth0").verify(decoded);
			user_id = decoded.get_payload_claim("sub").as_string();
		}
		catch (...)
		{
			upgradeData->aborted = true;
			std::cout << "401 Unauthorized" << std::endl;
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
		ws->subscribe(perSocketData->user_id);
		//ws->subscribe("global_soc");
	};

	auto ws_message = [&app](auto* ws, std::string_view message, uWS::OpCode opCode)
	{
		auto perSocketData = ws->getUserData();
		auto mess = std::string(message);
		auto json = CoreHelper::ParseJson(mess);

		if (json["type"] == "message_seen_by")
		{
			if (json["message"].isNull())
			{
				return;
			}

			app->publish(json["channel"].asString(), json.toStyledString(), opCode);
		}

		if (json["type"] == "message")
		{
			if (json["message"].isNull())
			{
				return;
			}

			app->publish(json["channel"].asString(), json.toStyledString(), opCode);
		}
		else if (json["type"] == "subscribe")
		{
			ws->subscribe(json["channel"].asString());
			json["socket_id"] = perSocketData->user_id;
			app->publish(json["channel"].asString(), json.toStyledString(), uWS::OpCode::TEXT);
		}
		else if (json["type"] == "unsubscribe")
		{
			ws->unsubscribe(json["channel"].asString());
		}
		else if (json["type"] == "online")
		{
			ws->getUserData()->contacts = json["contacts"];
			for (auto& value : json["contacts"])
			{
				{
					Json::Value message;
					message["type"] = "online";
					message["socket_id"] = perSocketData->user_id;

					ws->publish(value.asString(), message.toStyledString(), opCode);
				}
			}
		}
		else if (json["type"] == "typing")
		{
			app->publish(json["channel"].asString(), json.toStyledString(), opCode);
		}

		app->publish("global_soc", message, opCode);
	};

	auto ws_close = [&app](auto* ws, int code, std::string_view message)
	{
		auto perSocketData = ws->getUserData();
		for (auto& value : perSocketData->contacts)
		{
			//if (user_socket_map.find(value.asString()) != user_socket_map.end())
			{
				Json::Value message;
				message["type"] = "offline";
				message["socket_id"] = perSocketData->user_id;
				app->publish(value.asString(), message.toStyledString(), uWS::OpCode::TEXT);
			}
		}
		
		user_socket_map.erase(perSocketData->user_id);
	};

	app->get("/online-user/:user_id", [&app](auto* res, auto* req)
	{

		auto user_id = std::string(req->getParameter(0));
		if (user_socket_map.find(user_id) != user_socket_map.end())
		{
			res->writeHeader("Access-Control-Allow-Origin", "*");
			res->writeHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
			res->writeStatus("200 OK")->end("true");
		}
		else
		{
			res->writeHeader("Access-Control-Allow-Origin", "*");
			res->writeHeader("Access-Control-Allow-Methods", "GET, POST, PUT, DELETE, OPTIONS");
			res->writeStatus("200 OK")->end("false");
		}
	});

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