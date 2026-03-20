#ifndef ADDWORKDIALOG_H
#define ADDWORKDIALOG_H

#include <QDialog>

namespace Ui {
class AddWorkDialog;
}

class AddWorkDialog : public QDialog
{
    Q_OBJECT

public:
    explicit AddWorkDialog(QWidget *parent = nullptr);
    ~AddWorkDialog();

    QString getName() const;
    double getBasePay() const;
    double getBonusPercent() const;

private slots:
    void validateInputs();
    void onAccept();

private:
    Ui::AddWorkDialog *ui;
    void setupValidators();
};

#endif // ADDWORKDIALOG_H
