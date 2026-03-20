#ifndef WORKTYPE_H
#define WORKTYPE_H

#include <string>
#include <memory>
#include "bonusstrategy.h"

class IWorkType {
public:
    virtual ~IWorkType() = default;
    virtual std::string getName() const = 0;
    virtual double getBasePay() const = 0;
    virtual double getFinalPay() const = 0;
    virtual double getBonusPercent() const = 0;
};

class WorkTypeBase : public IWorkType {
private:
    std::string name;
    double basePay;
    std::shared_ptr<IBonusStrategy> bonusStrategy;

public:
    WorkTypeBase(const std::string& name,
                 double basePay,
                 std::shared_ptr<IBonusStrategy> strategy);

    std::string getName() const override;
    double getBasePay() const override;
    double getFinalPay() const override;
    double getBonusPercent() const override;

    void setName(const std::string& newName) { name = newName; }
    void setBasePay(double newBasePay) { basePay = newBasePay; }
    void setBonusStrategy(std::shared_ptr<IBonusStrategy> strategy) {
        bonusStrategy = strategy;
    }
};

#endif // WORKTYPE_H
