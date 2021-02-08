#include "MinicraftDef.h"
#include <cstdlib>
#include <cstring>

mcplus::VersionPack::VersionPack(const std::string &stringVersion) : version(0), major(0), minor(0) {
    *this = stringVersion;
}

mcplus::VersionPack::VersionPack(int version, int major, int minor) : version(version), major(major), minor(minor) {

}

mcplus::VersionPack::VersionPack() : VersionPack(2, 0, 6) {

}

mcplus::VersionPack& mcplus::VersionPack::operator=(const VersionPack& anotherVersion) = default;

// x.y.z(-suffix)
mcplus::VersionPack &mcplus::VersionPack::operator=(const std::string &stringVersion) {
    char* token = strtok(const_cast<char*>(stringVersion.c_str()), ".");
    for (auto* pint : {&this->version, &this->major, &this->minor}) {
        *pint = strtol(token, nullptr, 10);
        token = strtok(nullptr, ".");
    }
    return *this;
}

mcplus::VersionPack::operator std::string() const {
    return std::to_string(version) + "." + std::to_string(major) + "." + std::to_string(minor);
}

mcplus::Color::Color(int8_t alpha, int8_t red, int8_t green, int8_t blue) {
    this->alpha = alpha;
    this->red   = red;
    this->green = green;
    this->blue  = blue;
}

mcplus::Color::Color(int32_t raw) {
    this->alpha = (raw >> 24) & 0xFF;
    this->red   = (raw >> 16) & 0xFF;
    this->green = (raw >> 8)  & 0xFF;
    this->blue  = (raw     )  & 0xFF;
}

mcplus::Color::Color() {
    this->alpha = 0;
    this->red   = 0;
    this->green = 0;
    this->blue  = 0;
}

int32_t mcplus::Color::raw() const {
    return (alpha << 24) | (red << 16) | (green << 8) | blue;
}

mcplus::FixedLocation::FixedLocation(int32_t x, uint32_t xDecimal, int32_t y, uint32_t yDecimal) {
    this->x        = x;
    this->xDecimal = xDecimal;
    this->y        = y;
    this->yDecimal = yDecimal;
}

mcplus::FixedLocation::FixedLocation(int32_t packed_x, int32_t packed_y) {
    this->x        = packed_x >> 4;
    this->xDecimal = packed_x & 0xF;
    this->y        = packed_y >> 4;
    this->yDecimal = packed_y & 0xF;
}

mcplus::FixedLocation::FixedLocation() {
    this->x        = 0;
    this->xDecimal = 0;
    this->y        = 0;
    this->yDecimal = 0;
}

mcplus::FixedLocation mcplus::FixedLocation::operator+(const mcplus::FixedLocation& another) const {
    return mcplus::FixedLocation{x + another.x, static_cast<uint32_t>(xDecimal + another.xDecimal),
                                 y + another.y, static_cast<uint32_t>(yDecimal + another.yDecimal)};
}
