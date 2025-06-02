#include <exception>
#include <string>
#include <cxxabi.h>
#include <memory>

#define GERROR(msg) std::runtime_error(std::string(__FILE__) + ':' + std::to_string(__LINE__) + ": " + std::string(msg))

inline std::string demangle(const char* name) {
    int status = 0;
    std::unique_ptr<char, void(*)(void*)> res {
        abi::__cxa_demangle(name, nullptr, nullptr, &status),
        std::free
    };
    return (status == 0) ? res.get() : name;
}

#define __CLASS_NAME__ demangle(typeid(*this).name())

#if DEBUG
#define IF_DEBUG(A) A
#else
#define IF_DEBUG(A)
#endif

#if DEBUG
#define CONSTRUCTOR(msg) std::cout << __CLASS_NAME__ + " " + msg
#else
#define CONSTRUCTOR(msg)
#endif