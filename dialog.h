#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtSql/QSqlDatabase>
#include <QtSql/QSqlQuery>
#include <QtSql/QSqlRecord>
#include <QString>
#include <QMessageBox>
#include <QDebug>
#include "widget.h"

namespace Ui {
class Dialog;
}

class Dialog : public QDialog
{
    Q_OBJECT

public:
    explicit Dialog(QWidget *parent = 0);
    ~Dialog();
    QString getAccount();
    QString getPassword();

private slots:
    void on_loginButton_clicked();
    void on_applyButton_clicked();
    void on_exitButton_clicked();
    void on_submitButton_clicked();
    void on_cancelButton_clicked();

private:
    void setUi();
    void setWindow();

private:
    Ui::Dialog *ui;
    int sign;
    QString account;
    QString password;
    QString newAccount;
    QString newPassword;
    QString checkAccount;
    QString checkPassword;

    friend class Widget;
};

#endif // DIALOG_H
