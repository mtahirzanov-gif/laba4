#include "payrolldepartment.h"
#include "exceptions.h"
#include "bonusstrategy.h"
#include <fstream>
#include <sstream>
#include <algorithm>
#include <iomanip>

bool PayrollDepartment::existsWorkType(const std::string& name) const {
    for (const auto& w : workTypes) {
        if (w->getName() == name) {
            return true;
        }
    }
    return false;
}

bool PayrollDepartment::existsWorkType(const std::string& name, int excludeIndex) const {
    for (size_t i = 0; i < workTypes.size(); ++i) {
        if (i != excludeIndex && workTypes[i]->getName() == name) {
            return true;
        }
    }
    return false;
}

void PayrollDepartment::addWorkType(const std::string& name, double basePay, double bonusPercent) {
    if (name.size() > 50) {
        // Можно добавить логгирование
    }

    if (existsWorkType(name)) {
        throw DuplicateWorkTypeException("тип работ '" + name + "' уже существует");
    }

    std::shared_ptr<IBonusStrategy> strategy;
    if (bonusPercent == 0.0) {
        strategy = std::make_shared<NoBonusStrategy>();
    } else {
        strategy = std::make_shared<PercentageBonusStrategy>(bonusPercent);
    }

    auto wt = std::make_shared<WorkTypeBase>(name, basePay, strategy);
    workTypes.push_back(wt);
}

bool PayrollDepartment::removeWorkType(int index) {
    if (index >= 0 && index < workTypes.size()) {
        workTypes.erase(workTypes.begin() + index);
        return true;
    }
    return false;
}

bool PayrollDepartment::updateWorkType(int index, const std::string& name,
                                       double basePay, double bonusPercent) {
    if (index < 0 || index >= workTypes.size()) {
        return false;
    }

    if (existsWorkType(name, index)) {
        throw DuplicateWorkTypeException("тип работ '" + name + "' уже существует");
    }

    std::shared_ptr<IBonusStrategy> strategy;
    if (bonusPercent == 0.0) {
        strategy = std::make_shared<NoBonusStrategy>();
    } else {
        strategy = std::make_shared<PercentageBonusStrategy>(bonusPercent);
    }

    auto wt = std::make_shared<WorkTypeBase>(name, basePay, strategy);
    workTypes[index] = wt;
    return true;
}

std::shared_ptr<IWorkType> PayrollDepartment::getWorkType(int index) const {
    if (index >= 0 && index < workTypes.size()) {
        return workTypes[index];
    }
    return nullptr;
}

double PayrollDepartment::calculateAveragePay() const {
    if (workTypes.empty()) {
        throw EmptyWorkListException("невозможно вычислить среднее значение");
    }

    double sum = 0.0;
    for (const auto& w : workTypes) {
        sum += w->getFinalPay();
    }
    return sum / static_cast<double>(workTypes.size());
}

double PayrollDepartment::calculateTotalPay() const {
    double sum = 0.0;
    for (const auto& w : workTypes) {
        sum += w->getFinalPay();
    }
    return sum;
}

bool PayrollDepartment::saveToFile(const std::string& filename) const {
    std::ofstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Заголовок CSV
    file << "Название,Базовая оплата,Надбавка %,Итоговая оплата\n";

    for (const auto& w : workTypes) {
        file << w->getName() << ","
             << std::fixed << std::setprecision(2) << w->getBasePay() << ","
             << w->getBonusPercent() << ","
             << w->getFinalPay() << "\n";
    }

    file.close();
    return true;
}

bool PayrollDepartment::loadFromFile(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        return false;
    }

    // Очищаем текущие данные
    workTypes.clear();

    std::string line;
    // Пропускаем заголовок
    std::getline(file, line);

    while (std::getline(file, line)) {
        if (line.empty()) continue;

        std::stringstream ss(line);
        std::string name, basePayStr, bonusStr, finalPayStr;

        if (std::getline(ss, name, ',') &&
            std::getline(ss, basePayStr, ',') &&
            std::getline(ss, bonusStr, ',')) {

            try {
                double basePay = std::stod(basePayStr);
                double bonusPercent = std::stod(bonusStr);

                addWorkType(name, basePay, bonusPercent);
            } catch (...) {
                // Пропускаем некорректные строки
            }
        }
    }

    file.close();
    return true;
}

void PayrollDepartment::sortByName(bool ascending) {
    std::sort(workTypes.begin(), workTypes.end(),
              [ascending](const std::shared_ptr<IWorkType>& a,
                          const std::shared_ptr<IWorkType>& b) {
                  if (ascending) {
                      return a->getName() < b->getName();
                  } else {
                      return a->getName() > b->getName();
                  }
              });
}

void PayrollDepartment::sortByBasePay(bool ascending) {
    std::sort(workTypes.begin(), workTypes.end(),
              [ascending](const std::shared_ptr<IWorkType>& a,
                          const std::shared_ptr<IWorkType>& b) {
                  if (ascending) {
                      return a->getBasePay() < b->getBasePay();
                  } else {
                      return a->getBasePay() > b->getBasePay();
                  }
              });
}

void PayrollDepartment::sortByFinalPay(bool ascending) {
    std::sort(workTypes.begin(), workTypes.end(),
              [ascending](const std::shared_ptr<IWorkType>& a,
                          const std::shared_ptr<IWorkType>& b) {
                  if (ascending) {
                      return a->getFinalPay() < b->getFinalPay();
                  } else {
                      return a->getFinalPay() > b->getFinalPay();
                  }
              });
}
