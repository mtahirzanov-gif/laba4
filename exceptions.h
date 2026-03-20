#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdexcept>
#include <string>

class PayrollException : public std::runtime_error {
public:
    explicit PayrollException(const std::string& msg)
        : std::runtime_error(msg) {}
};

class InvalidRateException : public PayrollException {
public:
    explicit InvalidRateException(const std::string& msg)
        : PayrollException("Неверная ставка: " + msg) {}
};

class DuplicateWorkTypeException : public PayrollException {
public:
    explicit DuplicateWorkTypeException(const std::string& msg)
        : PayrollException("Дублирование типа работ: " + msg) {}
};

class EmptyWorkListException : public PayrollException {
public:
    explicit EmptyWorkListException(const std::string& msg)
        : PayrollException("Список работ пуст: " + msg) {}
};

class FileException : public PayrollException {
public:
    explicit FileException(const std::string& msg)
        : PayrollException("Ошибка файла: " + msg) {}
};

#endif // EXCEPTIONS_H
