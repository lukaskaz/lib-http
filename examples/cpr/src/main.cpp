#include "http/interfaces/cpr.hpp"

#include <iostream>

int main()
{
    try
    {
        using namespace http::cpr;
        auto httpIf = http::HttpIfFactory<http::cpr::Http>::create();
    }
    catch (std::exception& err)
    {
        std::cerr << "[ERROR] " << err.what() << '\n';
    }

    return 0;
}