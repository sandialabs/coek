#pragma once

#include <string>

namespace coek {

namespace exceptions {

class Exception : public std::exception {
   public:
    std::string message;

    Exception(const std::string& msg) : message(msg) {}
    Exception(const char* msg) : message(msg) {}

    const char* what() const throw() { return message.c_str(); }
};

class NonquadraticExpression : public Exception {
   public:
    NonquadraticExpression(const std::string& msg) : Exception(msg) {}
    NonquadraticExpression(const char* msg) : Exception(msg) {}
};

}  // namespace exceptions

}  // namespace coek

namespace std {

inline string to_string(const coek::exceptions::Exception& e) { return e.message; }

}  // namespace std
