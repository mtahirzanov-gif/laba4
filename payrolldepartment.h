#ifndef PAYROLLDEPARTMENT_H
#define PAYROLLDEPARTMENT_H

#include <vector>
#include <memory>
#include <string>
#include "worktype.h"

class PayrollDepartment {
private:
    std::vector<std::shared_ptr<IWorkType>> workTypes;

    bool existsWorkType(const std::string& name) const;
    bool existsWorkType(const std::string& name, int excludeIndex) const;

public:
    PayrollDepartment() = default;

    // CRUD операции
    void addWorkType(const std::string& name, double basePay, double bonusPercent = 0.0);
    bool removeWorkType(int index);
    bool updateWorkType(int index, const std::string& name, double basePay, double bonusPercent);

    // Поиск и доступ
    std::shared_ptr<IWorkType> getWorkType(int index) const;
    int getWorkTypeCount() const { return workTypes.size(); }

    // Расчеты
    double calculateAveragePay() const;
    double calculateTotalPay() const;

    // Работа с файлами
    bool saveToFile(const std::string& filename) const;
    bool loadFromFile(const std::string& filename);

    // Сортировка
    void sortByName(bool ascending = true);
    void sortByBasePay(bool ascending = true);
    void sortByFinalPay(bool ascending = true);

    // Получение всех типов работ (для отображения)
    const std::vector<std::shared_ptr<IWorkType>>& getAllWorkTypes() const { return workTypes; }

    // Очистка
    void clear() { workTypes.clear(); }
};

#endif // PAYROLLDEPARTMENT_H
