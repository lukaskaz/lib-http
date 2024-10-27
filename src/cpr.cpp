#include "http/interfaces/cpr.hpp"

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include <cstdlib>
#include <iostream>

using json = nlohmann::json;

template <typename T, typename... Types>
void variant_from_json(const json& json, std::variant<Types...>& variant)
{
    try
    {
        if constexpr (!std::is_same<std::monostate, T>())
        {
            auto value = json.get<T>();
            if (json.type() == nlohmann::json(value).type())
            {
                variant = value;
            }
        }
    }
    catch (...)
    {
        // this variant type not match, skip to the next one
    }
}

template <typename... Types>
struct nlohmann::adl_serializer<std::variant<Types...>>
{
    static void to_json(json& json, const std::variant<Types...>& variant)
    {
        std::visit(
            [&json](const auto& value) {
                if constexpr (!std::is_same<const std::monostate&,
                                            decltype(value)>())
                {
                    json = value;
                }
            },
            variant);
    }

    static void from_json(const json& json, std::variant<Types...>& varinat)
    {
        (variant_from_json<Types>(json, varinat), ...);
    }
};

namespace http::cpr
{

static constexpr auto httpipaddrvar = "HTTPIPADDRESS";

struct Http::Handler
{
  public:
    Handler() :
        ip{[](const char* var) {
            if (auto ip = std::getenv(var))
                return ip;
            throw std::runtime_error("[Http request] Please set envvar: " +
                                     std::string{var});
        }(httpipaddrvar)},
        url{[this]() { return "http://" + ip + "/js?json="; }()}
    {}

    bool getmethod(const datamap& input, std::string& output)
    {
        auto resp = ::cpr::Get(::cpr::Url{url + json(input).dump()});
        if (resp.status_code == 200)
        {
            output = resp.text;
            return true;
        }
        if (resp.status_code == 0)
        {
            throw std::runtime_error("Given host IP address is invalid");
        }
        return false;
    }

    bool getmethod(const datamap& input, datamap& output)
    {
        std::string resp;
        if (getmethod(input, resp) && !resp.empty())
        {
            auto json = json::parse(resp);
            for (const auto& [key, val] : json.items())
            {
                output.emplace(key, val);
            }
            return true;
        }
        return false;
    }

    std::string getinfo()
    {
        return "http@" + ip + " using cpr";
    }

  private:
    const std::string ip;
    const std::string url;
};

Http::Http() : handler{std::make_unique<Handler>()}
{}

Http::~Http() = default;

bool Http::get(const datamap& input, std::string& output)
{
    return handler->getmethod(input, output);
}

bool Http::get(const datamap& input, datamap& output)
{
    return handler->getmethod(input, output);
}

std::string Http::info()
{
    return handler->getinfo();
}

} // namespace http::cpr
