#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "C:\CPLUS-UI-ChatCPPTest-main\sources\Database.h"
#include <memory>
#include <QTcpSocket>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(   QString login, QString password,std::shared_ptr<Database> dbptr,QWidget *parent = nullptr);
    ~MainWindow();
    static MainWindow* CreateClient(std::shared_ptr<Database> dbptr = nullptr);
    static int kInstasceCount;
    QTcpSocket* socket;
    QString str;
     QString chat;
    QString privateChat;
     QString m_login;
     QString m_password;
     QVector<QString> Userlist;

     void SendToServer(QString str);

private slots:

    void on_lineEdit_returnPressed();
    void on_SendToAllMessageButton_clicked();
    void on_SendPrivateMessageButton_clicked();
    void on_actionOpen_enother_client_triggered();
    void on_actionClose_this_client_triggered();
    void updateChats();
      void slotReadyRead();

    void on_pushButton_clicked();

private:
     QByteArray Data;
    Ui::MainWindow *ui;
    std::shared_ptr<Database> m_dbptr;

};

#endif // MAINWINDOW_H
