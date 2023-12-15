#include <coek/util/option_cache.hpp>

namespace coek {

//
// OptionCacheRepn
//

class OptionCacheRepn {
   public:
    std::map<std::string, std::string> string_options;
    std::map<std::string, int> integer_options;
    std::map<std::string, double> double_options;
};

//
// OptionCache
//

OptionCache::OptionCache() { options = std::make_shared<OptionCacheRepn>(); }

std::map<std::string, std::string>& OptionCache::string_options()
{
    return options->string_options;
}

const std::map<std::string, std::string>& OptionCache::string_options() const
{
    return options->string_options;
}

std::map<std::string, int>& OptionCache::integer_options()
{
    return options->integer_options;
}

const std::map<std::string, int>& OptionCache::integer_options() const
{
    return options->integer_options;
}

std::map<std::string, double>& OptionCache::double_options()
{
    return options->double_options;
}

const std::map<std::string, double>& OptionCache::double_options() const
{
    return options->double_options;
}

bool OptionCache::get_option(const std::string& option, int& value) const
{
    auto it = options->integer_options.find(option);
    if (it == options->integer_options.end()) return false;
    value = it->second;
    return true;
}

bool OptionCache::get_option(const std::string& option, double& value) const
{
    auto it = options->double_options.find(option);
    if (it == options->double_options.end()) return false;
    value = it->second;
    return true;
}

bool OptionCache::get_option(const std::string& option, std::string& value) const
{
    auto it = options->string_options.find(option);
    if (it == options->string_options.end()) return false;
    value = it->second;
    return true;
}

void OptionCache::set_option(const std::string& option, int value)
{
    options->integer_options[option] = value;
}

void OptionCache::set_option(const std::string& option, double value)
{
    options->double_options[option] = value;
}

void OptionCache::set_option(const std::string& option, const std::string value)
{
    options->string_options[option] = value;
}

}  // namespace coek
