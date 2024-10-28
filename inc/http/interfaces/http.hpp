#pragma once

#include <cstdint>
#include <string>
#include <unordered_map>
#include <variant>
#include <vector>

namespace http
{

using datatype =
    std::variant<std::monostate, bool, int32_t, uint32_t, double, std::string>;
using inputtype = std::vector<std::pair<std::string, datatype>>;
using outputtype = std::unordered_map<std::string, datatype>;

class HttpIf
{
  public:
    virtual ~HttpIf() = default;
    virtual bool get(const inputtype&, std::string&) = 0;
    virtual bool get(const inputtype&, outputtype&) = 0;
    virtual std::string info() = 0;
};

} // namespace http
