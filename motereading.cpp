//! Implementation of the MoteReading class
/*! \file motereading.cpp
  * \author Jharrod LaFon
  * \date Spring 2010
  */
#include "motereading.h"

//! \todo Create copy constructor

MoteReading::MoteReading()
{

}


MoteReading::MoteReading(int m_id, int m_mgx, int m_mgy, int m_accx, int m_accy, int m_temp, int m_ir, int m_mic, int m_vl, int s)
 {

     id = m_id;
     mgx = m_mgx;
     mgy = m_mgy;
     accx = m_accx;
     accy = m_accy;
     temp = m_temp;
     ir = m_ir;
     mic = m_mic;
     vl = m_vl;
     seq = s;
 }
