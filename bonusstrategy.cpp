#include "bonusstrategy.h"
#include "exceptions.h"

double NoBonusStrategy::computePay(double basePay) const {
    return basePay;
}

PercentageBonusStrategy::PercentageBonusStrategy(double percent)
    : bonusPercent(percent) {
    if (bonusPercent < 0) {
        throw InvalidRateException("надбавка должна быть >= 0");
    }
    if (bonusPercent > 100.0) {
        throw InvalidRateException("надбавка не может превышать 100%");
    }
}

double PercentageBonusStrategy::computePay(double basePay) const {
    return basePay * (1.0 + bonusPercent / 100.0);
}
