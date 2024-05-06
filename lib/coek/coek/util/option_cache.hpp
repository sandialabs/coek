#pragma once

#include <map>
#include <memory>
#include <string>

namespace coek {

class OptionCacheRepn;

class OptionCache {
   public:
    std::shared_ptr<OptionCacheRepn> options;

   public:
    OptionCache();

    std::map<std::string, std::string>& string_options();
    const std::map<std::string, std::string>& string_options() const;

    std::map<std::string, bool>& boolean_options();
    const std::map<std::string, bool>& boolean_options() const;

    std::map<std::string, int>& integer_options();
    const std::map<std::string, int>& integer_options() const;

    std::map<std::string, double>& double_options();
    const std::map<std::string, double>& double_options() const;

    /** Get the value of a boolean option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a boolean value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& option, bool& value) const;

    /** Get the value of an integer option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   an integer value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& option, int& value) const;

    /** Get the value of a double option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a double value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& option, double& value) const;

    /** Get the value of a string option
     *
     * The option value is returned by reference if it has
     * a value.
     *
     * \param option  the option name
     * \param value   a string value that is passed by reference
     *
     * \returns \c true if the option is found
     */
    virtual bool get_option(const std::string& option, std::string& value) const;

    /** Set a boolean option
     *
     * \param option  the option name
     * \param value   the boolean value
     */
    virtual void set_option(const std::string& option, bool value);

    /** Set an integer option
     *
     * \param option  the option name
     * \param value   the integer value
     */
    virtual void set_option(const std::string& option, int value);

    /** Set a double option
     *
     * \param option  the option name
     * \param value   the double value
     */
    virtual void set_option(const std::string& option, double value);

    /** Set a string option
     *
     * \param option  the option name
     * \param value   the string value
     */
    virtual void set_option(const std::string& option, const std::string& value);

    /** Set a string option
     *
     * \param option  the option name
     * \param value   the string value
     */
    virtual void set_option(const std::string& option, const char* value);
};

}  // namespace coek
