#pragma once

#include "http/interfaces/http.hpp"

#include <memory>

namespace http
{

class HttpFactory
{
  public:
    template <typename T>
    static std::shared_ptr<HttpIf> create()
    {
        return std::shared_ptr<T>(new T());
    }
};

} // namespace http
