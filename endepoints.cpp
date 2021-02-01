#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include <cpprest/json.h>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/console.hpp>
#include <boost/log/support/date_time.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/asio.hpp>
#include <boost/algorithm/string.hpp>


using namespace web;
using namespace http;
using namespace utility;
using namespace http::experimental::listener;
using namespace boost::asio;
using namespace boost::asio::ip;

using HostInetInfo = tcp::resolver::iterator;

class MyCommandHandler
{
public:

    MyCommandHandler(){}

    MyCommandHandler(utility::string_t url);
    pplx::task<void> open(){return listener.open();}
    pplx::task<void> close(){return listener.close();}
    pplx::task<void> accept();
    pplx::task<void> shutdown();
    void setEndepoints(const std::string & value);
    void initHandlers();
    std::string endpoint();
    std::vector<utility::string_t> requestPatch(const http_request & message);
    static std::string hostIP4();
    static std::string hostIP6();
    static std::string hostName();
   
private:
    
    static json::value responseNotImpl(const http::method & method);
    static HostInetInfo queryHostinetInfo();
    static std::string hostIP(unsigned short family);
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
    http_listener listener;

};

void MyCommandHandler::initHandlers() 
{
    listener.support(methods::GET, std::bind(&MyCommandHandler::handlerGet, this, std::placeholders::_1));
    listener.support(methods::POST, std::bind(&MyCommandHandler::handlerPut, this, std::placeholders::_1));
    listener.support(methods::PUT, std::bind(&MyCommandHandler::handlerPost, this, std::placeholders::_1));
    listener.support(methods::POST, std::bind(&MyCommandHandler::handlerDelete, this, std::placeholders::_1));
    listener.support(methods::DEL, std::bind(&MyCommandHandler::handlerPatch, this, std::placeholders::_1));;
    
}

void MyCommandHandler::handlerGet(http_request message)
{
    auto path = requestPatch(message);
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

HostInetInfo MyCommandHandler::queryHostinetInfo()
{
    io_service ios;

    tcp::resolver resolver(ios);
    tcp::resolver::query query(host_name(), " ");
    return resolver.resolve(query);
}

std::string MyCommandHandler::hostIP(unsigned short family)
{
    auto hostInetInfo = queryHostinetInfo();
    tcp::resolver::iterator end;

    while(hostInetInfo != end)
    {
        tcp::endpoint ep = *hostInetInfo++;
        sockaddr sa = *ep.data();

        if(sa.sa_family == family)
        {
            return ep.address().to_string();
        }
    }
    return nullptr;
}

void MyCommandHandler::setEndepoints(const std::string &value)
{
    uri endpointURI(value);
    uri_builder endpointBuilder;

    endpointBuilder.set_scheme(endpointURI.scheme());

    if (endpointURI.host() == "host_auto_ip4")
    {
        endpointBuilder.set_host(MyCommandHandler::hostIP4());
    }
    else if(endpointURI.host() == "host_auto_ip6")
    {
        endpointBuilder.set_host(MyCommandHandler::hostIP6());
    }

    endpointBuilder.set_port(endpointURI.port());
    endpointBuilder.set_path(endpointURI.path());

    listener = http_listener(endpointBuilder.to_uri());
}

std::string MyCommandHandler::endpoint() 
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

std::vector<utility::string_t>MyCommandHandler::requestPatch(const http_request & message)
{
    auto relativePath = uri::decode(message.relative_uri().path());
    return uri::split_path(relativePath);
}


int main(int argc, char const *argv[])
{
   MyCommandHandler server;

   server.setEndepoints("http://192.168.10.109:5000");
   server.accept().wait();
   std::cout << "Fazendo solicitações em : " << server.endpoint() << std::endl;
}





