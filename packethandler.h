//! Class to serve packets to the DataPlot class.
/*! \file packethandler.h
  * \class packetHandler
  * \author Jharrod LaFon
  * \date Spring 2010
  * \remarks This class connects to the serial forwarder, and demultiplexes packets for the datagraph class.
  */
#ifndef PACKETHANDLER_H
#define PACKETHANDLER_H
#include <QTcpSocket>
#include <QHostAddress>
#include <QList>
#include "motereading.h"

//! Number of packets to be buffered for each mote
#define MAX_BUFFER 1

//! Class the serve packets to the DataPlot
class packetHandler : public QObject
{ Q_OBJECT

public:
    //! Default constructor
    packetHandler();

    /*! Function to return the list of nodes from which we have received packets.
      * \returns QList<int>
      */
    QList<int> getNodeList();

    /*! Function to get the most recent reading for node i
      * \param i The id of the mote.
      * \returns MoteReading
      */
    MoteReading getReading(int i);

signals:
    //! Signal emitted when a new node is detected.
    void nodesChanged();

    //! Signal emitted when disconnected from the serial forwarder
    void disconnectNotice();

    //! Signal emitted when connected to the serial forwarder
    void sFconnected();

public slots:
    //! Slot for when a connection is established.  This also initializes communication with the serial forwarder.
    void connected();

    //! Slot for when we are disconnected from the serial forwarder
    void disconnected();

    //! Slot for when there is new data ready from the serial forwarder

    void dataReady();

private:
    QTcpSocket * source;
    QList< QList<MoteReading> > ringBuffer;

    QList<int> ringIndex;
    QList<int> motes;


    int mgx, mgy, accx, accy, temp, ir, vl, mic, seq;
    unsigned long count;
};

#endif // PACKETHANDLER_H
