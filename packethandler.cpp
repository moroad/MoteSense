//! Implementation of packetHandler class
/*! \file packethandler.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "packethandler.h"
#include "motereading.h"
#include <stdint.h>
#include <QDebug>


packetHandler::packetHandler()
{
    count = 0;
    source = new QTcpSocket();
    //! \todo Make this connection user configurable.
    source->connectToHost("localhost",9002,QIODevice::ReadWrite);

    //! \todo Remove this line after debugging
    ringIndex.push_back(0);   QList<MoteReading> temp; ringBuffer.push_back(temp); motes.push_back(0);
    // Connect signals
    connect(source, SIGNAL(connected()), this, SLOT(connected()));
    connect(source, SIGNAL(readyRead()),this, SLOT(dataReady()));
    connect(source, SIGNAL(disconnected()), this, SLOT(disconnected()));
}

// This gets called when the TcpSocket is connected
void packetHandler::connected()
{
    QByteArray *data = new QByteArray();
    data->append(0x55);
    data->append(0x32);
    source->write(*data);
    emit sFconnected();
    qDebug() << "Connected to " << source->localAddress() << ":" << source->localPort();
}

//! \remarks This gets called when the TcpSocket is disconnected, so we tell the dataGraph class also.
void packetHandler::disconnected()
{
    emit disconnectNotice();
}

MoteReading packetHandler::getReading(int i)
{
        if(ringBuffer.count() > 0 && ringBuffer[i].count() < 0)
    {
        MoteReading *r = new MoteReading(0,0,0,0,0,0,0,0,0);
        return *r;
    }
    else
    {
    //! \bug I need to get this working correctly, but I'm not sure on the logic yet.

        MoteReading r = ringBuffer[i][0];
    return r;
    }
}


QList<int> packetHandler::getNodeList()
{

    QList<int> list = QList<int>(motes);
    return list;
}

void packetHandler::dataReady()
{
    QByteArray bytes = source->readAll();
//    for(int j = 0; j < bytes.length(); j++)
//        printf("%02x   ", ((uint8_t) bytes[j]));
//    printf("\n");
//
//    fflush(stdout);

    count++;

    if((int) bytes[0] != 0x1c)
        return;
//      printf("Length = %d ", bytes.length());
      int id = (int) (uint8_t) bytes[5];
    //  qDebug() << "Count = " << motes.count() << "ID = " << id;
      //! \a Check to see if the mote is already in the list, but mote id's must be sequential.
      if(motes.count() <= id)
      {
          qDebug() << "Adding node " << id;
          QList<MoteReading> temp;
          ringBuffer.push_back(temp);
          ringIndex.push_back(0);
          motes.push_back(id);
          emit this->nodesChanged();
      }
// This doesnt work:
//      vl = ((uint8_t) bytes[14] & 0xf)*256  + ((uint8_t) (bytes[15]));
//      mic = ((uint8_t) bytes[16] & 0xf)*256 + ((uint8_t) (bytes[17]));
//      ir = ((uint8_t) bytes[18] & 0xf)*256 + ((uint8_t) (bytes[19]));
//      temp = ((uint8_t) bytes[19] & 0xf)*256 + ((uint8_t) (bytes[20]));
//      accx = ((uint8_t) bytes[21] & 0xf)*256  + ((uint8_t) (bytes[22]));
//      accy = ((uint8_t) bytes[23] & 0xf)*256 + ((uint8_t) (bytes[24]));
//      mgx = ((uint8_t) bytes[25] & 0xf) * 256 + ((uint8_t) (bytes[26]));
//      mgy = ((uint8_t) (bytes[27] & 0xf)) * 256 + ((uint8_t) (bytes[28]));


      mgx = ((uint16_t) bytes[25]);
      mgx = mgx << 8;
      mgx ^= ((uint16_t) (bytes[26] & 0xff));


      mgy = ((uint16_t) bytes[27]);
      mgy = mgy << 8;
      mgy ^= ((uint16_t) (bytes[28] & 0xff));

      accy = ((uint16_t) bytes[23]);
      accy = accy << 8;
      accy ^= ((uint16_t) (bytes[24] & 0xff));

      accx = ((uint16_t) bytes[21]);
      accx = accx << 8;
      accx ^= ((uint16_t) (bytes[22] & 0xff));

      temp = ((uint16_t) bytes[19]);
      temp = temp << 8;
      temp ^= ((uint16_t) (bytes[20] & 0xff));

      ir = ((uint16_t) bytes[17]);
      ir = ir << 8;
      ir ^= ((uint16_t) (bytes[18] & 0xff));

      vl = ((uint16_t) bytes[15]);
      vl = vl << 8;
      vl ^= ((uint16_t) (bytes[16] & 0xff));

      mic = ((uint16_t) bytes[13]);
      mic = mic << 8;
      mic ^= ((uint16_t) (bytes[14] & 0xff));

      MoteReading *tempReading = new MoteReading(id, mgx, mgy, accx, accy, temp, ir, mic, vl);

      //! \remarks Up to MAX_BUFFER packets are stored in a ringBuffer for each mote.
      // Added the first case for input validation
      if(id < ringBuffer.count() && ringBuffer[id].count() < MAX_BUFFER)
      {
        ringBuffer[id].push_back(*tempReading);
      }
      else
      {
        if(ringIndex.count()-1 >= id && ringIndex[id] == MAX_BUFFER)
          {
              ringIndex[id] = 0;
          }

          ringBuffer[id].replace(ringIndex[id], *tempReading);
          ringIndex[id] = ringIndex[id] + 1;
      }

//      printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\n\n\n",tempReading->getVl(), tempReading->getMic(),tempReading->getIr(),tempReading->getTemp(),tempReading->getAccx(),tempReading->getAccy(),tempReading->getMgx(),tempReading->getMgy());
      //fflush(stdout);
}
