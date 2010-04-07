//! Window to display the filtered dataplot
/*!  \file filterwindow.h
  *  \class FilterWindow
  *  \author Jharrod LaFon
  *  \date Spring 2010
  *  \file filterwindow.h
  *  \brief Class to display a filtered dataplot widget in a window
  *
  */
#ifndef FILTERWINDOW_H
#define FILTERWINDOW_H
#include <qmainwindow.h>
#include <QString>
#include "filtered_data_plot.h"


//! Window class that contains the DataPlot widget
class FilterWindow: public QMainWindow
{

public:

    //! Default constructor
    FilterWindow();

    //! Sets the data source \remarks This also sets the source for the dataplot class.
    void setSource(packetHandler * p);

    //! Sets the ID of the Mote we are monitoring
    void setId(int i);

private:
    //! The dataplot
    FilteredDataPlot *plot; //!< This is the actual graphing widget >

    //! Mote ID
    int id;

};

#endif // FILTERWINDOW_H
