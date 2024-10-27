#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>

namespace http
{

using datatype =
    std::variant<std::monostate, bool, int32_t, uint32_t, double, std::string>;
using datamap = std::unordered_map<std::string, datatype>;

class HttpIf
{
  public:
    virtual ~HttpIf() = default;
    virtual bool get(const datamap&, std::string&) = 0;
    virtual bool get(const datamap&, datamap&) = 0;
    virtual std::string info() = 0;
};

} // namespace http
