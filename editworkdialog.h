#ifndef EDITWORKDIALOG_H
#define EDITWORKDIALOG_H

#include <QDialog>

namespace Ui {
class EditWorkDialog;
}

class EditWorkDialog : public QDialog
{
    Q_OBJECT

public:
    // Конструктор для редактирования с переданными значениями
    explicit EditWorkDialog(const QString& name, double basePay, double bonusPercent,
                            QWidget *parent = nullptr);
    ~EditWorkDialog();

    QString getName() const;
    double getBasePay() const;
    double getBonusPercent() const;

private slots:
    void validateInputs();
    void onAccept();

private:
    Ui::EditWorkDialog *ui;
    void setupValidators();
};

#endif // EDITWORKDIALOG_H
