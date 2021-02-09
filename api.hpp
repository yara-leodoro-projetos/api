#ifndef API_HPP
#define API_HPP

#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>

using namespace web::http::experimental::listener;


class MyCommandHandler{

public:

    MyCommandHandler(){}
    
    void initHandlers();
    void setEndpoints(const std::string &values);

    MyCommandHandler(utility::string_t url);
    pplx::task<void> accept();
    pplx::task<void> shutdown();
    

    std::string endpoints();
    std::vector<utility::string_t> requestPatch(const web::http::http_request & message);

    static std::string hostName();

   
private:
    void handleGet(web::http::http_request message);
    void handlePut(web::http::http_request message);
    void handlePost(web::http::http_request message);
    void handleDelete(web::http::http_request message);
    void handlePatch(web::http::http_request messge);
    void handleHead(web::http::http_request message);
    void handleOptions(web::http::http_request message);
    void handleTrace(web::http::http_request message);
    void handleConnect(web::http::http_request message);
    void handleMerge(web::http::http_request message);
    web::json::value responseNotImpl(const web::http::method & method);
    http_listener _listener;
};

#endif /*_API_HPP*/

