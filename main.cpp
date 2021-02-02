
int main(int argc, char const *argv[])
{
    try
    {
        MyCommandHandler handler;

        handler.setEndepoints("http://localhost:5000/endpoint");
        handler.setEndepoints("http://localhost:5000/analytics");

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

