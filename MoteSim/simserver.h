//! Simulated data source for MoteSense
/*! \file simserver.h
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#ifndef SIMSERVER_H
#define SIMSERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

//! Class to simulate a mote data source
class SimServer : public QObject
{
    Q_OBJECT
public:
    //! Default constructor
    SimServer();
    ~SimServer();
public slots:
    //! Slot called when a new connection is established
    void connected();

    //! Slot called when data from the TcpSocket is ready to be read
    void dataReady();

    //! Slot to send a packet with random sensor readings
    void sendRandom();

private:
    QTcpServer *sa;
    QTcpSocket *socket;
};

#endif // SIMSERVER_H
