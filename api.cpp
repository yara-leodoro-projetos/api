#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/json.h>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>
#include "api.hpp"

using namespace web::http::experimental::listener;


void MyCommandHandler::initHandlers() 
{
    _listener.support(web::http::methods::GET, std::bind(&MyCommandHandler::handleGet, this, std::placeholders::_1));
    _listener.support(web::http::methods::POST, std::bind(&MyCommandHandler::handlePost, this, std::placeholders::_1));
    _listener.support(web::http::methods::PUT, std::bind(&MyCommandHandler::handlePut, this, std::placeholders::_1));
    _listener.support(web::http::methods::PATCH, std::bind(&MyCommandHandler::handlePatch, this, std::placeholders::_1));
    _listener.support(web::http::methods::DEL, std::bind(&MyCommandHandler::handleDelete, this, std::placeholders::_1));;
}

void MyCommandHandler::handleGet(web::http::http_request message)
{
    auto path = requestPatch(message);

    if (!path.empty())
    {
        if (path[1] == "person" && path[2] == "test")
        {
            auto response = web::json::value::object();
            response["mensage"] = web::json::value::string("person");
            message.reply(web::http::status_codes::OK, response);
            message.reply(web::http::status_code(), "ACCEPTED");    
        }
    }  
    else{
        message.reply(web::http::status_codes::NotFound);
    }
}

void MyCommandHandler::handlePatch(web::http::http_request message)
{
   message.reply(web::http::status_codes::NotImplemented, (web::http::methods::PATCH));
}

void MyCommandHandler::handlePut(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::PUT));
}

void MyCommandHandler::handlePost(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::POST));
}

void MyCommandHandler::handleDelete(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::DEL));
}

void MyCommandHandler::handleHead(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::HEAD));
}

void MyCommandHandler::handleOptions(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::OPTIONS));
}

void MyCommandHandler::handleMerge(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::MERGE));
}

void MyCommandHandler::handleConnect(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::CONNECT));
}

void MyCommandHandler::handleTrace(web::http::http_request message)
{
    message.reply(web::http::status_codes::NotImplemented, (web::http::methods::TRCE));
}

void MyCommandHandler::setEndpoints(const std::string &values)
{
    web::uri endpointURI(values);
    web::uri_builder endpointBuilder;

    endpointBuilder.set_scheme(endpointURI.scheme());
    endpointBuilder.set_host(endpointURI.host());

    endpointBuilder.set_port(endpointURI.port());
    endpointBuilder.set_path(endpointURI.path());

    _listener =  http_listener(endpointBuilder.to_uri());

}

std::string MyCommandHandler::endpoints() 
{
    return _listener.uri().to_string();
}

pplx::task<void>MyCommandHandler::accept()
{
    return _listener.open();
}

pplx::task<void>MyCommandHandler::shutdown()
{
    return _listener.close();
}

std::vector<utility::string_t>MyCommandHandler::requestPatch(const web::http::http_request & message)
{
    auto relativePath = web::uri::decode(message.relative_uri().path());
    return web::uri::split_path(relativePath);
}

web::json::value MyCommandHandler::responseNotImpl(const web::http::method & method)
{
    auto response = web::json::value::object();
    response["http_method"] = web::json::value::string(method);
    return response;
}

