//! Filter to condition the signal
/*! \file datafilter.h
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <QObject>

//! Class to implement a filter
class dataFilter : public QObject
{
public:
    //! Default constructor
    dataFilter();

    //! Returns filtered data
    double getFilteredData(int c, double d);

private:
    double rc;
    double trigger_threshold;
    double trigger_level;
    double scaling_factor;
    int clock[2];
    double last_result;

};

#endif // DATAFILTER_H
