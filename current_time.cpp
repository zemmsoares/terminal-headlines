using sysclock_t = std::chrono::system_clock;

std::string CurrentDate()
{
    std::time_t now = sysclock_t::to_time_t(sysclock_t::now());

    char buf[30] = { 0 };
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    
    return std::string(buf);
}
