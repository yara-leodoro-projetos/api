#ifndef API_HPP
#define API_HPP

#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>
#include "controller.hpp"


using namespace web::http::experimental::listener;
using namespace cfx;

class MyCommandHandler : public Controller
{
public:

    MyCommandHandler():Controller(){}
    ~MyCommandHandler(){}
    void handleGet(http_request message) override;
    void handlePut(http_request message) override;
    void handlePost(http_request message) override;
    void handleDelete(http_request message) override;
    void handlePatch(http_request messge) override;
    void handleHead(http_request message) override;
    void handleOptions(http_request message) override;
    void handleTrace(http_request message) override;
    void handleConnect(http_request message) override;
    void handleMerge(http_request message) override;
    void initHandlers() override;
    void setEndpoints(const std::string &values);

    MyCommandHandler(utility::string_t url);
    pplx::task<void> accept();
    pplx::task<void> shutdown();
    

    std::string endpoints();
    std::vector<utility::string_t> requestPatch(const web::http::http_request & message);

    static std::string hostName();

   
private:
    static web::json::value responseNotImpl(const web::http::method & method);

protected:
    http_listener _listener;
};

#endif /*_API_HPP*/

