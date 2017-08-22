#include "widget.h"
#include "ui_widget.h"

extern int id;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget),
    rootItemModel(new QStandardItemModel(this)),
    accountItemModel(new QStandardItemModel(this)),
    rootCount(0),
    accountCount(0)
{
    setUi();
    setWindow();
    setRootTable();
    setAccountTable();


}

Widget::~Widget()
{
    delete ui;
}

void Widget::getAccount(QString account)
{
    userAccount = account;
}

void Widget::getPassword(QString password)
{
    userPassword = password;
}

void Widget::firstShow()
{
    QString s = QString("select * from user");
    QSqlQuery query;
    query.exec(s);
    if(query.first())
    {
        do
        {
            if((userAccount == query.value(1).toString()) && (NULL != query.value(3).toString()))
                rootItemModel->setItem(rootCount++, 0, new QStandardItem(query.value(3).toString()));
            int i = 0;
            while(i + 1 < rootCount)
            {
                if(rootItemModel->item(i)->text() == rootItemModel->item(rootCount - 1)->text())
                {
                    rootItemModel->removeRow(rootCount - 1);
                    --rootCount;
                }
                ++i;
            }
        }
        while(query.next());
    }
}

void Widget::setUi()
{
    ui->setupUi(this);
    ui->accountEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->urlEdit->setEnabled(false);
    ui->saveBuuton->setEnabled(false);
    ui->rootDeleteButton->setEnabled(false);
    ui->accountDeleteButton->setEnabled(false);
}


void Widget::setWindow()
{
    setWindowTitle(" ");
}

void Widget::setRootTable()
{
    ui->rootTable->setModel(rootItemModel);
    ui->rootTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->rootTable->horizontalHeader()->setStretchLastSection(true);
    ui->rootTable->horizontalHeader()->setVisible(false);
    ui->rootTable->verticalHeader()->setVisible(false);
    ui->rootTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Widget::setAccountTable()
{
    ui->accountTable->setModel(accountItemModel);
    ui->accountTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->accountTable->horizontalHeader()->setStretchLastSection(true);
    ui->accountTable->horizontalHeader()->setVisible(false);
    ui->accountTable->verticalHeader()->setVisible(false);
    ui->accountTable->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void Widget::on_exitButton_clicked()
{
    exit(0);
}

void Widget::on_rootAddButton_clicked()
{
    bool ok;
    catalogue_1 = QInputDialog::getText(this,"",
        tr("Please input your root name"),
        QLineEdit::Normal,"",&ok);
    if(ok)
    {
        rootItemModel->setItem(rootCount++, 0, new QStandardItem(catalogue_1));
    }
}

void Widget::on_rootDeleteButton_clicked()
{
    if(0 == rootCount) return;

    int row = ui->rootTable->currentIndex().row();
    if(row >= 0)
    {
        QString cata = rootItemModel->item(row)->text();
        QSqlQuery query;
        QString s = QString("delete from user where catalogue_1=='%1' ").arg(cata);
        query.exec(s);

        rootItemModel->removeRow(row);
        --rootCount;

        accountItemModel->removeRows(0, accountCount);
        ui->accountEdit->clear();
        ui->passwordEdit->clear();
        ui->urlEdit->clear();
    }

    ui->rootDeleteButton->setEnabled(false);
}

void Widget::on_accountAddButton_clicked()
{
    int row = ui->rootTable->currentIndex().row();
    if(row >= 0)
    {
        bool ok;
        catalogue_2 = QInputDialog::getText(this,"",
            tr("Please input your account name"),
            QLineEdit::Normal,"",&ok);
        if(ok)
        {
            accountItemModel->setItem(accountCount++, 0, new QStandardItem(catalogue_2));
        }
    }
}


void Widget::on_accountDeleteButton_clicked()
{

    int row = ui->accountTable->currentIndex().row();
    if(row >= 0)
    {
        QString cata = accountItemModel->item(row)->text();
        QSqlQuery query;
        QString s = QString("delete from user where catalogue_2=='%1' ").arg(cata);
        query.exec(s);

        accountItemModel->removeRow(row);
        --accountCount;

        ui->accountEdit->clear();
        ui->passwordEdit->clear();
        ui->urlEdit->clear();
    }

    if(0 == accountCount)
    {
        ui->accountEdit->setEnabled(false);
        ui->passwordEdit->setEnabled(false);
        ui->urlEdit->setEnabled(false);
        ui->saveBuuton->setEnabled(false);
    }

    ui->accountDeleteButton->setEnabled(false);
}

void Widget::on_rootTable_clicked(const QModelIndex &index)
{
    ui->rootDeleteButton->setEnabled(true);

    accountItemModel->clear();
    accountCount = 0;
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->urlEdit->clear();
    catalogue_1 = rootItemModel->item(index.row())->text();
    QString s = QString("select * from user");
    QSqlQuery query;
    query.exec(s);
    while(query.next())
    {
        qDebug() << "ok";
        qDebug() << query.value(3).toString();
        if((userAccount == query.value(1).toString()) && (catalogue_1 == query.value(3).toString()))
            accountItemModel->setItem(accountCount++, 0, new QStandardItem(query.value(4).toString()));
    }
}

void Widget::on_accountEdit_textChanged(const QString &arg1)
{
    //int rootRow = ui->rootTable->currentIndex().row();
    //int accountRow = ui->accountTable->currentIndex().row();
    //rootVector[rootRow][accountRow].account = arg1;

    ui->saveBuuton->setEnabled(true);
}

void Widget::on_passwordEdit_textChanged(const QString &arg1)
{
    //int rootRow = ui->rootTable->currentIndex().row();
    //int accountRow = ui->accountTable->currentIndex().row();
    //rootVector[rootRow][accountRow].password = arg1;

    ui->saveBuuton->setEnabled(true);

    qDebug() << arg1;
    qDebug() << checkPassword(arg1);
    switch(checkPassword(arg1))
    {
    case 0:
        ui->securityLabel->setText("too weak");
        break;
    case 1:
        ui->securityLabel->setText("weak");
        break;
    case 2:
        ui->securityLabel->setText("mid");
        break;
    case 3:
        ui->securityLabel->setText("strong");
        break;
    case 4:
        ui->securityLabel->setText("very strong");
        break;
    }

}

void Widget::on_urlEdit_textChanged(const QString &arg1)
{
    //int rootRow = ui->rootTable->currentIndex().row();
    //int accountRow = ui->accountTable->currentIndex().row();
    //rootVector[rootRow][accountRow].url = arg1;

    ui->saveBuuton->setEnabled(true);
}

void Widget::on_accountTable_clicked(const QModelIndex &index)
{
    if(0 != accountCount)
    {
        catalogue_2 = accountItemModel->item(index.row())->text();
        QString s = QString("select * from user");
        QSqlQuery query;
        query.exec(s);
        while(query.next())
        {
            if((userAccount == query.value(1).toString()) &&
                    (catalogue_1 == query.value(3).toString()) &&
                    (catalogue_2 == query.value(4).toString()))
            {
                QStringList list = query.value(5).toString().split("\a");
                ui->accountEdit->setText(list[0]);
                ui->passwordEdit->setText(list[1]);
                ui->urlEdit->setText(list[2]);
            }
        }
    }

    ui->accountEdit->setEnabled(true);
    ui->passwordEdit->setEnabled(true);
    ui->urlEdit->setEnabled(true);
    ui->saveBuuton->setEnabled(false);
    ui->accountDeleteButton->setEnabled(true);
}

int Widget::checkPassword(const QString& password)
{
    if(password.length() < 6) return 0;

    int upCount = 0;
    int lowCount = 0;
    int digitCount = 0;
    int otherCount = 0;
    for(auto ch : password)
    {
        if(ch.isUpper()) upCount = 1;
        else if(ch.isLower()) lowCount = 1;
        else if(ch.isDigit()) digitCount = 1;
        else if(!ch.isNull()) otherCount = 1;
    }
    return upCount + lowCount + digitCount + otherCount;
}

void Widget::on_saveBuuton_clicked()
{
    QString data = QString(" %1\a%2\a%3 ").
            arg(ui->accountEdit->text()).arg(ui->passwordEdit->text()).arg(ui->urlEdit->text());
    QString s = QString("insert into user values(%1, '%2', '%3', '%4', '%5', '%6') ").arg(++id).arg(userAccount).
            arg(userPassword).arg(catalogue_1).arg(catalogue_2).arg(data);
    QSqlQuery query;
    query.exec(s);

    ui->saveBuuton->setEnabled(false);
}
