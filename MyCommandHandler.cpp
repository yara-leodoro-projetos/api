#include <iostream>
#include <stdio.h>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>


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
    virtual void handlerRequest(http_request &request, http_response &response);

private:
    
    void handlerGetPost(http_request message);
    http_listener m_listener;

};

MyCommandHandler::MyCommandHandler(utility::string_t url) : m_listener(url)
{
    m_listener.support(methods::GET, std::bind(&MyCommandHandler::handlerGetPost, this, std::placeholders::_1));
    m_listener.support(methods::POST, std::bind(&MyCommandHandler::handlerGetPost, this, std::placeholders::_1));
}

void MyCommandHandler::handlerGetPost(http_request message)
{
    ucout << "Method: " << message.method() << std::endl;
    ucout << "URI: " << http::uri::decode(message.relative_uri().path()) << std::endl;
    ucout << "Query: " << http::uri::decode(message.relative_uri().query()) << std::endl << std::endl;
    message.reply(status_code(), "ACCEPTED");
}

virtual void MyCommandHandler::handlerRequest(http_request &request, http_response &response)
{
    response.status_codes(OK);
    response._set_server_context("text/html");

    std::ostream& out = response.send();

    out << "<h1> Hello World </h1>"
        << "<p>Host: " << request.getHost() << "</p>"
        << "<p>Method: " << request.getMethod() << "</p>"
        << "<p>URI: " << request.getURI() << "</p>"
        << "<p>String: " << "</p>"
        << "<p>Count: " << count++ <<  "</p>";
    out.flush();


}


int main(int argc, char const *argv[])
{
    try
    {
        utility::string_t address = U("http://192.168.10.109:5000");
        uri_builder uri(address);
        auto addr = uri.to_uri().to_string();

        MyCommandHandler handler(addr);

        handler.open().wait();

        ucout << utility::string_t(U("Listening for request at: ")) << addr << std::endl;
        ucout << U("press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
        handler.close().wait();
    }

    catch(std::exception& ex)
    {
        ucout << U("Exception: ") << ex.what() << std::endl;
        ucout << U("press ENTER key to quit...") << std::endl;
        std::string line;
        std::getline(std::cin, line);
    }
    return 0;
}

