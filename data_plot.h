//! Widget to graph data
/*! \file data_plot.h
  * \class DataPlot
  * \author Jharrod LaFon
  * \date Spring 2010
  * \remarks This class is used to plot data on a moving canvas.  The data
  * are acquired from an instance of the packet handler class.
  */
#ifndef _DATA_PLOT_H
#define _DATA_PLOT_H 1
#include <qwt_plot.h>
#include <QDebug>
#include <qwt_painter.h>
#include <qwt_plot_canvas.h>
#include <qwt_plot_marker.h>
#include <qwt_plot_curve.h>
#include <qwt_scale_widget.h>
#include <qwt_legend.h>
#include <qwt_scale_draw.h>
#include <qwt_math.h>
#include "packethandler.h"
#include "motereading.h"

//! Size of the plot
const int PLOT_SIZE = 201;

//! Used for turning them on or off on the plot
enum { MGX, MGY, ACCX, ACCY, IR, TEMP, VL, MIC };


//! The widget class that graphs the data
class DataPlot : public QwtPlot
{
    Q_OBJECT

public:
    //! Default constructor
    DataPlot(QWidget* = NULL);
    /*! Sets the dataGraph's id.  Used to demultiplex mote readings.
     *  \param i
     *  \return void
     */
    void setId(int i) { id = i; }

    /*! Mutator for packetHander
     *  Use this to set the packetHandler (server) for this plot
     * \param p Pointer to the packetHandler
     */
    void setSource(packetHandler* p){ mySource = p; running = true; }

public slots:
    /*! Slot for changing the timer frequency
     * \param interval The frequency of sampling
     */

    void setTimerInterval(double interval);

    /*! Slot for setting the upper range of the graph
     * \param range The upper bound of the viewing window
     */
    void setRange(double range);

    /*! Slot for setting the lower range of the graph
     * \param range is the lower bound of the viewing window
     */
    void setMinRange(double range);

    /*! Slot for informing the graph of a new reading
      * \param r The new mote reading
      */
    void newReading(MoteReading r);

    /*! Slot for notifying the datagraph that it's been disconnected from the packet source
     */
    void disconnected();

    /*! Slot for detaching MGx
     */
    void detachMgx(){ detach(MGX); }
    /*! Slot for detaching MGy
     */
    void detachMgy(){ detach(MGY); }
    /*! Slot for detaching ACCX
     */
    void detachAccx(){ detach(ACCX); }
    /*! Slot for detaching ACCY
     */
    void detachAccy(){ detach(ACCY); }
    /*! Slot for detaching IR
     */
    void detachIr(){ detach(IR); }
    /*! Slot for detaching Temp
     */
    void detachTemp(){ detach(TEMP); }
    /*! Slot for detaching VL
     */
    void detachVl(){ detach(VL); }
    /*! Slot for detaching MIC
     */
    void detachMic(){ detach(MIC); }

    /*! Slot for detaching a data type.
      * \param item is from the enumeration defined in data_plot.h
      */
    void detach(int item);

protected:
    /*! Timer event function */
    virtual void timerEvent(QTimerEvent *e);

private:
    /*! Needed for qwt library */
    void alignScales();

    //! This is the data source.
    packetHandler * mySource;

    //! This is the most recently acquired data reading
    MoteReading currentReading;

    //! Independant variable (x axis)
    double d_x[PLOT_SIZE];

    //! Magnetometer x
    double d_mgx[PLOT_SIZE];

    //! Magnetometer y
    double d_mgy[PLOT_SIZE];

    //! Accelerometer x
    double d_accx[PLOT_SIZE];

    //! Accelerometer y
    double d_accy[PLOT_SIZE];

    //! Temperature
    double d_temp[PLOT_SIZE];

    //! Infrared
    double d_ir[PLOT_SIZE];

    //! Microphone
    double d_mic[PLOT_SIZE];

    //! Visual Light
    double d_vl[PLOT_SIZE];

    //! My id
    int id;

    //! This is true if connected to the source
    bool running;

    //! Timer interval
    int d_interval;

    //! Timer id
    int d_timerId;

    int min;
    int max;

    bool plotMgx, plotMgy, plotAccx, plotAccy, plotIr, plotTemp, plotVl, plotMic;

    QwtPlotCurve *mgxPlot;
    QwtPlotCurve *mgyPlot;
    QwtPlotCurve *AccyPlot;
    QwtPlotCurve *AccxPlot;
    QwtPlotCurve *tempPlot;
    QwtPlotCurve *irPlot;
    QwtPlotCurve *micPlot;
    QwtPlotCurve *vlPlot;

    bool attached[8];
};

#endif
