#include <iostream>
#include "CommandLineInterface.h"
#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>
using namespace std;

int main(int argc, char *argv[])
{
   QApplication a(argc, argv);

   auto w = MainWindow::CreateClient();
   if(w)
   w->show();
   else
   return 0;
   w->setWindowTitle(w->m_login);
  return a.exec();
}


