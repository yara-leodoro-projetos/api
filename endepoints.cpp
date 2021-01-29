#include <iostream>
#include <stdio.h>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/json.h>


using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;

class MyCommandHandler
{
public:

    MyCommandHandler(){}

    MyCommandHandler(utility::string_t url);
    pplx::task<void> open(){return m_listener.open();}
    pplx::task<void> close(){return m_listener.close();}

private:
    
    static json::value responseNotImpl(const http::method & method);
    void handlerGet(http_request message);
    void handlerPut(http_request message);
    void handlerPost(http_request message);
    void handlerDelete(http_request message);
    void handlerPatch(http_request message);
    void handlerHead(http_request message);
    void handlerOptions(http_request message);
    void handlerConnect(http_request message);
    void handlerMerge(http_request message);
    void handlerTrace(http_request message);
    void initRestOpHandler();
    http_listener m_listener;

};

MyCommandHandler::MyCommandHandler(utility::string_t url) : m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&MyCommandHandler::handlerGet, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyCommandHandler::handlerPut, this, std::placeholders::_1));
    m_listener.support(methods::PUT, std::bind(&MyCommandHandler::handlerPost, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyCommandHandler::handlerDelete, this, std::placeholders::_1));
    m_listener.support(methods::DEL, std::bind(&MyCommandHandler::handlerPatch, this, std::placeholders::_1));;
}

void MyCommandHandler::handlerGet(http_request message)
{
    auto path = requestPath(message);
    if (!path.empty())
    {
        if(path[0] == "service" && path[1] == "test")
        {
            auto response = json::value::object();
            response["version"] = json::value::string("0.1.1");
            response["status"] = json::value::string("ready!");
            message.reply(status_codes::OK, response);
        }
    }
    else
    {
        message.reply(status_codes::NotFound);
    }
}

void MyCommandHandler::handlerPatch(http_request message)
{
   message.reply(status_codes::NotImplemented, (methods::PATCH));
}

void MyCommandHandler::handlerPut(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::PUT));
}

void MyCommandHandler::handlerPost(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::POST));
}

void MyCommandHandler::handlerDelete(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::DEL));
}

void MyCommandHandler::handlerHead(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::HEAD));
}

void MyCommandHandler::handlerOptions(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::OPTIONS));
}

void MyCommandHandler::handlerMerge(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::MERGE));
}

void MyCommandHandler::handlerConnect(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::CONNECT));
}

void MyCommandHandler::handlerTrace(http_request message)
{
    message.reply(status_codes::NotImplemented, (methods::TRCE));
}

json::value MyCommandHandler::responseNotImpl(const http::method & method)
{
    auto response = json::value::object();
    response["serviceName"] = json::value::string("Endpoints");
    response["http_method"] = json::value::string(method);
    return response;
}


int main(int argc, char const *argv[])
{
   MyCommandHandler server;
}





