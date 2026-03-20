#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTableWidgetItem>
#include <QCloseEvent>
#include "payrolldepartment.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

protected:
    void closeEvent(QCloseEvent *event) override;

private slots:
    void onAddButtonClicked();
    void onEditButtonClicked();
    void onDeleteButtonClicked();
    void onSaveButtonClicked();
    void onLoadButtonClicked();
    void onSortByNameClicked();
    void onSortByBasePayClicked();
    void onSortByFinalPayClicked();
    void onCalculateAverageClicked();
    void onCalculateTotalClicked();
    void onAboutClicked();
    void onTableItemDoubleClicked(QTableWidgetItem *item);
    void onSearchTextChanged(const QString &text);

private:
    Ui::MainWindow *ui;
    PayrollDepartment department;
    QString currentFile;

    void setupTable();
    void updateTable();
    void updateStatistics();
    void loadDataFromFile(const QString &fileName);
    void saveDataToFile(const QString &fileName);
    void showErrorMessage(const QString &message);
    void showInfoMessage(const QString &message);
    bool confirmAction(const QString &message);
    void clearSelection();
};

#endif // MAINWINDOW_H
