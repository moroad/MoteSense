//! Implementation of DataPlot class
/*! \file data_plot.cpp
 *  \author Jharrod LaFon
 *  \date Spring 2010
 */
#include <QDebug>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include "data_plot.h"
#include "packethandler.h"

//! Constructor
/*!
  * Initializes the Main Window
  */
DataPlot::DataPlot(QWidget *parent):
    QwtPlot(parent),
    d_interval(0),
    d_timerId(-1),
    min(0),
    max(10)



{
    id = 0;
    plotAccx = plotAccy = plotMgx = plotMgy = plotTemp = plotMic = plotVl = plotIr = true;
    for(int i = 0; i < 8; i++)
        attached[i] = true;
    // Disable polygon clipping
    QwtPainter::setDeviceClipping(false);
    running = false;
    // We don't need the cache here
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintCached, false);
    canvas()->setPaintAttribute(QwtPlotCanvas::PaintPacked, false);

#if QT_VERSION >= 0x040000
#ifdef Q_WS_X11
    /*
       Qt::WA_PaintOnScreen is only supported for X11, but leads
       to substantial bugs with Qt 4.2.x/Windows
     */
    canvas()->setAttribute(Qt::WA_PaintOnScreen, true);
#endif
#endif

    alignScales();
    
    //  Initialize data
    for (int i = 0; i< PLOT_SIZE; i++)
    {
        d_x[i] = 0.5 * i;     // time axis
        d_mgx[i] = 0;
        d_mgy[i] = 0;
        d_accx[i] = 0;
        d_accy[i] = 0;
        d_mic[i] = 0;
        d_ir[i] = 0;
        d_temp[i] = 0;
    }


    // Assign a title
    setTitle("Moving Graph");
    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    mgxPlot = new QwtPlotCurve("MGx");
    mgyPlot = new QwtPlotCurve("MGy");
    AccyPlot = new QwtPlotCurve("ACCy");
    AccxPlot = new QwtPlotCurve("ACCx");
    tempPlot = new QwtPlotCurve("Temp");
    irPlot = new QwtPlotCurve("IR");
    micPlot = new QwtPlotCurve("MIC");
    vlPlot = new QwtPlotCurve("VL");

    AccyPlot->attach(this);
    AccxPlot->attach(this);
    mgxPlot->attach(this);
    mgyPlot->attach(this);
    tempPlot->attach(this);
    irPlot->attach(this);
    micPlot->attach(this);
    vlPlot->attach(this);


    //! \todo Change line thickness
    // Set curve styles
    mgxPlot->setPen(QPen(Qt::blue));
    mgyPlot->setPen(QPen(Qt::red));
    AccyPlot->setPen(QPen(Qt::green));
    AccxPlot->setPen(QPen(Qt::black));
    tempPlot->setPen(QPen(Qt::darkBlue));
    irPlot->setPen(QPen(Qt::darkCyan));
    micPlot->setPen(QPen(Qt::darkMagenta));
    vlPlot->setPen(QPen(Qt::darkRed));

    // Attach (don't copy) data. Both curves use the same x array.
    mgxPlot->setRawData(d_x, d_mgx, PLOT_SIZE);
    mgyPlot->setRawData(d_x, d_mgy, PLOT_SIZE);
    AccyPlot->setRawData(d_x, d_accy, PLOT_SIZE);
    AccxPlot->setRawData(d_x, d_accx, PLOT_SIZE);
    tempPlot->setRawData(d_x, d_temp, PLOT_SIZE);
    irPlot->setRawData(d_x, d_ir, PLOT_SIZE);
    micPlot->setRawData(d_x, d_mic, PLOT_SIZE);
    vlPlot->setRawData(d_x, d_vl, PLOT_SIZE);

#if 0
    //  Insert zero line at y = 0
    QwtPlotMarker *mY = new QwtPlotMarker();
    mY->setLabelAlignment(Qt::AlignRight|Qt::AlignTop);
    mY->setLineStyle(QwtPlotMarker::HLine);
    mY->setYValue(0.0);
    mY->attach(this);
#endif

    // Axis 
    setAxisTitle(QwtPlot::xBottom, "Time/seconds");
    setAxisScale(QwtPlot::xBottom, 0, 100);

    setAxisTitle(QwtPlot::yLeft, "Values");
    setAxisScale(QwtPlot::yLeft, 0, 10);
    
    setTimerInterval(0.0); 
}


void DataPlot::setRange(double range)
{
    max = (int) range;
    setAxisScale(QwtPlot::yLeft, min, max);
}
void DataPlot::setMinRange(double range)
{
    //! \bug Qt Doesn't capture this event for some reason
    min = (int) range;
    setAxisScale(QwtPlot::yRight,min, max);
}

void DataPlot::disconnected()
{
      // This happens when we get disconnected from the packetHandler
      running = false;
}

//
//  Set a plain canvas frame and align the scales to it
//
void DataPlot::alignScales()
{

    canvas()->setFrameStyle(QFrame::Box | QFrame::Plain );
    canvas()->setLineWidth(1);

    for ( int i = 0; i < QwtPlot::axisCnt; i++ )
    {
        QwtScaleWidget *scaleWidget = (QwtScaleWidget *)axisWidget(i);
        if ( scaleWidget )
            scaleWidget->setMargin(0);

        QwtScaleDraw *scaleDraw = (QwtScaleDraw *)axisScaleDraw(i);
        if ( scaleDraw )
            scaleDraw->enableComponent(QwtAbstractScaleDraw::Backbone, false);
    }
}


void DataPlot::detach(int item)
{
    switch(item)
    {
    case MGX:
        if(attached[MGX])
        {
            mgxPlot->detach();
            attached[MGX] = false;
        }
        else
        {
            mgxPlot->attach(this);
            attached[MGX] = true;
        }
        break;
    case MGY:
        if(attached[MGY])
        {
            mgyPlot->detach();
            attached[MGY] = false;
        }
        else
        {
            mgyPlot->attach(this);
            attached[MGY] = true;
        }
        break;
    case ACCX:
        if(attached[ACCX])
        {
            AccxPlot->detach();
            attached[ACCX] = false;
        }
        else
        {
            AccxPlot->attach(this);
            attached[ACCX] = true;
        }
        break;
    case ACCY:
        if(attached[ACCY])
        {
            AccyPlot->detach();
            attached[ACCY] = false;
        }
        else
        {
            AccyPlot->attach(this);
            attached[ACCY] = true;
        }
        break;
    case TEMP:
        if(attached[TEMP])
        {
            tempPlot->detach();
            attached[TEMP] = false;
        }
        else
        {
            tempPlot->attach(this);
            attached[TEMP] = true;
        }
        break;
    case IR:
        if(attached[IR])
        {
            irPlot->detach();
            attached[IR] = false;
        }
        else
        {
            irPlot->attach(this);
            attached[IR] = true;
        }
        break;
    case VL:
        if(attached[VL])
        {
            vlPlot->detach();
            attached[VL] = false;
        }
        else
        {
            vlPlot->attach(this);
            attached[VL] = true;
        }
        break;
    case MIC:
        if(attached[MIC])
        {
            micPlot->detach();
            attached[MIC] = false;
        }
        else
        {
            micPlot->attach(this);
            attached[MIC] = true;
        }
        break;
    }
}

void DataPlot::setTimerInterval(double ms)
{
    //! \remark Units are in ms
    d_interval = qRound(ms);

    if ( d_timerId >= 0 )
    {
        killTimer(d_timerId);
        d_timerId = -1;
    }
    if (d_interval >= 0 )
        d_timerId = startTimer(d_interval);
}

void DataPlot::newReading(MoteReading r)
{
    currentReading = r;
}

//! \remark This gets called to update the graph.
void DataPlot::timerEvent(QTimerEvent *)
{
    if(!running)
        return;

    // Get new reading
    this->newReading(mySource->getReading(0));

        for ( int j = 0; j < PLOT_SIZE - 1; j++ )
        {
            d_mgx[j] = d_mgx[j+1];
            d_mgy[j] = d_mgy[j+1];
            d_accx[j] = d_accx[j+1];
            d_accy[j] = d_accy[j+1];
            d_temp[j] = d_temp[j+1];
            d_ir[j] = d_ir[j+1];
            d_mic[j] = d_mic[j+1];
            d_vl[j] = d_vl[j+1];
        }

        d_mgx[PLOT_SIZE -1] = currentReading.getMgx();
        d_mgy[PLOT_SIZE -1] = currentReading.getMgy();
        d_accx[PLOT_SIZE -1] = currentReading.getAccx();
        d_accy[PLOT_SIZE -1] = currentReading.getAccy();
        d_temp[PLOT_SIZE -1] = currentReading.getTemp();
        d_ir[PLOT_SIZE -1] = currentReading.getIr();
        d_mic[PLOT_SIZE -1] = currentReading.getMic();
        d_vl[PLOT_SIZE -1] = currentReading.getVl();

       // update the display
    replot();

    }
