#ifndef API_HPP
#define API_HPP

#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>

using namespace web::http::experimental::listener;
using HostInetInfo = boost::asio::ip::tcp::resolver::iterator;

class MyCommandHandler
{
public:

    MyCommandHandler(){}

    MyCommandHandler(utility::string_t url);
    pplx::task<void> accept();
    pplx::task<void> shutdown();

    void initHandlers();
    void setEndpoints(const std::string &values);

    std::string endpoints();
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

#endif /*_API_HPP*/

