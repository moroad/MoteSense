//! Implementation of the dataFilter class
/*! \file datafilter.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "datafilter.h"
#include <QDebug>
dataFilter::dataFilter()
{
    // Initialize values
    clock[0] = clock[1] = 0;
    trigger_threshold = trigger_level = scaling_factor = 0;
    last_result = 1;
    rc = 5.0;
}

double dataFilter::getFilteredData(int c, double d)
{

    clock[1] = c;
    double dt = (double)(clock[1] - clock[0]);
    scaling_factor = double(dt /(rc + dt));
    double result = scaling_factor * d + (1.0 - scaling_factor) * last_result;
    //qDebug() << "scaling_factor = " << scaling_factor;
    //qDebug() << "dt = " << dt;
    //qDebug() << "Unfiltered = " << d << " Filtered = " << result;
    last_result = result;
    clock[0] = clock[1];
    return result;

}
