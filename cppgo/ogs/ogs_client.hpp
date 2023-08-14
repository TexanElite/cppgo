#ifndef OGS_CLIENT_HPP
#define OGS_CLIENT_HPP

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <iostream>

#include "https_client.hpp"

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;
using tcp = net::ip::tcp;

namespace cppgo::ogs {

class OgsClient {
   public:
    constexpr static const std::string host = "online-go.com";
    OgsClient(const std::string client_id, const std::string username,
              const std::string password) {
        std::string res = _https_client.post(
            host, "/oauth2/token/",
            std::format(
                "client_id={}&grant_type=password&username={}&password={}",
                client_id, username, password));
        json::parser parser;
        parser.write(res);
        const json::value res_json = parser.release();
        _access_token = res_json.get_object().at("access_token").get_string();
    }

   private:
    HttpsClient _https_client;
    std::string _access_token;
};
}  // namespace cppgo::ogs

#endif