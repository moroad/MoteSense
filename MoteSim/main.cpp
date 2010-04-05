//! SimServer Entry Point
/*! \file main.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include <QtCore/QCoreApplication>
#include <QTcpServer>
#include "simserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    SimServer *s = new SimServer();

    return a.exec();
}
