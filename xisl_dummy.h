/******************************************************************
*
*  $Id: xisl_dummy.h,v 1.3 2009/11/30 19:32:00 jennings Exp $
*
*******************************************************************/

#ifndef XISL_DUMMY_H
#define XISL_DUMMY_H

#include "qcepmacros.h"

#include <QObject>
#include <QTimer>

class AcquisitionTimer : public QObject 
{
  Q_OBJECT;

 public:
  AcquisitionTimer();
  void start();
  void stop();
  void setmode(int m);
  void setintegration(int m);
  void setBuffers(unsigned short *buffer, int nFrames, int nRows, int nColumns);
  int noiseValue(int maxVal);

 public slots:
  void timeout();

 private:
  QTimer m_Timer;
  int    m_Mode;
  int    m_IntegrationTime;
  unsigned short *m_Buffer;
  int    m_NFrames;
  int    m_NRows;
  int    m_NColumns;
  int    m_CurrentFrame;

  HEADER_IDENT("$Id: xisl_dummy.h,v 1.3 2009/11/30 19:32:00 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: xisl_dummy.h,v $
*  Revision 1.3  2009/11/30 19:32:00  jennings
*  Implement a little bit more of the dummy XISL routines
*
*  Revision 1.2  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

