#ifndef VALIDATORS_H
#define VALIDATORS_H

#include <string>

namespace Validators {
bool isInteger(const std::string& str);
bool isFloat(const std::string& str);
bool isValidName(const std::string& str);
bool isValidBasePay(double value);
bool isValidBonusPercent(double value);
}

#endif // VALIDATORS_H
