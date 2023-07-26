#ifndef EXCEPTION_HPP
#define EXCEPTION_HPP

#include <stdexcept>

namespace cppgo::core {
class IllegalMoveException : public std::exception {
    const char* what() const noexcept { return "Illegal move"; }
};

class IllegalColorException : public std::exception {
    const char* what() const noexcept { return "Illegal color"; }
};
}  // namespace cppgo::core

#endif