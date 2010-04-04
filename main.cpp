//! Main application, entry point
/*!  \file main.cpp
 *   \author Jharrod LaFon
 *   \date Spring 2010
 *   \remarks Main application
 */

#include <QtGui/QApplication>
#include "mainwindow.h"
#include "packethandler.h"

//! Main function
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
