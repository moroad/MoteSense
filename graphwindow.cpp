//! Implementation of the GraphWindow class
/*! \file graphwindow.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "graphwindow.h"
#include <qwt_counter.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include "packethandler.h"
#include "data_plot.h"
#include <QPushButton>

GraphWindow::GraphWindow()
{
    QToolBar *toolBar = new QToolBar(this);
    toolBar->setFixedHeight(80);

#if QT_VERSION < 0x040000
    setDockEnabled(TornOff, true);
    setRightJustification(true);
#else
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
#endif
    QWidget *hBox = new QWidget(toolBar);
    QLabel *label = new QLabel("Timer Interval", hBox);
    QLabel *clabel = new QLabel("Range Max", hBox);
    QLabel *mlabel = new QLabel("Range Min", hBox);



    QwtCounter *counter = new QwtCounter(hBox);
    QwtCounter *rangeCounter = new QwtCounter(hBox);
    QwtCounter *minCounter = new QwtCounter(hBox);
    QPushButton *mgxButton = new QPushButton("MG&x", this);
    QPushButton *mgyButton = new QPushButton("MG&y", this);
    QPushButton *accxButton = new QPushButton("Accx", this);
    QPushButton *accyButton = new QPushButton("Accy", this);
    QPushButton *vlButton = new QPushButton("&Vl", this);
    QPushButton *micButton = new QPushButton("&Mic", this);
    QPushButton *irButton = new QPushButton("&Ir", this);
    QPushButton *tempButton = new QPushButton("&Temp", this);
    QPushButton *scaleButton = new QPushButton("&Autoscale", this);



    // Sets the range and increment of the widgets
    counter->setRange(-1.0, 2000.0, 10.0);
    rangeCounter->setRange(-1048576.0,1048576.0,10.0);
    minCounter->setRange(-1048576.0, 1048576.0, 10.0);


    QHBoxLayout *layout = new QHBoxLayout(hBox);
    layout->addWidget(label);
    layout->addWidget(counter);
    layout->addWidget(clabel);
    layout->addWidget(rangeCounter);
    layout->addWidget(mlabel);
    layout->addWidget(minCounter);
    layout->addWidget(mgxButton);
    layout->addWidget(mgyButton);
    layout->addWidget(accxButton);
    layout->addWidget(accyButton);
    layout->addWidget(micButton);
    layout->addWidget(irButton);
    layout->addWidget(vlButton);
    layout->addWidget(tempButton);
    layout->addWidget(scaleButton);


  //  layout->addWidget(new QWidget(hBox), 10); // spacer);

#if QT_VERSION >= 0x040000
    toolBar->addWidget(hBox);
#endif
    addToolBar(toolBar);

    // Instantiates the plot, this being the parent widget
    plot = new DataPlot(this);
    setCentralWidget(plot);

   // Connect signals
    connect(counter, SIGNAL(valueChanged(double)),
        plot, SLOT(setTimerInterval(double)) );
    connect(rangeCounter, SIGNAL(valueChanged(double)),
            plot, SLOT(setRange(double)));
    connect(minCounter, SIGNAL(valueChanged(double)),
            plot, SLOT(setMinRange(double)));
    connect(mgxButton, SIGNAL(clicked()), plot, SLOT(detachMgx()));
    connect(mgyButton, SIGNAL(clicked()), plot, SLOT(detachMgy()));
    connect(accxButton, SIGNAL(clicked()), plot, SLOT(detachAccx()));
    connect(accyButton, SIGNAL(clicked()), plot, SLOT(detachAccy()));
    connect(micButton, SIGNAL(clicked()), plot, SLOT(detachMic()));
    connect(tempButton, SIGNAL(clicked()), plot, SLOT(detachTemp()));
    connect(vlButton, SIGNAL(clicked()), plot, SLOT(detachVl()));
    connect(irButton, SIGNAL(clicked()), plot, SLOT(detachIr()));
    connect(scaleButton, SIGNAL(clicked()), plot, SLOT(autoScale()));
    counter->setValue(50.0);
    rangeCounter->setValue(4096.0);
    minCounter->setValue(1200);
}

void GraphWindow::setSource(packetHandler * p)
{
    connect(p, SIGNAL(disconnectNotice()), plot, SLOT(disconnected()));
    plot->setSource(p);

}
