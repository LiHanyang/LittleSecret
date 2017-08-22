#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QStyle>
#include <QStyleFactory>
#include <QStandardItem>
#include <vector>
#include <QDebug>
#include <QInputDialog>
#include "dialog.h"

using namespace std;

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void getAccount(QString account);
    void getPassword(QString password);
    void firstShow();

private slots:
    void on_exitButton_clicked();
    void on_rootAddButton_clicked();
    void on_rootDeleteButton_clicked();
    void on_accountAddButton_clicked();
    void on_accountDeleteButton_clicked();
    void on_rootTable_clicked(const QModelIndex &index);
    void on_accountEdit_textChanged(const QString &arg1);
    void on_passwordEdit_textChanged(const QString &arg1);
    void on_urlEdit_textChanged(const QString &arg1);
    void on_accountTable_clicked(const QModelIndex &index);

    void on_saveBuuton_clicked();

private:
    void setWindow();
    void setUi();
    void setRootTable();
    void setAccountTable();
    int checkPassword(const QString& password);

private:
    Ui::Widget *ui;
    QString userAccount;
    QString userPassword;
    QStandardItemModel *accountItemModel;
    QStandardItemModel *rootItemModel;
    int rootCount;
    int accountCount;
    QString catalogue_1;
    QString catalogue_2;

    friend class Dialog;
};

#endif // WIDGET_H
