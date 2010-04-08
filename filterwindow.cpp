//! Implementation of the FilterWindow class
/*! \file filterwindow.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "filterwindow.h"
#include <qwt_knob.h>
#include <qwt_counter.h>
#include <qtoolbar.h>
#include <qlabel.h>
#include <qlayout.h>
#include "packethandler.h"
#include "filtered_data_plot.h"
#include <QPushButton>

FilterWindow::FilterWindow()
{
    id = 0;
    QToolBar *toolBar = new QToolBar(this);
    QToolBar *lowerToolBar = new QToolBar(this);
    //lowerToolBar->setFixedHeight(80);
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
    QPushButton *avgButton = new QPushButton("Local Avg", lowerBox);
    QPushButton *eButton = new QPushButton("e", lowerBox);
    QPushButton *fButton = new QPushButton("f[n]", lowerBox);
    QPushButton *mavgButton = new QPushButton("Moving Avg", lowerBox);

    QwtKnob *rcKnob = new QwtKnob(lowerBox);
    rcKnob->setRange(0,5,.1,0);
    rcKnob->setScaleMaxMajor(10);
    rcKnob->setValue(5.0);
    QLabel *rcLabel = new QLabel("RC", rcKnob);
    rcLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QwtKnob *dtKnob = new QwtKnob(lowerBox);
    dtKnob->setRange(0,5,.1,0);
    dtKnob->setScaleMaxMajor(10);
    dtKnob->setValid(1.0);
    QLabel *dtLabel = new QLabel("Dt", dtKnob);
    dtLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);

    QwtKnob *duKnob = new QwtKnob(lowerBox);
    duKnob->setRange(0,5000,10,1);

    duKnob->setScaleMaxMajor(10);
    duKnob->setValid(1.0);

    QLabel *duLabel = new QLabel("Du", duKnob);

    duLabel->setAlignment(Qt::AlignTop | Qt::AlignHCenter);


    // Sets the range and increment of the widgets
    counter->setRange(-1.0, 2000.0, 10.0);
    rangeCounter->setRange(-1048576.0,INT_MAX,100.0);
    minCounter->setRange(-1048576.0, INT_MAX, 100.0);

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
    lowerLayout->addWidget(avgButton);
    lowerLayout->addWidget(eButton);
    lowerLayout->addWidget(fButton);
    lowerLayout->addWidget(mavgButton);
    lowerLayout->addWidget(rcKnob);
    lowerLayout->addWidget(dtKnob);
    lowerLayout->addWidget(duKnob);

  //  layout->addWidget(new QWidget(hBox), 10); // spacer);

#if QT_VERSION >= 0x040000
    toolBar->addWidget(hBox);
    lowerToolBar->addWidget(lowerBox);
  #endif
    addToolBar(toolBar);
    

    // Instantiates the plot, this being the parent widget
    plot = new FilteredDataPlot(this);
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
    connect(avgButton, SIGNAL(clicked()), plot, SLOT(detachAvg()));
    connect(eButton,SIGNAL(clicked()), plot, SLOT(detachE()));
    connect(fButton, SIGNAL(clicked()), plot, SLOT(detachF()));
    connect(mavgButton, SIGNAL(clicked()), plot, SLOT(detachMavg()));
    connect(scaleButton, SIGNAL(clicked()), plot, SLOT(autoScale()));
    connect(scaleButton, SIGNAL(clicked()), plot->getDataFilter(), SLOT(resetLocalMax()));
    connect(rcKnob, SIGNAL(valueChanged(double)), plot->getDataFilter(), SLOT(setRc(double)) );
    connect(dtKnob, SIGNAL(valueChanged(double)), plot->getDataFilter(), SLOT(setDt(double)) );
    connect(duKnob, SIGNAL(valueChanged(double)), plot->getDataFilter(), SLOT(setU(double)));
    counter->setValue(50.0);
    rangeCounter->setValue(10000.0);
    minCounter->setValue(2000);

}

void FilterWindow::setId(int i)
{
    id = i;
    plot->setId(i);
}

void FilterWindow::setSource(packetHandler * p)
{
    connect(p, SIGNAL(disconnectNotice()), plot, SLOT(disconnected()));
    plot->setSource(p);

}
