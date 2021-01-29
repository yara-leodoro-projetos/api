#include <iostream>
#include <stdio.h>
#include <cpprest/uri.h>
#include <cpprest/http_listener.h>
#include <cpprest/asyncrt_utils.h>
#include "MyCommandHandler.cpp"

int main(int argc, char const *argv[])
{
    try
    {
        utility::string_t address = U("http//*:8080");
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
