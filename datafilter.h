//! Filter to condition the signal
/*! \file datafilter.h
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#ifndef DATAFILTER_H
#define DATAFILTER_H

#include <QObject>

//! Number of readings to remember
#define NUM_READINGS 40

//! Positive constant
#define MIN_DELTA_U 3

//! Constant T
#define CONSTANT_T 70000

//! Constant N
#define CONSTANT_N 3

//! Constant M
#define CONSTANT_M 3

//! Number of states
#define NUM_STATES 5

//! States of the vehicle detector
enum states { flat, flat_count_up, flat_count_down, hill_count_up, hill_count_down };

//! Defines the sign of a number
enum signs { positive, negative };

//! Class to implement a filter
class dataFilter : public QObject
{
public:
    //! Default constructor
    dataFilter();

    //! Returns filtered data
    double getFilteredData(int c, double d);
    bool minMaxDetection();
    double getMovingAverage(){ return movingAverage; }
    double getE(){ return e; }

private:

    int state;
    int stateCounter[5];
    double min(double num1, double num2){ return (num1 < num2)?num1:num2; }
    double max(double num1, double num2){ return (num1 > num2)?num1:num2; }
    double getSlope();
    double doLocalMin();
    double doLocalMax();
    double rc;
    double dt;
    double localAverage;
    double localMin;
    double localMax;
    double rawData;
    double e;
    double movingAverage;
    double last_result;
    double readings[NUM_READINGS];

};

#endif // DATAFILTER_H
