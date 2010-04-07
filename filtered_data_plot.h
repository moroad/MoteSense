//! Widget to graph data
/*! \file filtered_data_plot.h
  * \class FilteredFilteredDataPlot
  * \author Jharrod LaFon
  * \date Spring 2010
  * \remarks This class is used to plot data on a moving canvas.  The data
  * are acquired from an instance of the packet handler class.
  */
#ifndef _FILTERED_DATA_PLOT_H
#define _FILTERED_DATA_PLOT_H 1
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
#include "datafilter.h"

//! Size of the plot
const int FPLOT_SIZE = 201;


//! The widget class that graphs the data
class FilteredDataPlot : public QwtPlot
{
    Q_OBJECT

public:
    //! Default constructor
    FilteredDataPlot(QWidget* = NULL);
    /*! Sets the dataGraph's id.  Used to demultiplex mote readings.
     *  \param i
     *  \return void
     */
    void setId(int i) { id = i; setTitle(QString("Sensors for Node %1").arg(id)); }

    /*! Mutator for packetHander
     *  Use this to set the packetHandler (server) for this plot
     * \param p Pointer to the packetHandler
     */
    void setSource(packetHandler* p){ mySource = p; running = true; }
    void setType(int t){ myType = t; }
    dataFilter *getDataFilter();

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
    /*! Slot for detaching Average
     */
    void detachAvg(){ detach(AVG); }
    /*! Slot for detaching Squared difference E
     */
    void detachE(){ detach(E); }
    /*! Slot for detaching F[n]
     */
    void detachF(){ detach(F); }
    /*! Slot for detaching Moving Average
     */
    void detachMavg(){ detach(MAVG); }

    /*! Slot for detaching a data type.
      * \param item is from the enumeration defined in filtered_data_plot.h
      * \note This is actually a bad name.  If the graph is already detached,
      * it reattaches the graph.
      */
    void detach(int item);

    /*! Enables or disables autoscaling
      */
    void autoScale();

    /*! Resets the viewing window of the graph
      */
    void reRange();



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
    double d_x[FPLOT_SIZE];

    //! Magnetometer x
    double d_mgx[FPLOT_SIZE];

    //! Local Average x
    double d_avg[FPLOT_SIZE];

    //! Squared Difference E
    double d_e[FPLOT_SIZE];

    //! Function F
    double d_f[FPLOT_SIZE];

    //! Moving Average
    double d_mavg[FPLOT_SIZE];

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

    int myType;
    bool plotMgx, plotAvg, plotE, plotF, plotMavg;
    bool auto_scale;

    QwtPlotCurve *mgxPlot;
    QwtPlotCurve *avgPlot;
    QwtPlotCurve *ePlot;
    QwtPlotCurve *fPlot;
    QwtPlotCurve *mavgPlot;

    //! The state of each graph (attached or not)
    bool attached[11];
    dataFilter *filter;
};

#endif
