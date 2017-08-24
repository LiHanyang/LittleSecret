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
    accountItemModel->clear();
    rootCount = 0;
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
    ui->searchCata_2->setEnabled(false);
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
    QString cata = QInputDialog::getText(this,"",
        tr("Please input your root name"),
        QLineEdit::Normal,"",&ok);
    QSqlQuery query;
    QString s = QString("select * from user where name=='%1' ").arg(userAccount);
    query.exec(s);
    query.first();
    isRepeated(rootItemModel, rootCount, catalogue_1, cata);

    if(ok)
    {
        rootItemModel->setItem(rootCount++, 0, new QStandardItem(catalogue_1));
    }
}

void Widget::isRepeated(QStandardItemModel *itemModel, int nCount, QString &catalogue, QString &cata)
{
    int count = 0;
    for(int i = 0; i < nCount; i++)
    {
        if(itemModel->item(i)->text() != cata)
        {
            continue;
        }
        ++count;
        break;
    }
    if(0 == count)
    {
        catalogue = cata;
    }
    else
    {
        int i = 0;
        while(true)
        {
            QString temp = QString("%1 (%2)").arg(cata).arg(i + 2);
            int tempCount = 0;
            for(int j = 0; j < nCount; j++)
            {
                if(itemModel->item(j)->text() != temp)
                {
                    continue;
                }
                ++tempCount;
                break;
            }
            if(0 == tempCount)
            {
                catalogue = temp;
                break;
            }
            ++i;
        }
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
        QString cata = QInputDialog::getText(this,"",
            tr("Please input your account name"),
            QLineEdit::Normal,"",&ok);
        isRepeated(accountItemModel, accountCount, catalogue_2, cata);

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
    ui->searchCata_2->clear();
    ui->rootDeleteButton->setEnabled(true);
    if(index.row() >= 0)
        ui->searchCata_2->setEnabled(true);

    accountItemModel->clear();
    accountCount = 0;
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->urlEdit->clear();
    ui->accountEdit->setEnabled(false);
    ui->passwordEdit->setEnabled(false);
    ui->urlEdit->setEnabled(false);
    ui->saveBuuton->setEnabled(false);
    catalogue_1 = rootItemModel->item(index.row())->text();
    QString s = QString("select * from user where name=='%1' and catalogue_1=='%2' ").arg(userAccount).arg(catalogue_1);
    QSqlQuery query;
    query.exec(s);
    while(query.next())
    {
        accountItemModel->setItem(accountCount++, 0, new QStandardItem(query.value(4).toString()));
        checkTime(query, accountCount - 1);
    }
}

void Widget::checkTime(QSqlQuery &query, int row)
{
    QDateTime currentTime = QDateTime::currentDateTime();
    QDateTime dateTime = QDateTime::fromString(query.value(6).toString(), "yyyy-MM-dd hh:mm:ss");
    int flag;
    if(dateTime.addSecs(5).secsTo(currentTime) < 0)
        flag = 0;
    else if(dateTime.addSecs(10).secsTo(currentTime) < 0)
        flag = 1;
    else if(dateTime.addSecs(15).secsTo(currentTime) < 0)
        flag = 2;
    else
        flag = 3;
    QColor niceGreen(175, 255, 175, 175);
    QColor niceYellow(240, 255, 165);
    QColor niceOrange(255, 200, 100);
    QColor niceRed(255, 0, 0, 100);
    switch(flag)
    {
    case 0:
        accountItemModel->item(row)->setBackground(niceGreen);
        break;
    case 1:
        accountItemModel->item(row)->setBackground(niceYellow);
        break;
    case 2:
        accountItemModel->item(row)->setBackground(niceOrange);
        break;
    case 3:
        accountItemModel->item(row)->setBackground(niceRed);
        break;
    }
}

void Widget::on_accountEdit_textChanged(const QString &arg1)
{
    //int rootRow = ui->rootTable->currentIndex().row();
    //int accountRow = ui->accountTable->currentIndex().row();
    //rootVector[rootRow][accountRow].account = arg1;

    //ui->saveBuuton->setEnabled(true);
}

void Widget::on_passwordEdit_textChanged(const QString &arg1)
{
    //int rootRow = ui->rootTable->currentIndex().row();
    //int accountRow = ui->accountTable->currentIndex().row();
    //rootVector[rootRow][accountRow].password = arg1;

    //ui->saveBuuton->setEnabled(true);

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

    //ui->saveBuuton->setEnabled(true);
}

void Widget::on_accountTable_clicked(const QModelIndex &index)
{
    if(0 != accountCount)
    {
        catalogue_2 = accountItemModel->item(index.row())->text();
        QString s = QString("select * from user where name=='%1' and catalogue_1=='%2' and catalogue_2=='%3' ").
                arg(userAccount).arg(catalogue_1).arg(catalogue_2);
        QSqlQuery query;
        query.exec(s);
        if(query.first())
        {
            int row = ui->accountTable->currentIndex().row();
            QStringList list = query.value(5).toString().split("\a");
            ui->accountEdit->setText(list[0]);
            ui->passwordEdit->setText(list[1]);
            ui->urlEdit->setText(list[2]);

            QDateTime currentTime = QDateTime::currentDateTime();
            QDateTime dateTime = QDateTime::fromString(query.value(6).toString(), "yyyy-MM-dd hh:mm:ss");
            if(dateTime.addSecs(600).secsTo(currentTime) > 0)
            {
                accountItemModel->item(row)->setToolTip("You have kept this password for 1 year. Please modify it.");
            }
        }
    }

    if("" != ui->accountEdit->text() || "" != ui->passwordEdit->text() || "" != ui->urlEdit->text())
    {
        ui->saveBuuton->setEnabled(true);
    }
    ui->accountEdit->setEnabled(true);
    ui->passwordEdit->setEnabled(true);
    ui->urlEdit->setEnabled(true);
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
    QString data = QString("%1\a%2\a%3").
            arg(ui->accountEdit->text()).arg(ui->passwordEdit->text()).arg(ui->urlEdit->text());

    QSqlQuery query;
    QString s = QString("select * from user where name=='%1' and catalogue_1=='%2' and catalogue_2=='%3' ").
            arg(userAccount).arg(catalogue_1).arg(catalogue_2);
    query.exec(s);
    if(query.first())
    {
        s = QString("update user set data=='%1' where name=='%2' and catalogue_1=='%3' and catalogue_2=='%4' ").
                arg(data).arg(userAccount).arg(catalogue_1).arg(catalogue_2);
        query.exec(s);
    }
    else
    {
        QDateTime currentTime = QDateTime::currentDateTime();
        QString temp = currentTime.toString("yyyy-MM-dd hh:mm:ss");
        s = QString("insert into user values(%1, '%2', '%3', '%4', '%5', '%6', '%7') ").arg(++id).arg(userAccount).
                arg(userPassword).arg(catalogue_1).arg(catalogue_2).arg(data).arg(temp);
        query.exec(s);
    }

    ui->saveBuuton->setEnabled(false);
}

void Widget::on_setRandomAccount_clicked()
{
    ui->accountEdit->setEnabled(true);
    const QString possibleCharacters("0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    const int randomStringLength = 10;
    QString randomString;
    qsrand(QTime::currentTime().msec());
    for(int i=0; i<randomStringLength; i++)
    {
        int j = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(j);
        randomString.append(nextChar);
    }
    ui->accountEdit->setText(randomString);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(randomString);
}

void Widget::on_setRandomPassword_clicked()
{
    ui->passwordEdit->setEnabled(true);
    const QString possibleCharacters(".!@0123456789abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ");
    const int randomStringLength = 10;
    QString randomString;
    qsrand(QTime::currentTime().msec());
    for(int i=0; i<randomStringLength; i++)
    {
        int j = qrand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(j);
        randomString.append(nextChar);
    }
    ui->passwordEdit->setText(randomString);
    QClipboard *clipboard = QApplication::clipboard();
    clipboard->setText(randomString);
}

void Widget::on_searchCata_1_textChanged(const QString &arg1)
{
    rootItemModel->clear();
    accountItemModel->clear();
    ui->searchCata_2->clear();
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->urlEdit->clear();
    rootCount = 0;
    accountCount = 0;
    QString s = QString("select * from user where name=='%1' and catalogue_1 like '%%2%' ").
            arg(userAccount).arg(arg1);
    QSqlQuery query;
    query.exec(s);
    if(query.first())
    {
        do
        {
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

void Widget::on_searchCata_2_textChanged(const QString &arg1)
{
    accountItemModel->clear();
    ui->accountEdit->clear();
    ui->passwordEdit->clear();
    ui->urlEdit->clear();
    accountCount = 0;
    QString s;
    QSqlQuery query;
    s = QString("select * from user where name=='%1' and catalogue_1=='%2' and catalogue_2 like '%%3%' ").
            arg(userAccount).arg(catalogue_1).arg(arg1);
    query.exec(s);
    while(query.next())
    {
        accountItemModel->setItem(accountCount++, 0, new QStandardItem(query.value(4).toString()));
        checkTime(query, accountCount - 1);
    }
}
