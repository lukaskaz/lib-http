#pragma once

#include "http/factory.hpp"

#include <string>

namespace http::cpr
{

class Http : public HttpIf
{
  public:
    ~Http();
    bool get(const inputtype&, std::string&) override;
    bool get(const inputtype&, outputtype&) override;
    std::string info() override;

  private:
    friend class http::HttpFactory;
    Http(std::shared_ptr<logging::LogIf>);
    struct Handler;
    std::unique_ptr<Handler> handler;
};

} // namespace http::cpr
