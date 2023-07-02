#include "mainwindow.h"
#include "ui_MainWindow.h"
#include "C:\CPLUS-UI-ChatCPPTest-main\sources\StartScreen.h"
#include <QDialog>
#include <QVBoxLayout>
#include <QDialogButtonBox>
#include <QListWidget>
#include <QTimer>
#include <QMessageBox>

int MainWindow::kInstasceCount =0;

MainWindow::MainWindow(  QString login, QString password,std::shared_ptr<Database> dbptr,QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_login(login),
    m_password(password)
{
    ui->setupUi(this);
    socket = new QTcpSocket(this);

    connect (socket, &QTcpSocket::readyRead,this, &MainWindow::slotReadyRead);
    connect(socket,&QTcpSocket::disconnected, socket, &QTcpSocket::deleteLater);

    socket->connectToHost("127.0.0.1", 2323);

    QString registr = m_login + "> ["+m_password + "]";
    SendToServer(registr);
    kInstasceCount++;
    QString chlog;
    for(int i =2;i< m_login.length();i++){
        chlog += m_login[i];
    }
    m_login = chlog;

    if(dbptr)
        m_dbptr = dbptr;
    else{
        m_dbptr = make_shared<Database>();}
    auto timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::updateChats);
    timer->start(10);
}

MainWindow::~MainWindow()
{
    kInstasceCount--;
    delete ui;
    if(kInstasceCount <=0){
        socket->close();
        qApp->exit(0);
    }
}

MainWindow *MainWindow::CreateClient(std::shared_ptr<Database> dbptr)
{
   StartScreen s(dbptr);
   auto result =  s.exec();
   if(result == QDialog::Rejected)
   {
    return nullptr;
   }else
   {
   auto w =  new MainWindow(s.m_login,s.m_password, s.getDatabase());
   w->setAttribute(Qt::WA_DeleteOnClose);
   return w;
   }
}

void MainWindow::on_lineEdit_returnPressed()
{
    on_SendToAllMessageButton_clicked();
}

void MainWindow::on_SendToAllMessageButton_clicked()
{
    QString curstr = QString::number(5) + QString("<") +m_login + QString("> : ") + ui->lineEdit->text()+ QString("\n");
    SendToServer(curstr);
    ui->lineEdit->clear();
}

void MainWindow::on_SendPrivateMessageButton_clicked()
{
    QDialog dial(this);
    dial.setModal(true);
    auto l = new QVBoxLayout();
    dial.setLayout(l);
    auto userlistwidget = new QListWidget(&dial);
    l->addWidget(userlistwidget);
    auto buttonbox = new QDialogButtonBox(QDialogButtonBox::Ok| QDialogButtonBox::Cancel, &dial);
    l->addWidget(buttonbox);
    connect(buttonbox, &QDialogButtonBox::accepted, &dial, &QDialog::accept);
     connect(buttonbox, &QDialogButtonBox::rejected, &dial, &QDialog::reject);
 //   auto userList = m_dbptr->getUserList() ;
     for(auto user: Userlist)
    {
    userlistwidget->addItem(user);
    }

     userlistwidget->setCurrentRow(0);
     auto result = dial.exec();
     if(result == QDialog::Accepted && userlistwidget->currentItem())
     {
     QString curstr = QString::number(6) + QString("<")+ m_login + QString(">[")  + userlistwidget->currentItem()->text() + QString("] :") + ui->lineEdit->text() + QString("\n");
     SendToServer(curstr);
     }
      ui->lineEdit->clear();
}

void MainWindow::on_actionOpen_enother_client_triggered()
{
    auto w =  CreateClient(m_dbptr);
    w->show();
}


void MainWindow::on_actionClose_this_client_triggered()
{
    this->close();
}

void MainWindow::updateChats()
{
    ui->commonChatBrowser->setText(chat);
    ui->privateChatBrowser->setText(privateChat);
}

//обработка ответов от сервера
void MainWindow::slotReadyRead()
{
    QDataStream in(socket);
    in.setVersion(QDataStream::Qt_4_2);
    if(in.status()==QDataStream::Ok)
    {
    in >> str;
    QString message;
    QString sender;
    QString reciever;
    QString getsuser;
    int counter = 1;

    //Вход по логину неудачный
   if(str[0]=='G')
    {
    for(int i = 1; i< str.length();i++)
       {
           sender+=str[i];
       }
    if(sender == m_login)
       {
       QMessageBox::critical(this, tr("Error"), tr("Client not found"));
       qApp->exit(0);
       return;
       }
    }

    //ПОлучение списка зарегестрированных пользователей
    if(str[0]=='F')
    {
    for(int i = 1; i< str.length();i++)
    {
    if(str[i]=='<')
    {
             while(str[i+counter]!='>')
       {
             getsuser += str[i+counter++];
       }
    }
            counter =1;
            if(str[i]=='[')
            {
                while(str[i+counter]!=']')
                {
                 reciever += str[i+counter++];
                }
            if(getsuser == m_login)
            {
                 Userlist.push_back(reciever);
                 reciever.clear();
                 getsuser.clear();
            }
             counter =1;
            }
    }
    }

    //Получение приватного сообщения
   if(str[0]=='6')
   {
    for(int i =1; i<str.length();i++)
    {
            if(str[i]=='<')
            {
        while(str[i+counter]!='>')
        {
                 sender += str[i+counter++];
        }
            }
            counter =1;

            if(str[i]=='[')
            {
        while(str[i+counter]!=']')
        {
                 reciever += str[i+counter++];
        }
            }
            counter =1;

            if(str[i]==':'){
        while(str[i+counter]!='\n')
        {
                 message += str[i+counter++];
        }
            }
            counter =1;
    }
        if(reciever == m_login)
        {
            privateChat.append("From " + sender + ": "+message + "\n");
            message.clear();
        }

  }

  //Получение сообщения для всех
   if(str[0]=='5')
    {

       chat.append(str);
       str.clear();
       in >> str;
   }
       in >> str;
   }
}

void MainWindow::SendToServer(QString str2)
{
    Data.clear();
    QDataStream out(&Data, QIODevice::WriteOnly);
    out.setVersion(QDataStream::Qt_4_2);
    out << str2;
    socket->write(Data);
}

void MainWindow::on_pushButton_clicked()
{
    SendToServer(QString::number(3) + "<" + m_login + ">" );
    ui->pushButton->close();

}

