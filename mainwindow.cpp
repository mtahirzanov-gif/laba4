#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "addworkdialog.h"
#include "editworkdialog.h"
#include "exceptions.h"
#include "validators.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>
#include <QHeaderView>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , currentFile("")
{
    ui->setupUi(this);
    setupTable();

    // Подключаем сигналы
    connect(ui->addButton, &QPushButton::clicked, this, &MainWindow::onAddButtonClicked);
    connect(ui->editButton, &QPushButton::clicked, this, &MainWindow::onEditButtonClicked);
    connect(ui->deleteButton, &QPushButton::clicked, this, &MainWindow::onDeleteButtonClicked);
    connect(ui->saveButton, &QPushButton::clicked, this, &MainWindow::onSaveButtonClicked);
    connect(ui->loadButton, &QPushButton::clicked, this, &MainWindow::onLoadButtonClicked);
    connect(ui->sortNameButton, &QPushButton::clicked, this, &MainWindow::onSortByNameClicked);
    connect(ui->sortBasePayButton, &QPushButton::clicked, this, &MainWindow::onSortByBasePayClicked);
    connect(ui->sortFinalPayButton, &QPushButton::clicked, this, &MainWindow::onSortByFinalPayClicked);
    connect(ui->calculateAverageButton, &QPushButton::clicked, this, &MainWindow::onCalculateAverageClicked);
    connect(ui->calculateTotalButton, &QPushButton::clicked, this, &MainWindow::onCalculateTotalClicked);
    connect(ui->aboutButton, &QPushButton::clicked, this, &MainWindow::onAboutClicked);
    connect(ui->tableWidget, &QTableWidget::itemDoubleClicked, this, &MainWindow::onTableItemDoubleClicked);
    connect(ui->searchEdit, &QLineEdit::textChanged, this, &MainWindow::onSearchTextChanged);

    // Обновляем статистику
    updateStatistics();

    setWindowTitle("Система расчета зарплаты");
}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::setupTable() {
    ui->tableWidget->setColumnCount(4);
    QStringList headers;
    headers << "Название" << "Базовая оплата" << "Надбавка %" << "Итоговая оплата";
    ui->tableWidget->setHorizontalHeaderLabels(headers);

    // Настройка внешнего вида таблицы
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableWidget->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->setVisible(false);

    // Установка ширины столбцов
    ui->tableWidget->setColumnWidth(0, 200);
    ui->tableWidget->setColumnWidth(1, 150);
    ui->tableWidget->setColumnWidth(2, 100);
}

void MainWindow::updateTable() {
    ui->tableWidget->setRowCount(0);

    const auto& workTypes = department.getAllWorkTypes();
    for (size_t i = 0; i < workTypes.size(); ++i) {
        const auto& wt = workTypes[i];

        int row = ui->tableWidget->rowCount();
        ui->tableWidget->insertRow(row);

        // Название
        QTableWidgetItem *nameItem = new QTableWidgetItem(QString::fromStdString(wt->getName()));
        nameItem->setData(Qt::UserRole, QVariant::fromValue(i)); // Сохраняем индекс
        ui->tableWidget->setItem(row, 0, nameItem);

        // Базовая оплата
        QTableWidgetItem *baseItem = new QTableWidgetItem(QString::number(wt->getBasePay(), 'f', 2));
        baseItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 1, baseItem);

        // Надбавка %
        QTableWidgetItem *bonusItem = new QTableWidgetItem(QString::number(wt->getBonusPercent(), 'f', 1));
        bonusItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 2, bonusItem);

        // Итоговая оплата
        QTableWidgetItem *finalItem = new QTableWidgetItem(QString::number(wt->getFinalPay(), 'f', 2));
        finalItem->setTextAlignment(Qt::AlignRight | Qt::AlignVCenter);
        ui->tableWidget->setItem(row, 3, finalItem);
    }

    updateStatistics();
}

void MainWindow::updateStatistics() {
    int count = department.getWorkTypeCount();
    ui->statusLabel->setText(QString("Количество записей: %1").arg(count));

    // Обновляем кнопки в зависимости от наличия данных
    bool hasData = count > 0;
    ui->editButton->setEnabled(hasData);
    ui->deleteButton->setEnabled(hasData);
    ui->calculateAverageButton->setEnabled(hasData);
    ui->calculateTotalButton->setEnabled(hasData);
    ui->sortNameButton->setEnabled(hasData);
    ui->sortBasePayButton->setEnabled(hasData);
    ui->sortFinalPayButton->setEnabled(hasData);
}

void MainWindow::onAddButtonClicked() {
    AddWorkDialog dialog(this);
    if (dialog.exec() == QDialog::Accepted) {
        try {
            std::string name = dialog.getName().toStdString();
            double basePay = dialog.getBasePay();
            double bonusPercent = dialog.getBonusPercent();

            department.addWorkType(name, basePay, bonusPercent);
            updateTable();

            showInfoMessage("Тип работ успешно добавлен");
        } catch (const PayrollException& e) {
            showErrorMessage(QString::fromStdString(e.what()));
        } catch (...) {
            showErrorMessage("Неизвестная ошибка при добавлении");
        }
    }
}

void MainWindow::onEditButtonClicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        showErrorMessage("Выберите запись для редактирования");
        return;
    }

    auto wt = department.getWorkType(currentRow);
    if (!wt) return;

    EditWorkDialog dialog(
        QString::fromStdString(wt->getName()),
        wt->getBasePay(),
        wt->getBonusPercent(),
        this
        );

    if (dialog.exec() == QDialog::Accepted) {
        try {
            std::string name = dialog.getName().toStdString();
            double basePay = dialog.getBasePay();
            double bonusPercent = dialog.getBonusPercent();

            if (department.updateWorkType(currentRow, name, basePay, bonusPercent)) {
                updateTable();
                showInfoMessage("Запись успешно обновлена");
            }
        } catch (const PayrollException& e) {
            showErrorMessage(QString::fromStdString(e.what()));
        } catch (...) {
            showErrorMessage("Неизвестная ошибка при обновлении");
        }
    }
}

void MainWindow::onDeleteButtonClicked() {
    int currentRow = ui->tableWidget->currentRow();
    if (currentRow < 0) {
        showErrorMessage("Выберите запись для удаления");
        return;
    }

    if (confirmAction("Вы уверены, что хотите удалить выбранную запись?")) {
        if (department.removeWorkType(currentRow)) {
            updateTable();
            showInfoMessage("Запись успешно удалена");
        }
    }
}

void MainWindow::onSaveButtonClicked() {
    QString fileName = QFileDialog::getSaveFileName(this,
                                                    "Сохранить данные", currentFile.isEmpty() ? "payroll_data.csv" : currentFile,
                                                    "CSV файлы (*.csv);;Все файлы (*.*)");

    if (!fileName.isEmpty()) {
        if (department.saveToFile(fileName.toStdString())) {
            currentFile = fileName;
            showInfoMessage(QString("Данные сохранены в файл: %1").arg(fileName));
        } else {
            showErrorMessage("Ошибка при сохранении файла");
        }
    }
}

void MainWindow::onLoadButtonClicked() {
    if (!department.getAllWorkTypes().empty()) {
        if (!confirmAction("Текущие данные будут потеряны. Продолжить?")) {
            return;
        }
    }

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    "Загрузить данные", "", "CSV файлы (*.csv);;Все файлы (*.*)");

    if (!fileName.isEmpty()) {
        loadDataFromFile(fileName);
    }
}

void MainWindow::loadDataFromFile(const QString &fileName) {
    if (department.loadFromFile(fileName.toStdString())) {
        currentFile = fileName;
        updateTable();
        showInfoMessage(QString("Данные загружены из файла: %1").arg(fileName));
    } else {
        showErrorMessage("Ошибка при загрузке файла");
    }
}

void MainWindow::onSortByNameClicked() {
    department.sortByName(true);
    updateTable();
    showInfoMessage("Данные отсортированы по названию");
}

void MainWindow::onSortByBasePayClicked() {
    department.sortByBasePay(true);
    updateTable();
    showInfoMessage("Данные отсортированы по базовой оплате");
}

void MainWindow::onSortByFinalPayClicked() {
    department.sortByFinalPay(true);
    updateTable();
    showInfoMessage("Данные отсортированы по итоговой оплате");
}

void MainWindow::onCalculateAverageClicked() {
    try {
        double avg = department.calculateAveragePay();
        QMessageBox::information(this, "Средняя оплата",
                                 QString("Средняя величина оплаты: %1").arg(avg, 0, 'f', 2));
    } catch (const PayrollException& e) {
        showErrorMessage(QString::fromStdString(e.what()));
    }
}

void MainWindow::onCalculateTotalClicked() {
    try {
        double total = department.calculateTotalPay();
        QMessageBox::information(this, "Общая оплата",
                                 QString("Общая сумма оплаты: %1").arg(total, 0, 'f', 2));
    } catch (...) {
        showErrorMessage("Ошибка при расчете общей суммы");
    }
}

void MainWindow::onAboutClicked() {
    QMessageBox::about(this, "О программе",
                       "Система расчета зарплаты\n"
                       "Версия 1.0\n\n"
                       "Лабораторная работа №4\n"
                       "Портирование консольного приложения в Windows Form\n\n"
                       "Функции:\n"
                       "- Добавление, редактирование, удаление типов работ\n"
                       "- Расчет надбавок и итоговых выплат\n"
                       "- Сортировка данных\n"
                       "- Сохранение и загрузка из файлов\n"
                       "- Валидация вводимых данных");
}

void MainWindow::onTableItemDoubleClicked(QTableWidgetItem *item) {
    onEditButtonClicked();
}

void MainWindow::onSearchTextChanged(const QString &text) {
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        QTableWidgetItem *item = ui->tableWidget->item(i, 0);
        bool match = text.isEmpty() ||
                     item->text().contains(text, Qt::CaseInsensitive);
        ui->tableWidget->setRowHidden(i, !match);
    }
}

void MainWindow::showErrorMessage(const QString &message) {
    QMessageBox::critical(this, "Ошибка", message);
}

void MainWindow::showInfoMessage(const QString &message) {
    QMessageBox::information(this, "Информация", message);
}

bool MainWindow::confirmAction(const QString &message) {
    return QMessageBox::question(this, "Подтверждение", message,
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes;
}

void MainWindow::clearSelection() {
    ui->tableWidget->clearSelection();
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if (!department.getAllWorkTypes().empty()) {
        if (confirmAction("Сохранить данные перед выходом?")) {
            onSaveButtonClicked();
        }
    }
    event->accept();
}
