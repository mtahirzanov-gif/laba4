#include "editworkdialog.h"
#include "ui_editworkdialog.h"
#include "validators.h"
#include <QMessageBox>
#include <QDoubleValidator>
#include <QPushButton>

EditWorkDialog::EditWorkDialog(const QString& name, double basePay, double bonusPercent,
                               QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::EditWorkDialog)
{
    ui->setupUi(this);
    setWindowTitle("Редактировать тип работ");

    // Заполняем поля переданными значениями
    ui->nameEdit->setText(name);
    ui->basePayEdit->setText(QString::number(basePay, 'f', 2));
    ui->bonusPercentEdit->setText(QString::number(bonusPercent, 'f', 1));

    setupValidators();

    // Подключаем сигналы
    connect(ui->nameEdit, &QLineEdit::textChanged, this, &EditWorkDialog::validateInputs);
    connect(ui->basePayEdit, &QLineEdit::textChanged, this, &EditWorkDialog::validateInputs);
    connect(ui->bonusPercentEdit, &QLineEdit::textChanged, this, &EditWorkDialog::validateInputs);
    connect(ui->buttonBox, &QDialogButtonBox::accepted, this, &EditWorkDialog::onAccept);

    validateInputs();
}

EditWorkDialog::~EditWorkDialog() {
    delete ui;
}

void EditWorkDialog::setupValidators() {
    // Валидатор для базовой оплаты
    QDoubleValidator *basePayValidator = new QDoubleValidator(0.01, 1000000.0, 2, this);
    basePayValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->basePayEdit->setValidator(basePayValidator);

    // Валидатор для процента надбавки
    QDoubleValidator *bonusValidator = new QDoubleValidator(0.0, 100.0, 1, this);
    bonusValidator->setNotation(QDoubleValidator::StandardNotation);
    ui->bonusPercentEdit->setValidator(bonusValidator);
}

QString EditWorkDialog::getName() const {
    return ui->nameEdit->text().trimmed();
}

double EditWorkDialog::getBasePay() const {
    return ui->basePayEdit->text().replace(',', '.').toDouble();
}

double EditWorkDialog::getBonusPercent() const {
    QString text = ui->bonusPercentEdit->text();
    if (text.isEmpty()) return 0.0;
    return text.replace(',', '.').toDouble();
}

void EditWorkDialog::validateInputs() {
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

void EditWorkDialog::onAccept() {
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
