#include "addworkdialog.h"
#include "ui_addworkdialog.h"
#include "validators.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QPushButton>

AddWorkDialog::AddWorkDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::AddWorkDialog)
{
    ui->setupUi(this);
    setWindowTitle("Добавить тип работ");

    setupValidators();

    // Подключаем сигналы
    connect(ui->nameEdit, &QLineEdit::textChanged, this, &AddWorkDialog::validateInputs);
    connect(ui->basePayEdit, &QLineEdit::textChanged, this, &AddWorkDialog::validateInputs);
    connect(ui->bonusPercentEdit, &QLineEdit::textChanged, this, &AddWorkDialog::validateInputs);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &AddWorkDialog::onAccept);

    validateInputs();
}

AddWorkDialog::~AddWorkDialog() {
    delete ui;
}

void AddWorkDialog::setupValidators() {
    // Валидатор для базовой оплаты (0-1000000 с 2 знаками после запятой)
    QDoubleValidator *basePayValidator = new QDoubleValidator(0.01, 1000000.0, 2, this);
    basePayValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->basePayEdit->setValidator(basePayValidator);

    // Валидатор для процента надбавки (0-100 с 1 знаком)
    QDoubleValidator *bonusValidator = new QDoubleValidator(0.0, 100.0, 1, this);
    bonusValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->bonusPercentEdit->setValidator(bonusValidator);
}

QString AddWorkDialog::getName() const {
    return ui->nameEdit->text().trimmed();
}

double AddWorkDialog::getBasePay() const {
    return ui->basePayEdit->text().replace(',', '.').toDouble();
}

double AddWorkDialog::getBonusPercent() const {
    QString text = ui->bonusPercentEdit->text();
    if (text.isEmpty()) return 0.0;
    return text.replace(',', '.').toDouble();
}

void AddWorkDialog::validateInputs() {
    bool isValid = true;

    // Проверка названия
    QString name = getName();
    if (name.isEmpty()) {
        isValid = false;
    }

    // Проверка базовой оплаты
    QString basePayText = ui->basePayEdit->text();
    if (basePayText.isEmpty()) {
        isValid = false;
    } else {
        double basePay = getBasePay();
        if (!Validators::isValidBasePay(basePay)) {
            isValid = false;
        }
    }

    // Проверка процента надбавки
    QString bonusText = ui->bonusPercentEdit->text();
    if (!bonusText.isEmpty()) {
        double bonus = getBonusPercent();
        if (!Validators::isValidBonusPercent(bonus)) {
            isValid = false;
        }
    }

    ui->buttonBox->button(QDialogButtonBox::Ok)->setEnabled(isValid);
}

void AddWorkDialog::onAccept() {
    // Дополнительные проверки
    QString name = getName();
    double basePay = getBasePay();
    double bonusPercent = getBonusPercent();

    if (!Validators::isValidName(name.toStdString())) {
        QMessageBox::warning(this, "Ошибка", "Название не может быть пустым или содержать только пробелы");
        return;
    }

    accept();
}
