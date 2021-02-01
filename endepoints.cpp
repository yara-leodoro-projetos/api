#include <iostream>
#include <string>
#include <map>
#include <regex>
#include <vector>
#include <mutex>
#include <signal.h>
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
    
    std::string hostIP4();
    std::string hostIP6();
    std::string endpoint();
    std::vector<utility::string_t> requestPatch(const http_request & message);
    static std::string hostName();
    static void hookSIGINT();
    static void waitForUserInterrupt();
    static void handleUserInterrupt(int signal);
    static HostInetInfo queryHostinetInfo();
    static std::string hostIP(unsigned short family);

    
   
private:

    static std::condition_variable _condition;
    static std::mutex _mutex;
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
    
    ucout << "Method: " << message.method() << std::endl;
    ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
    ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;
    message.reply(status_code(), "ACCEPTED");

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

HostInetInfo MyCommandHandler::queryHostinetInfo()
{
    io_service ios;
    tcp::resolver resolver(ios);
    tcp::resolver::query query(host_name(), "");
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

void MyCommandHandler::setEndepoints(const std::string &value)
{
    uri endpointURI(value);
    uri_builder endpointBuilder;

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
    try
    {
        MyCommandHandler handler;

        handler.setEndepoints("http://localhost:5000/endpoint");

        handler.open(); 

        std::cout << utility::string_t(U("Listening for request at: ")) << handler.endpoint() << std::endl;
        std::cout << U("press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
        handler.close().wait();
    }

    catch(std::exception& ex)
    {
        std::cout << U("Exception: ") << ex.what() << std::endl;
        std::cout << U("press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
    }
    return 0;
}


