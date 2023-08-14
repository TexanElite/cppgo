#ifndef HTTPS_CLIENT_HPP
#define HTTPS_CLIENT_HPP

#include <boost/asio/connect.hpp>
#include <boost/asio/ip/tcp.hpp>
#include <boost/beast/core.hpp>
#include <boost/beast/http.hpp>
#include <boost/beast/ssl.hpp>
#include <boost/beast/version.hpp>
#include <boost/beast/websocket.hpp>
#include <boost/json.hpp>
#include <iostream>

namespace beast = boost::beast;
namespace http = beast::http;
namespace websocket = beast::websocket;
namespace net = boost::asio;
namespace ssl = net::ssl;
namespace json = boost::json;
using tcp = net::ip::tcp;

namespace cppgo::ogs {

class HttpsClient {
   public:
    HttpsClient() : _resolver{_ioc} {}
    std::string get(
        std::string host, std::string target,
        std::vector<std::pair<std::string, std::string>> header_map = {}) {
        tcp::resolver resolver{_ioc};
        auto const results = resolver.resolve(host, "443");

        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_peer);

        beast::ssl_stream<beast::tcp_stream> stream =
            beast::ssl_stream<beast::tcp_stream>{_ioc, ctx};

        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                 net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        beast::get_lowest_layer(stream).connect(results);

        stream.handshake(ssl::stream_base::client);

        http::request<http::string_body> req{http::verb::get, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::user_agent, BOOST_BEAST_VERSION_STRING);
        for (auto& entry : header_map) {
            req.set(entry.first, entry.second);
        }
        req.prepare_payload();

        http::write(stream, req);
        beast::flat_buffer buffer;

        http::response<http::string_body> res;
        http::read(stream, buffer, res);

        beast::error_code ec;
        stream.shutdown(ec);

        if (ec == net::error::eof || ec == net::ssl::error::stream_truncated) {
            ec = {};
        }

        if (ec) {
            throw beast::system_error{ec};
        }
        return res.body();
    }

    std::string post(
        std::string host, std::string target, std::string body,
        std::vector<std::pair<std::string, std::string>> header_map = {}) {
        tcp::resolver resolver{_ioc};
        auto const results = resolver.resolve(host, "443");

        ssl::context ctx(ssl::context::tlsv12_client);
        ctx.set_default_verify_paths();
        ctx.set_verify_mode(ssl::verify_peer);

        beast::ssl_stream<beast::tcp_stream> stream =
            beast::ssl_stream<beast::tcp_stream>{_ioc, ctx};

        if (!SSL_set_tlsext_host_name(stream.native_handle(), host.c_str())) {
            beast::error_code ec{static_cast<int>(::ERR_get_error()),
                                 net::error::get_ssl_category()};
            throw beast::system_error{ec};
        }

        beast::get_lowest_layer(stream).connect(results);

        stream.handshake(ssl::stream_base::client);

        http::request<http::string_body> req{http::verb::post, target, 11};
        req.set(http::field::host, host);
        req.set(http::field::content_type, "application/x-www-form-urlencoded");
        for (auto& entry : header_map) {
            req.set(entry.first, entry.second);
        }
        req.body() = body;
        req.prepare_payload();
        http::write(stream, req);
        beast::flat_buffer buffer;

        http::response<http::string_body> res;
        http::read(stream, buffer, res);
        beast::error_code ec;
        stream.shutdown(ec);

        if (ec == net::error::eof || ec == net::ssl::error::stream_truncated) {
            ec = {};
        }

        if (ec) {
            throw beast::system_error{ec};
        }
        return res.body();
    }

   private:
    net::io_context _ioc;
    tcp::resolver _resolver;
};
}  // namespace cppgo::ogs

#endif