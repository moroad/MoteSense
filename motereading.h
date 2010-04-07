//! Class to encapsulate sensor readings
/*! \file motereading.h
  * \class MoteReading
  * \author Jharrod LaFon
  * \date Spring 2010
  * \remarks Class to encapsulate mote readings.
  */
#ifndef MOTEREADING_H
#define MOTEREADING_H

#include <QObject>
#include <QString>
//! Enumeration for sensors
enum { MGX, MGY, ACCX, ACCY, IR, TEMP, VL, MIC, AVG, E, F, MAVG };

//! Class to encapsulate a sensor reading
class MoteReading
{
public:
    //! Default constructor.
    MoteReading();
    /*! Constructor, takes all readings as parameters
      * \param id, mgx, mgy, accx, accy, temp, ir, mic, vl
      */
    MoteReading(int id, int mgx, int mgy, int accx, int accy, int temp, int ir, int mic, int vl, int seq);

    /*! \brief Accessor for id
     * \returns id
     */
    int getId(){ return id; }

    /*! \brief Accessor for magnetometer x
     * \returns mgx
     */
    int getMgx(){ return mgx; }

    /*! \brief Accessor for magnetometer y
     * \returns mgy
     */
    int getMgy(){ return mgy; }

    /*! \brief Accessor for accelerometer y
     * \returns mgy
     */
    int getAccy(){ return accy; }

    /*! \brief Accessor for acclerometer x
     * \returns accx
     */
    int getAccx(){ return accx; }

    /*! \brief Accessor for temperature
     * \returns temp
     */
    int getTemp(){ return temp; }

    /*! \brief Accessor for infrared
     * \returns ir
     */
    int getIr(){ return ir; }

    /*! \brief Accessor for microphone
     * \returns mic
     */
    int getMic(){ return mic; }

    /*! \brief Accessor for visual light
     * \returns int visual_light
     */
    int getVl(){ return vl; }

    /*! \brief Accessor for sequence number
      * \returns int seq
      */
    int getSeq(){ return seq; }

    /*! \brief Accessor for value by enumeration.
      * If called with MGX, returns mgx, etc.
      * \param type type from the enum in motereading.h
      * \returns sensor reading
      */
    int getValue(int type);

    /*! \brief Mutator for id
     * \param i
     */
    void setId(int i){ id = i; }

    /*! \brief Accessor for string representation of reading
     * \returns QString
     */
    QString toString(){ return QString("%1 %2 %2 %4 %5 %6 %7 %8").arg(mgx).arg(mgy).arg(accx).arg(accy).arg(temp).arg(ir).arg(mic).arg(vl); }

private:

    //! \todo Create member for packet sequence number
    int id;
    int mgx;
    int mgy;
    int accx;
    int accy;
    int temp;
    int ir;
    int mic;
    int vl;
    int seq;
};

#endif // MOTEREADING_H
