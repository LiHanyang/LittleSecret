#include "dialog.h"
#include "widget.h"
#include "ui_dialog.h"

extern int id;

Dialog::Dialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Dialog)
{
    setUi();
    setWindow();
}

Dialog::~Dialog()
{
    delete ui;
}

QString Dialog::getAccount()
{
    return account;
}

QString Dialog::getPassword()
{
    return password;
}

void Dialog::setUi()
{
    ui->setupUi(this);
    ui->passwordEdit->setEchoMode(QLineEdit::Password);
    ui->newPasswordEdit->setEchoMode(QLineEdit::Password);
    ui->checkPasswordEdit->setEchoMode(QLineEdit::Password);
}

void Dialog::on_loginButton_clicked()
{
    QString uname = ui->accountEdit->text();
    QString upasswd = ui->passwordEdit->text();
    QString s = QString("select * from user where name=='%1' and passwd=='%2' ").arg(uname).arg(upasswd);
    QSqlQuery query;
    query.exec(s);
    if(query.first())
    {
        accept();
        account = uname;
        password = upasswd;
    }
    else
    {
        QMessageBox::warning(this,tr("Waring"),tr("wrong account or password"),QMessageBox::Yes);
        ui->passwordEdit->clear();
        ui->accountEdit->setFocus();
    }

}

void Dialog::on_applyButton_clicked()
{
    this->resize(QSize(960, 400));

}

void Dialog::setWindow()
{
    resize(QSize(480, 400));
    setWindowTitle(" ");
    setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);
}

void Dialog::on_exitButton_clicked()
{
    exit(0);
}

void Dialog::on_submitButton_clicked()
{
    QString uname = ui->newAccountEdit->text();
    QString upasswd = ui->newPasswordEdit->text();
    QString s = QString("insert into user values(%1, '%2', '%3', NULL, NULL, NULL, NULL) ").arg(++id).arg(uname).arg(upasswd);
    QSqlQuery query;
    bool ok = query.exec(s);
    QMessageBox box;
    if(ok)
    {
        box.setText("register success");
        box.exec();
        ui->newAccountEdit->clear();
        ui->newPasswordEdit->clear();
        ui->checkPasswordEdit->clear();
        ui->newAccountEdit->setFocus();
        resize(QSize(480, 400));
    }

}

void Dialog::on_cancelButton_clicked()
{
    resize(QSize(480, 400));

}
