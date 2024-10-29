#include "http/interfaces/cpr.hpp"

#include <cpr/cpr.h>

#include <nlohmann/json.hpp>

#include <cstdlib>

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
        // need monostate when type from json is not recognized, no throw
    }
    catch (...)
    {
        // this variant type does not match, skip to the next one
    }
}

template <typename... Types>
struct nlohmann::adl_serializer<std::variant<Types...>>
{
    static void to_json(json& json, const std::variant<Types...>& variant)
    {
        json = std::visit(
            [](const auto& value) -> nlohmann::json {
                if constexpr (!std::is_same<const std::monostate&,
                                            decltype(value)>())
                {
                    return value;
                }
                throw std::runtime_error("Wrong json input (monostate)");
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
    Handler(std::shared_ptr<logging::LogIf> logIf) :
        logIf{logIf}, ip{[this](const char* var) {
            if (auto ip = std::getenv(var))
                return ip;
            log(logging::type::critical, "IP address not provided");
            throw std::runtime_error("[Http request] Please set envvar: " +
                                     std::string{var});
        }(httpipaddrvar)},
        url{[this]() { return "http://" + ip + "/js?json="; }()}
    {
        log(logging::type::debug, "IP address: " + ip);
        log(logging::type::debug, "Http url: " + url);
    }

    bool getmethod(const inputtype& input, std::string& output)
    {
        return get(json(input).dump(), output);
    }

    bool getmethod(const std::string& input, std::string& output)
    {
        return get(json::parse(input).dump(), output);
    }

    bool getmethod(const inputtype& input, outputtype& output)
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
    const std::string module{"libhttp"};
    std::shared_ptr<logging::LogIf> logIf;
    const std::string ip;
    const std::string url;

    bool get(const std::string jsonstr, std::string& output)
    {
        auto requesturl = url + jsonstr;
        log(logging::type::debug, "Requested json: " + jsonstr);
        log(logging::type::debug, "Requested url: " + requesturl);
        auto resp = ::cpr::Get(::cpr::Url{requesturl});
        log(logging::type::debug,
            "Status code: " + std::to_string(resp.status_code));
        if (resp.status_code == 200)
        {
            log(logging::type::debug, "Response data: " + resp.text);
            output = resp.text;
            return true;
        }
        if (resp.status_code == 0)
        {
            log(logging::type::critical, "Given IP address invalid");
            throw std::runtime_error("Given host IP address is invalid");
        }
        return false;
    }

    void log(logging::type type, const std::string& msg)
    {
        if (logIf)
        {
            logIf->log(type, module, msg);
        }
    }
};

Http::Http(std::shared_ptr<logging::LogIf> logIf) :
    handler{std::make_unique<Handler>(logIf)}
{}

Http::~Http() = default;

bool Http::get(const std::string& input, std::string& output)
{
    return handler->getmethod(input, output);
}

bool Http::get(const inputtype& input, std::string& output)
{
    return handler->getmethod(input, output);
}

bool Http::get(const inputtype& input, outputtype& output)
{
    return handler->getmethod(input, output);
}

std::string Http::info()
{
    return handler->getinfo();
}

} // namespace http::cpr
