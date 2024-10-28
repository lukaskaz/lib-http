#pragma once

#include "http/interfaces/http.hpp"
#include "log/interfaces/logging.hpp"

#include <memory>

namespace http
{

class HttpFactory
{
  public:
    template <typename T>
    static std::shared_ptr<HttpIf> create()
    {
        std::shared_ptr<logging::LogIf> logIf{nullptr};
        return create<T>(logIf);
    }

    template <typename T>
    static std::shared_ptr<HttpIf> create(std::shared_ptr<logging::LogIf> logIf)
    {
        return std::shared_ptr<T>(new T(logIf));
    }
};

} // namespace http
