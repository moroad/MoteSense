//! Implementation of FilteredDataPlot class
/*! \file filtered_data_plot.cpp
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
#include <qwt_curve_fitter.h>
#include <qwt_math.h>
#include "filtered_data_plot.h"
#include "packethandler.h"

//! Constructor
/*!
  * Initializes the Main Window
  */
FilteredDataPlot::FilteredDataPlot(QWidget *parent):
    QwtPlot(parent),
    d_interval(0),
    d_timerId(-1),
    min(0),
    max(10)



{

    id = 0;
    plotMgx = plotAvg = plotE = plotF = plotMavg = true;
    auto_scale = false;
    filter = new dataFilter();
    myType = 0;
    for(int i = 0; i < 11; i++)
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
    for (int i = 0; i< FPLOT_SIZE; i++)
    {
        d_x[i] = 0.5 * i;     // time axis
        d_mgx[i] = 0;
        d_avg[i] = 0;
        d_e[i] = 0;
        d_f[i] = 0;
        d_mavg[i] = 0;

    }


    // Assign a title
    setTitle(QString("Sensors for Node %1").arg(id));
    insertLegend(new QwtLegend(), QwtPlot::BottomLegend);

    // Insert new curves
    mgxPlot = new QwtPlotCurve("MGx");
    //mgxFilteredPlot = new QwtPlotCurve("Filtered MGx");

    avgPlot = new QwtPlotCurve("Local Avg");
    //mgyPlot = new QwtPlotCurve("Moving Average");

    ePlot = new QwtPlotCurve("e");


    fPlot = new QwtPlotCurve("F[n]");
    mavgPlot = new QwtPlotCurve("Moving Avg");

    mgxPlot->attach(this);
    avgPlot->attach(this);
    ePlot->attach(this);
    fPlot->attach(this);
    mavgPlot->attach(this);

    const QColor bgColor(255,255,255);
#if QT_VERSION < 0x040000
    setPaletteBackgroundColor(bgColor);
#else
    QPalette p = palette();
    p.setColor(backgroundRole(), bgColor);
    setPalette(p);
#endif

    //! \todo Change line thickness
    // Set curve styles

    QPen *pen = new QPen(Qt::darkBlue);
    pen->setWidth(2);
    mgxPlot->setPen(*pen);

    QwtSplineCurveFitter *f = new QwtSplineCurveFitter();
    f->setFitMode(QwtSplineCurveFitter::Spline);
    pen->setColor(Qt::darkMagenta);
    mgxPlot->setCurveFitter(f);
    mgxPlot->setPen(*pen);

    pen->setColor(Qt::red);
    ePlot->setPen(*pen);
    ePlot->setCurveFitter(f);

    pen->setColor(Qt::green);
    fPlot->setPen(*pen);
    fPlot->setCurveFitter(f);

    pen->setColor(Qt::darkGray);
    mavgPlot->setPen(*pen);
    mavgPlot->setCurveFitter(f);

    // Attach (don't copy) data. Both curves use the same x array.
    mgxPlot->setRawData(d_x, d_mgx, FPLOT_SIZE);
    avgPlot->setRawData(d_x, d_avg, FPLOT_SIZE);
    fPlot->setRawData(d_x, d_f, FPLOT_SIZE);
    ePlot->setRawData(d_x, d_e, FPLOT_SIZE);
    mavgPlot->setRawData(d_x, d_mavg, FPLOT_SIZE);

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

void FilteredDataPlot::reRange()
{
    setAxisScale(QwtPlot::yLeft, min, max);
}

void FilteredDataPlot::setRange(double range)
{
    max = (int) range;
    setAxisScale(QwtPlot::yLeft, min, max);
}
void FilteredDataPlot::setMinRange(double range)
{
    //! \note Old Bug: Qt Doesn't capture this event for some reason. Fixed this bug.
    min = (int) range;
    setAxisScale(QwtPlot::yLeft,min, max);
}

void FilteredDataPlot::disconnected()
{
      // This happens when we get disconnected from the packetHandler
      running = false;
}

//
//  Set a plain canvas frame and align the scales to it
//
void FilteredDataPlot::alignScales()
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


void FilteredDataPlot::detach(int item)
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
    case F:
        if(attached[F])
        {
            fPlot->detach();
            attached[F] = false;
        }
        else
        {
            fPlot->attach(this);
            attached[F] = true;
        }
        break;
    case E:
        if(attached[E])
        {
            ePlot->detach();
            attached[E] = false;
        }
        else
        {
            ePlot->attach(this);
            attached[E] = true;
        }
        break;
    case AVG:
        if(attached[AVG])
        {
            avgPlot->detach();
            attached[AVG] = false;
        }
        else
        {
            avgPlot->attach(this);
            attached[AVG] = true;
        }
        break;
    case MAVG:
        if(attached[MAVG])
        {
            mavgPlot->detach();
            attached[MAVG] = false;
        }
        else
        {
            mavgPlot->attach(this);
            attached[MAVG] = true;
        }
        break;
    }
}

void FilteredDataPlot::setTimerInterval(double ms)
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

void FilteredDataPlot::newReading(MoteReading r)
{
    currentReading = r;
}

dataFilter* FilteredDataPlot::getDataFilter()
{
    return filter;
}

void FilteredDataPlot::autoScale()
{
    if(auto_scale)
        auto_scale = false;
    else
        auto_scale = true;
}

//! \remark This gets called to update the graph.
void FilteredDataPlot::timerEvent(QTimerEvent *)
{
    if(!running)
        return;

    // Get new reading
    this->newReading(mySource->getReading(this->id));

        for ( int j = 0; j < FPLOT_SIZE - 1; j++ )
        {
            d_mgx[j] = d_mgx[j+1];
            d_avg[j] = d_avg[j+1];
            d_f[j] = d_f[j+1];
            d_e[j] = d_e[j+1];
            d_mavg[j] = d_mavg[j+1];

        }


        d_mgx[FPLOT_SIZE -1] = currentReading.getMgx()*1000;
        d_avg[FPLOT_SIZE -1] = filter->getLocalAverage();
        d_f[FPLOT_SIZE -1] = filter->getFilteredData(currentReading.getValue(myType));
        d_e[FPLOT_SIZE -1] = filter->getE();
        d_mavg[FPLOT_SIZE -1] = filter->getMovingAverage();

        //! \todo Make this scale more intelligently.
        if(auto_scale)
        {
            if(attached[MGX])
            {
                if(currentReading.getMgx() > max)
                {
                    max = currentReading.getMgx();
                }
                if(currentReading.getMgx() < min)
                {
                    min = currentReading.getMgx();
                }
            }
            if(attached[MGY])
            {
                if(currentReading.getMgy() > max)
                {
                    max = currentReading.getMgy();
                }
                if(currentReading.getMgy() < min)
                {
                    min = currentReading.getMgy();
                }
            }
            if(attached[ACCX])
            {
                if(currentReading.getAccx() > max)
                {
                    max = currentReading.getAccx();
                }
                if(currentReading.getAccx() < min)
                {
                    min = currentReading.getAccx();
                }
            }
            if(attached[ACCY])
            {
                if(currentReading.getAccy() > max)
                {
                    max = currentReading.getAccy();
                }
                if(currentReading.getAccy() < min)
                {
                    min = currentReading.getAccy();
                }
            }
            if(attached[TEMP])
            {
                if(currentReading.getTemp() > max)
                {
                    max = currentReading.getTemp();
                }
                if(currentReading.getTemp() < min)
                {
                    min = currentReading.getTemp();
                }
            }
            if(attached[IR])
            {
                if(currentReading.getIr() > max)
                {
                    max = currentReading.getIr();
                }
                if(currentReading.getIr() < min)
                {
                    min = currentReading.getIr();
                }
            }
            if(attached[VL])
            {
                if(currentReading.getVl() > max)
                {
                    max = currentReading.getVl();
                }
                if(currentReading.getVl() < min)
                {
                    min = currentReading.getVl();
                }
            }
            if(attached[MIC])
            {
                if(currentReading.getMic() > max)
                {
                    max = currentReading.getMic();
                }
                if(currentReading.getMic() < min)
                {
                    min = currentReading.getMic();
                }
            }

            reRange();
        }
       // update the display
    replot();

    }
