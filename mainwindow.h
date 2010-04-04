//! Main Program Window
/*! \file mainwindow.h
  * \class MainWindow
  * \author Jharrod LaFon
  * \date Spring 2010
  * \remarks Main Window class.  Inherits from QMainWindow
  * \mainpage MoteSense Student Project
  * \author Jharrod LaFon, Michael Harris
  * \date Spring 2010
  *
  * \remarks This project is designed to acquire, and plot data in real time from a Wireless Sensor Network.  The end goal is to use this data to recognize
  * when a vehicle is present at the sensor.  It was written by Jharrod LaFon and Michael Harris
  * at New Mexico State University for our Senior Project in 2010.  This program connects to a tcp socket from which it acquires the data to plot.
  * For our development we have been using the serial forwarder application provided with TinyOS.
  * \paragraph
  *
  * Here is the hardware we used for development:
  *     - Tmote Sky IV Motes
  *     - SBT80 Sensor Boards
  *
  * Here is what we needed for our development environment:
  *     - TinyOS
  *         -# Version 2.1.1
  *     - Tinyos-tools
  *     - Qt4
  *     - Qwt http://qwt.sourceforge.net/
  *
  * \note To work with qwt, in the MoteSense.pro these lines must specify where the library is:
  * INCLUDEPATH += /usr/local/qwt-5.2.0/include/
  * LIBS += /usr/local/qwt-5.2.0/lib/libqwt.so.5.2.0
  * \see MainWindow class
  * \see packetHandler class
  * \see MoteReading class
  * \see GraphWindow class
  * \see DataPlot class
  * \see http://github.com/jlafon/MoteSense
  */
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QList>
#include "packethandler.h"
namespace Ui {
    class MainWindow;
}
//! The main window class
class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    //! Default constructor
    MainWindow(QWidget *parent = 0);
    //! Destructor
    ~MainWindow();

public slots:
    //! Updates the nodeList widget when a node is added or removed from the packet stream.
    void nodeChange();

protected:
    //! Needed for Qt
    void changeEvent(QEvent *e);

private:

    Ui::MainWindow *ui;
    packetHandler * server;
    QList<int> nodeList;

private slots:
    //! Function handler for when the plot button is clicked.
    /*!
     *  Launches graphWindow tracking the node selected in the
     *  nodeList widget.
     */
    void on_plotButton_clicked();
};

#endif // MAINWINDOW_H
