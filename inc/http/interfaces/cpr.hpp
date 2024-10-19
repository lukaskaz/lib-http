#pragma once

#include "http/factory.hpp"

#include <string>

namespace http::cpr
{

class Http : public HttpIf
{
  public:
    ~Http();
    std::string get(const std::string&) override;
    std::string info() override;

  private:
    friend class http::HttpFactory;
    Http();
    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace http::cpr
