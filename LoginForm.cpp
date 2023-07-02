#include "LoginForm.h"
#include "ui_LoginForm.h"
#include <QMessageBox>

#include "C:\CPLUS-UI-ChatCPPTest-main\sources\Database.h"

LoginForm::LoginForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LoginForm)
{
    ui->setupUi(this);
}

LoginForm::~LoginForm()
{
    delete ui;
}

void LoginForm::setDatabase(std::shared_ptr<Database> dbptr)
{
    m_dbptr = dbptr;
}


void LoginForm::on_buttonBox_accepted()
{
    QString login = "2<"+ ui->loginEdit->text();
QString password = ui->passEdit->text();

        emit accepted(login, password);
}

void LoginForm::on_buttonBox_rejected( )
{
    emit rejected();
}


void LoginForm::on_registrationPushButton_clicked()
{
emit registrationRequested();
}




