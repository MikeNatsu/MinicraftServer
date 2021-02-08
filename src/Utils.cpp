#include "Utils.h"

#include <cstring>
#include <cctype>

void mcplus::utils::splitString(const std::string& string,
                                const std::string& delimiter,
                                const std::function<void(const std::string&)>& function) {
    std::string m_string{string};
    const char* r_delimiter = delimiter.c_str();

    char* token = std::strtok(m_string.data(), r_delimiter);
    while (token != nullptr) {
        function(token);
        token = std::strtok(nullptr, r_delimiter);
    }
}

void mcplus::utils::splitString(const std::string& string,
                                const std::string& delimiter,
                                std::vector<std::string>& list) {
    splitString(string, delimiter, [&list](const std::string& found){list.emplace_back(found);});
}

std::string::size_type mcplus::utils::findNth(const std::string& source,
                                              const std::string& string,
                                              std::size_t nth) {
    using size_type = std::string::size_type;
    if (nth-- == 0) {
        return std::string::npos;
    }

    size_type previousIndex = 0;
    for (std::size_t i = 0; i <= nth; i++) {
        size_type currentIndex = source.find(string, previousIndex);
        if (currentIndex == std::string::npos) {
            return previousIndex != 0 ? previousIndex : std::string::npos;
        } else if (i == nth) {
            return currentIndex;
        }

        previousIndex = currentIndex;
    }

    return std::string::npos;
}

std::string& mcplus::utils::toLower(std::string& string) {
    std::transform(string.begin(), string.end(), string.begin(), [](unsigned char c){ return std::tolower(c); });
    return string;
}
