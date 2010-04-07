//! Implementation of the dataFilter class
/*! \file datafilter.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "datafilter.h"
#include <QDebug>
#include <cmath>

dataFilter::dataFilter()
{
    // Initialize values
    localAverage = 0.0;
    rawData = 0.0;
    e = 0.0;
    movingAverage = 0.0;
    localMin = 0.0;
    localMax = 0.0;
    last_result = 0.0;
    parameter_t = CONSTANT_T;
    parameter_n = CONSTANT_N;
    parameter_m = CONSTANT_M;
    delta_u = MIN_DELTA_U;
    for(int i = 0; i < NUM_READINGS; i++)
        readings[i] = 0.0;
    for(int i = 0; i < NUM_STATES; i++)
        stateCounter[i] = 0;
    rc = 5.0;
    dt = 1.0;
    state = flat;

}

double dataFilter::getFilteredData(double d)
{
    double result = 0.0;
    double sum = 0.0;
    rawData = d;
    localAverage = 0.9*localAverage + 0.1 * rawData;
    e = pow((rawData-localAverage),2.0);
    // Low pass filter

    double alpha = dt/(rc + dt);
    result = alpha * e + (1-alpha) * last_result;


    for(int i = 0; i < NUM_READINGS - 1; i++)
    {
        readings[i] = readings[i+1];
        sum+=readings[i];
    }

    readings[NUM_READINGS-1] = result;
    sum+=readings[NUM_READINGS-1];
    movingAverage = sum/NUM_READINGS;
    last_result = result;

  //  qDebug() << "State = " << state << " stateCounter = " << stateCounter[state] << " getSlope() = " << getSlope();
    //qDebug() << " slope = " << readings[NUM_READINGS-1] << " - " << readings[NUM_READINGS-2] << " = " << readings[NUM_READINGS-1] - readings[NUM_READINGS-2];
    minMaxDetection();
    return result;

}

bool dataFilter::minMaxDetection()
{
    int slope = getSlope();
   // flat
   if(state == flat && slope == positive)
    {
       state = flat_count_up;
       stateCounter[flat_count_up] = 0;
    }

   // flat_count_up
   if(state == flat_count_up && slope == positive && stateCounter[flat_count_up] < parameter_n)
   {
       stateCounter[flat_count_up]= stateCounter[flat_count_up]+1;
   }

   // The state machine jumps from flat_count_up to flat_count_down if the current slope is not positive
   if(state == flat_count_up && slope == negative)
   {
       state = flat_count_down;
       stateCounter[flat_count_down] = 0;
   }
   // The state machine can jump from flat_count_up to hill_count_up when the slope is positive and the
   // counter for flat_count_up has value not less than n.
   if(state == flat_count_up && slope == positive && stateCounter[flat_count_up] >= parameter_n)
   {
       state = hill_count_up;
       stateCounter[hill_count_up] = 0;
   }

   // flat_count_down

   // The state machine stays at flat_count_down and the counter increments if the slope is not positive.
   if(state == flat_count_down && slope == negative && stateCounter[flat_count_down] < parameter_m)
   {
       stateCounter[flat_count_down] = stateCounter[flat_count_down] + 1;
   }

   // The state machine jumps back to flat if the slope if not positive and the counter
   // has value not less than M
   if(state == flat_count_down && slope == negative && stateCounter[flat_count_down] >= parameter_m)
   {
       state = flat;
       stateCounter[flat] = 0;
   }

   // The state machine jumps from flat_count_down to flat_count_up if the slope is positive
   if(state == flat_count_down && slope == positive)
   {
       state = flat_count_up;
       stateCounter[flat_count_up] = 0;
   }

   // hill_count_up

   // When the state machine enters hill_count_up, the state machine keeps this state if the slope
   // is not negative
   if(state == hill_count_up && slope == positive)
   {
       stateCounter[hill_count_up] = stateCounter[hill_count_up] + 1;
   }

   // If the state is hill_count_up and the slope is negative the state machine jumps to
   // hill_count_down and the counter at this state resets.
   if(state == hill_count_up && slope == negative)
   {
       state = hill_count_down;
       stateCounter[hill_count_down] = 0;
   }

   // hill_count_down

   // The state machine stays at this state and the counter counts up if the slope if engative and
   // the counter has value less than M
   if(state == hill_count_down && slope == negative && stateCounter[hill_count_down] < parameter_m)
   {
       stateCounter[hill_count_down] = stateCounter[hill_count_down]+1;
   }

   // The state machine will jump back to hill_count_up if the slope is not negative
   if(state == hill_count_down && slope == positive)
   {
       state = hill_count_up;
       stateCounter[hill_count_up] = 0;
   }

   // The state machine will jump back to flat state if the slope is negative and the counter
   // has a value not less than M

   if(state == hill_count_down && slope == negative && stateCounter[hill_count_down] >= parameter_m)
   {
       if(doLocalMax() - doLocalMin() > 25)
           qDebug() << "Vehicle Detected";
       state = flat;
       stateCounter[flat] = 0;
   }

return false;
}
// u[n] =

double dataFilter::getSlope()
{
    int result = positive;
    if(delta_u < fabs(readings[NUM_READINGS-1] - readings[NUM_READINGS-2]))
        // Return 0 for negative, 1 for positive
        result = ((readings[NUM_READINGS-1] - readings[NUM_READINGS-2]) > 0)?positive:negative;
    else
        result = positive;
    return result;

}

/*! local min function
  *
  *                   min(f[n] - localMin[n-1] if state == flat or flat_count_down
  *   localMin[n] =   f[n] if state == hill_count_down and localMax[n] - localMin[n] > parameter_t
  *                   unchanged otherwise
  */
double dataFilter::doLocalMin()
{

    double result = 0.0;
    if(state == flat || state == flat_count_down)
        result = min(readings[NUM_READINGS-1], localMin);

    else if(state == hill_count_down && (doLocalMax() - readings[NUM_READINGS-1] > parameter_t))
        result = readings[NUM_READINGS-1];
    else
        result = localMin;

    localMin = result;
    return result;
}

double dataFilter::doLocalMax()
{
    double result = 0.0;
    if(state == hill_count_up)
        result = max(readings[NUM_READINGS-1], localMax);
    else
        result = localMax;

    localMax = result;
    return result;
}

