#ifndef MINICRAFTSERVER_UTILS_H
#define MINICRAFTSERVER_UTILS_H

#include <string>
#include <functional>
#include <vector>

namespace mcplus::utils {

    void splitString(const std::string& string,
                     const std::string& delimiter,
                     const std::function<void(const std::string&)>& function);

    void splitString(const std::string& string,
                     const std::string& delimiter,
                     std::vector<std::string>& list);

    std::string::size_type findNth(const std::string& source,
                                   const std::string& string,
                                   std::size_t nth);

    std::string& toLower(std::string& string);

}

#endif // MINICRAFTSERVER_UTILS_H