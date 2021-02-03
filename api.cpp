#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>
#include "api.hpp"

using namespace web::http::experimental::listener;

using HostInetInfo = boost::asio::ip::tcp::resolver::iterator;

class MyCommandHandler
{
public:

    MyCommandHandler(){}

    MyCommandHandler(utility::string_t url);
    pplx::task<void> open(){return listener.open();}
    pplx::task<void> close(){return listener.close();}
    pplx::task<void> accept();
    pplx::task<void> shutdown();

    void initHandlers();
    void setEndpoints(const std::string &value);

    std::string hostIP4();
    std::string hostIP6();
    std::string endpoint();
    std::vector<utility::string_t> requestPatch(const web::http::http_request & message);

    static std::string hostName();
    static HostInetInfo queryHostinetInfo();
    static std::string hostIP(unsigned short family);

    
   
private:

    void handlerGet(web::http::http_request message);
    void handlerPut(web::http::http_request message);
    void handlerPost(web::http::http_request message);
    void handlerDelete(web::http::http_request message);
    void handlerPatch(web::http::http_request message);
    void handlerHead(web::http::http_request message);
    void handlerOptions(web::http::http_request message);
    void handlerConnect(web::http::http_request message);
    void handlerMerge(web::http::http_request message);
    void handlerTrace(web::http::http_request message);
    void initRestOpHandler();
    http_listener listener;
};

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
    
    ucout << "Method: " << message.method() << std::endl;
    ucout << "URI: " << web::http::uri::decode(message.relative_uri().path()) << std::endl;
    ucout << "Query: " << web::http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;
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

std::string MyCommandHandler::hostIP(unsigned short family)
{
    auto hostInetInfo = queryHostinetInfo();
    boost::asio::ip::tcp::resolver::iterator end;
    while(hostInetInfo != end)
    {
        boost::asio::ip::tcp::endpoint ep = *hostInetInfo++;
        sockaddr sa = *ep.data();
        if (sa.sa_family == family)
        {
            return ep.address().to_string();
        }
       return nullptr;
    }
}

std::string MyCommandHandler::hostIP4()
{
    return hostIP(AF_INET);
}

std::string MyCommandHandler::hostIP6()
{
    return hostIP(AF_INET6);
}

void setEndpoints(const std::string &value);
{
    web::uri endpointURI(value);
    web::uri_builder endpointBuilder;

    endpointBuilder.set_scheme(endpointURI.scheme());

    if (endpointURI.host() == "localhost")
    {
        endpointBuilder.set_host(MyCommandHandler::hostIP4());
    }
    else if(endpointURI.host() == "host_auto_ip6")
    {
        endpointBuilder.set_host(MyCommandHandler::hostIP6());
    }

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

