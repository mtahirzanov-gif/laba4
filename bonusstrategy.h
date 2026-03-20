#ifndef BONUSSTRATEGY_H
#define BONUSSTRATEGY_H

#include <memory>

class IBonusStrategy {
public:
    virtual ~IBonusStrategy() = default;
    virtual double computePay(double basePay) const = 0;
};

class NoBonusStrategy : public IBonusStrategy {
public:
    double computePay(double basePay) const override;
};

class PercentageBonusStrategy : public IBonusStrategy {
private:
    double bonusPercent;

public:
    explicit PercentageBonusStrategy(double percent);
    double computePay(double basePay) const override;

    double getBonusPercent() const { return bonusPercent; }
};

#endif // BONUSSTRATEGY_H
