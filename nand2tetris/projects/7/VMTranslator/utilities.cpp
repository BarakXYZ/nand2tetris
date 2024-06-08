#include "utilities.h"

int stringViewToInt(std::string_view str) {
    int result = 0;
    auto [ptr, ec] = std::from_chars(str.data(), str.data() + str.size(), result);
    if (ec == std::errc()) {
        return result; // Conversion successful
    } else {
        throw std::invalid_argument("Invalid integer format");
    }
}
