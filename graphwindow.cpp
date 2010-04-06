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
    id = 0;
    QToolBar *toolBar = new QToolBar(this);
    QToolBar *lowerToolBar = new QToolBar(this);
    lowerToolBar->setFixedHeight(80);
    toolBar->setFixedHeight(80);

#if QT_VERSION < 0x040000
    setDockEnabled(TornOff, true);
    setRightJustification(true);
#else
    toolBar->setAllowedAreas(Qt::TopToolBarArea | Qt::BottomToolBarArea);
    lowerToolBar->setAllowedAreas(Qt::BottomToolBarArea);
#endif
    QWidget *hBox = new QWidget(toolBar);
    QWidget *lowerBox = new QWidget(lowerToolBar);
    QLabel *label = new QLabel("Frequency", hBox);
    QLabel *clabel = new QLabel("Range Max", hBox);
    QLabel *mlabel = new QLabel("Range Min", hBox);


    QwtCounter *counter = new QwtCounter(hBox);
    QwtCounter *rangeCounter = new QwtCounter(hBox);
    QwtCounter *minCounter = new QwtCounter(hBox);
    QPushButton *scaleButton = new QPushButton("&Autoscale", hBox);
    QPushButton *mgxButton = new QPushButton("MG&x", lowerBox);
    QPushButton *mgyButton = new QPushButton("MG&y", lowerBox);
    QPushButton *accxButton = new QPushButton("Accx", lowerBox);
    QPushButton *accyButton = new QPushButton("Accy", lowerBox);
    QPushButton *vlButton = new QPushButton("&Vl", lowerBox);
    QPushButton *micButton = new QPushButton("&Mic", lowerBox);
    QPushButton *irButton = new QPushButton("&Ir", lowerBox);
    QPushButton *tempButton = new QPushButton("&Temp", lowerBox);




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
    layout->addWidget(scaleButton);

    QHBoxLayout *lowerLayout = new QHBoxLayout(lowerBox);
    
    lowerLayout->addWidget(mgxButton);
    lowerLayout->addWidget(mgyButton);
    lowerLayout->addWidget(accxButton);
    lowerLayout->addWidget(accyButton);
    lowerLayout->addWidget(micButton);
    lowerLayout->addWidget(irButton);
    lowerLayout->addWidget(vlButton);
    lowerLayout->addWidget(tempButton);


  //  layout->addWidget(new QWidget(hBox), 10); // spacer);

#if QT_VERSION >= 0x040000
    toolBar->addWidget(hBox);
    lowerToolBar->addWidget(lowerBox);
  #endif
    addToolBar(toolBar);
    

    // Instantiates the plot, this being the parent widget
    plot = new DataPlot(this);
    setCentralWidget(plot);
    addToolBarBreak(Qt::TopToolBarArea);
    addToolBar(lowerToolBar);
    


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

void GraphWindow::setId(int i)
{
    id = i;
    plot->setId(i);
}

void GraphWindow::setSource(packetHandler * p)
{
    connect(p, SIGNAL(disconnectNotice()), plot, SLOT(disconnected()));
    plot->setSource(p);

}
