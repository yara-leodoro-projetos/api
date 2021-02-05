#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>
#include "api.hpp"

using namespace web::http::experimental::listener;

using HostInetInfo = boost::asio::ip::tcp::resolver::iterator;


void MyCommandHandler::initHandlers() 
{
    listener.support(web::http::methods::GET, std::bind(&MyCommandHandler::handlerGet, this, std::placeholders::_1));
    listener.support(web::http::methods::POST, std::bind(&MyCommandHandler::handlerPost, this, std::placeholders::_1));
    listener.support(web::http::methods::PUT, std::bind(&MyCommandHandler::handlerPut, this, std::placeholders::_1));
    listener.support(web::http::methods::PATCH, std::bind(&MyCommandHandler::handlerPatch, this, std::placeholders::_1));
    listener.support(web::http::methods::DEL, std::bind(&MyCommandHandler::handlerDelete, this, std::placeholders::_1));;
}

void MyCommandHandler::handlerGet(web::http::http_request message)
{
    message.reply(web::http::status_code(), "ACCEPTED");
}

void MyCommandHandler::handlerPatch(web::http::http_request message)
{
   message.reply(web::http::status_codes::NotImplemented, (web::http::methods::PATCH));
}

void MyCommandHandler::handlerPut(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::PUT));
}

void MyCommandHandler::handlerPost(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::POST));
}

void MyCommandHandler::handlerDelete(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::DEL));
}

void MyCommandHandler::handlerHead(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::HEAD));
}

void MyCommandHandler::handlerOptions(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::OPTIONS));
}

void MyCommandHandler::handlerMerge(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::MERGE));
}

void MyCommandHandler::handlerConnect(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::CONNECT));
}

void MyCommandHandler::handlerTrace(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::TRCE));
}

HostInetInfo MyCommandHandler::queryHostinetInfo()
{
    boost::asio::io_service ios;
    boost::asio::ip::tcp::resolver resolver(ios);
    boost::asio::ip::tcp::resolver::query query(boost::asio::ip::host_name(), "");
    return resolver.resolve(query);
}

void MyCommandHandler::setEndpoints(const std::string &values)
{
    web::uri endpointURI(values);
    web::uri_builder endpointBuilder;

    endpointBuilder.set_scheme(endpointURI.scheme());
    endpointBuilder.set_host(endpointURI.host());

    endpointBuilder.set_port(endpointURI.port());
    endpointBuilder.set_path(endpointURI.path());

    listener =  http_listener(endpointBuilder.to_uri());
}

std::string MyCommandHandler::endpoints() 
{
    return listener.uri().to_string();
}

pplx::task<void>MyCommandHandler::accept()
{
    initHandlers();
    return listener.open();
}

pplx::task<void>MyCommandHandler::shutdown()
{
    return listener.close();
}

std::vector<utility::string_t>MyCommandHandler::requestPatch(const web::http::http_request & message)
{
    auto relativePath = web::uri::decode(message.relative_uri().path());
    return web::uri::split_path(relativePath);
}

