#include <stdexcept>
namespace msghub::net {
  class NetException : public std::runtime_error {
   public:
    explicit NetException(const std::string& message)
        : std::runtime_error(message) {}

    explicit NetException(const std::string& message, int error_code)
        : std::runtime_error(
            message + " (Error code: " + std::to_string(error_code) + ")") {}

    explicit NetException(const char* message) : std::runtime_error(message) {}
  };
}  // namespace msghub::net