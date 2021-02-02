#include <iostream>
#include <vector>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <boost/asio.hpp>
#include "api.hpp"

int main(int argc, char const *argv[])
{
    try
    {
        MyCommandHandler handler;

        handler.setEndepoints("http://localhost:5000/endpoint");
        handler.setEndepoints("http://localhost:5000/analytics");

        handler.open(); 

        std::cout << "Listening for request at: " << handler.endpoint() << std::endl;
        std::cout << "press ENTER key to quit..." << std::endl;
        std::string line;
        std::getline(std::cin, line);
        handler.close().wait();
    }

    catch(std::exception& ex)
    {
        std::cout << "Exception: " << ex.what() << std::endl;
        std::cout << "press ENTER key to quit..." << std::endl;
        std::string line;
        std::getline(std::cin, line);
    }
    return 0;
}

