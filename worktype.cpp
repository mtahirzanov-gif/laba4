#include "worktype.h"
#include "exceptions.h"
#include <algorithm>

WorkTypeBase::WorkTypeBase(const std::string& name,
                           double basePay,
                           std::shared_ptr<IBonusStrategy> strategy)
    : name(name), basePay(basePay), bonusStrategy(strategy) {

    if (name.empty()) {
        throw InvalidRateException("название типа работ не может быть пустым");
    }
    if (basePay <= 0) {
        throw InvalidRateException("базовая оплата должна быть > 0");
    }
    if (basePay > 1000000.0) {
        throw InvalidRateException("базовая оплата не может превышать 1,000,000");
    }
    if (!bonusStrategy) {
        throw InvalidRateException("стратегия бонусов не может быть null");
    }
}

std::string WorkTypeBase::getName() const {
    return name;
}

double WorkTypeBase::getBasePay() const {
    return basePay;
}

double WorkTypeBase::getFinalPay() const {
    return bonusStrategy->computePay(basePay);
}

double WorkTypeBase::getBonusPercent() const {
    if (auto percStrategy = std::dynamic_pointer_cast<PercentageBonusStrategy>(bonusStrategy)) {
        return percStrategy->getBonusPercent();
    }
    return 0.0;
}
