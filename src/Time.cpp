#include <chrono>
#include "./json.hpp"

std::string format_datetime(std::string publishedAt) {
  std::tm tm = {};
  std::istringstream ss(publishedAt);
  ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
  char buffer[20];
  //std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &tm);
  std::strftime(buffer, sizeof(buffer), "%H:%M", &tm);
  return buffer;
}

using sysclock_t = std::chrono::system_clock;

std::string CurrentDate()
{
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());
    char buf[30] = { 0 };
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    return std::string(buf);
}
