#include "StartScreen.h"
#include "ui_StartScreen.h"



StartScreen::StartScreen(std::shared_ptr<Database> dbptr, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::StartScreen)
{
    ui->setupUi(this);

    if(dbptr)
        m_dbptr =dbptr;
    else
        m_dbptr = make_shared<Database>();

    ui->loginWidget->setDatabase(m_dbptr);
     ui->RegistrationWidget->setDatabase(m_dbptr);

    connect(ui->loginWidget, &LoginForm::registrationRequested,this, &StartScreen::setRegistrationForm);
    connect(ui->loginWidget, &LoginForm::accepted, this, &StartScreen::onLoggetIn);
     connect(ui->loginWidget, &LoginForm::rejected, this, &StartScreen::onRejectRequested);
    connect(ui->RegistrationWidget, &RegistrationForm::loginRequested,this, &StartScreen::setLoginForm);
    connect(ui->RegistrationWidget, &RegistrationForm ::accepted,this, &StartScreen::onLoggetIn);
     connect(ui->RegistrationWidget, &RegistrationForm ::rejected,this, &StartScreen::onRejectRequested);
}

StartScreen::~StartScreen()
{
    delete ui;
}

void StartScreen::setLoginForm()
{
    ui->stackedWidget->setCurrentIndex(1);
}

void StartScreen::setRegistrationForm()
{

    ui->stackedWidget->setCurrentIndex(0);
}


void StartScreen::onLoggetIn(QString login, QString password)
{

    m_login = login;
    m_password = password;
    accept();
}

void StartScreen::onRejectRequested()
{
    reject();
}

std::shared_ptr<Database> StartScreen::getDatabase() const
{
    return m_dbptr;
}
