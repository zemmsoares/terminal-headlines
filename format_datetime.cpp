    std::string format_datetime(std::string publishedAt) {
        std::tm tm = {};
        std::istringstream ss(publishedAt);
        ss >> std::get_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
        char buffer[20];
        std::strftime(buffer, sizeof(buffer), "%d/%m/%Y %H:%M", &tm);
        return buffer;
    }
