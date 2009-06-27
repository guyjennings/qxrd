/******************************************************************
*
*  $Id: xisl_dummy.h,v 1.2 2009/06/27 22:50:33 jennings Exp $
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

 public slots:
  void timeout();

 private:
  QTimer m_Timer;
  int    m_Mode;
  HEADER_IDENT("$Id: xisl_dummy.h,v 1.2 2009/06/27 22:50:33 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: xisl_dummy.h,v $
*  Revision 1.2  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

