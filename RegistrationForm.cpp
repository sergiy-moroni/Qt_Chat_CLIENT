#include "RegistrationForm.h"
#include "ui_RegistrationForm.h"
#include <QMessageBox>
#include <C:\CPLUS-UI-ChatCPPTest-main\sources\Database.h>

RegistrationForm::RegistrationForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::RegistrationForm)
{
    ui->setupUi(this);
}

RegistrationForm::~RegistrationForm()
{
    delete ui;
}

void RegistrationForm::setDatabase(std::shared_ptr<Database> dbptr)
{
    m_dbptr = dbptr;

}

void RegistrationForm::on_loginbutton_clicked()
{
emit loginRequested();
}


void RegistrationForm::on_buttonBox_accepted()
{
if(ui->passEdit->text() != ui->confirmPassEdit->text())
{

    QMessageBox::critical(this, tr("Error"), tr("Password not match"));
    return;
}
QString login = "1<" + ui->loginEdit->text();
QString password = ui->passEdit->text();

    emit accepted(login, password);

}

void RegistrationForm::on_buttonBox_rejected()
{
emit rejected();
}

