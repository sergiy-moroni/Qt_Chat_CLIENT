#ifndef STARTSCREEN_H
#define STARTSCREEN_H
#include "Database.h"
#include <QDialog>

namespace Ui {
class StartScreen;
}

class StartScreen : public QDialog
{
    Q_OBJECT

public:
    explicit StartScreen(std::shared_ptr<Database> dbptr = nullptr,
                         QWidget *parent = nullptr);
    ~StartScreen();
    QString m_login;
    QString m_password;
    void setLoginForm();
    void setRegistrationForm();

    std::shared_ptr<Database> getDatabase() const;

public slots:
    void onLoggetIn(QString login, QString password);
    void onRejectRequested();

private:
    Ui::StartScreen *ui;

    std::shared_ptr<Database> m_dbptr;

};

#endif // STARTSCREEN_H
