#include "validators.h"
#include <cctype>
#include <algorithm>

bool Validators::isInteger(const std::string& str) {
    if (str.empty()) return false;

    for (char c : str) {
        if (!std::isdigit(static_cast<unsigned char>(c))) {
            return false;
        }
    }
    return true;
}

bool Validators::isFloat(const std::string& str) {
    if (str.empty()) return false;

    bool hasDecimalPoint = false;
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];

        if (std::isdigit(static_cast<unsigned char>(c))) continue;

        if (c == '.' && !hasDecimalPoint) {
            hasDecimalPoint = true;
            continue;
        }

        return false;
    }

    return str != "." && !str.empty();
}

bool Validators::isValidName(const std::string& str) {
    if (str.empty() || str.length() > 50) {
        return false;
    }

    // Проверяем, что строка не состоит только из пробелов
    return std::any_of(str.begin(), str.end(), [](char c) {
        return !std::isspace(static_cast<unsigned char>(c));
    });
}

bool Validators::isValidBasePay(double value) {
    return value > 0 && value <= 1000000.0;
}

bool Validators::isValidBonusPercent(double value) {
    return value >= 0 && value <= 100.0;
}
