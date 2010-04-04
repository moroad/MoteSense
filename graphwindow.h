//! Window to display the DataPlot
/*!  \file graphwindow.h
  *  \class GraphWindow
  *  \author Jharrod LaFon
  *  \date Spring 2010
  *  \file graphwindow.h
  *  \brief Class to display a dataplot widget in a window
  *
  */
#ifndef GRAPHWINDOW_H
#define GRAPHWINDOW_H
#include <qmainwindow.h>
#include <QString>
#include "data_plot.h"


//! Window class that contains the DataPlot widget
class GraphWindow: public QMainWindow
{

public:

    //! Default constructor
    GraphWindow();

    //! Sets the data source \remarks This also sets the source for the dataplot class.
    void setSource(packetHandler * p);

private:
    //! The dataplot
    DataPlot *plot; //!< This is the actual graphing widget >

};

#endif // GRAPHWINDOW_H
