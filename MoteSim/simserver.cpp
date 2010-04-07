//! Implementation of SimServer class
/*! \file simserver.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "simserver.h"
#include "stdio.h"
#include "stdint.h"
#include <QDebug>
#include <QTimer>
//! Constructor

SimServer::SimServer()
{
    sa = new QTcpServer();
    connect(sa, SIGNAL(newConnection()), SLOT(connected()));
    sa->listen(QHostAddress::Any,9002);
    //qDebug() << sa->serverError();
    //qDebug() << sa->isListening();

}
SimServer::~SimServer()
{
    sa->close();
    socket->close();
}

void SimServer::connected()
{

    socket = sa->nextPendingConnection();
    qDebug() << "New Connection :" << socket->localAddress() << ":" << socket->localPort();
    connect(socket, SIGNAL(readyRead()), this, SLOT(dataReady()));    

    QTimer *t = new QTimer(this);
    connect(t, SIGNAL(timeout()), this, SLOT(sendRandom()));
    t->start(1500);

}
void SimServer::dataReady()
{
    QByteArray data = socket->readAll();
    for(int i = 0; i < data.length(); i++)
        printf("%02x\t", (uint8_t) data[i]);
    printf("\n");

}

void SimServer::sendRandom()
{
    static int seq = 0;
    int range = 0xf;
    QByteArray *data = new QByteArray();
    //1c 00 ff ff 00 01 14 00 06 00 01 01 a3 03 27 08 17 07 f0 05 22 07 4c 07 2c 07 f7 05 c4
    data->append(0x1c); //count             0
    data->append((char)0x00); //always zero       1
    data->append((char)0x00); //always 0xff       2
    data->append((char)0x00); //always 0xff       3
    data->append((char)0x00); //id                4
    data->append(rand() % NUM_MOTES); //always 14         5
    data->append((char)0x00); //                  6
    data->append((char)0x00); //                  7
    data->append((char)0x00); //                  8
    data->append((char)0x00); //                  9
    data->append((char)0x00); //                  10
    data->append((char)0x00); //                  11
    data->append(seq++ & 0xff); //seq             12
    data->append(rand() % range); //        13 mic
    data->append(rand() % range); //        14 mic
    data->append(rand() % range); //        15 vl
    data->append(rand() % range); //        16 vl
    data->append(rand() % range); //        17 ir
    data->append(rand() % range); //        18 ir
    data->append(rand() % range); //        19 temp
    data->append(rand() % range); //        20 temp
    data->append(rand() % range); //        21 accx
    data->append(rand() % range); //        22 accx
    data->append(rand() % range); //        23 accy
    data->append(rand() % range); //        24 accy
    data->append(rand() % range); //        25 mgy
    data->append(rand() % range); //        26 mgy
    data->append(rand() % range); //        27 mgx
    data->append(rand() % range); //        28 mgx

    //qDebug() << data;
    //qDebug() << "Sending data...";
    socket->write(*data);
    socket->flush();
    //else qDebug() << "Not writeable";
}
