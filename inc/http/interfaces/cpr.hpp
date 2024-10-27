#pragma once

#include "http/factory.hpp"

#include <string>

namespace http::cpr
{

class Http : public HttpIf
{
  public:
    ~Http();
    bool get(const datamap&, std::string&) override;
    bool get(const datamap&, datamap&) override;
    std::string info() override;

  private:
    friend class http::HttpFactory;
    Http();
    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace http::cpr
