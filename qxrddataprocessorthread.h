/******************************************************************
*
*  $Id: qxrddataprocessorthread.h,v 1.1 2009/07/10 22:54:23 jennings Exp $
*
*******************************************************************/

#ifndef QXRDDATAPROCESSORTHREAD_H
#define QXRDDATAPROCESSORTHREAD_H

#include "qcepmacros.h"

#include <QThread>

class QxrdDataProcessor;
class QxrdWindow;
class QxrdAcquisition;

class QxrdDataProcessorThread : public QThread
{
  Q_OBJECT;

public:
  QxrdDataProcessorThread(QxrdWindow *win, QxrdAcquisition *acq);
  ~QxrdDataProcessorThread();

  void shutdown();

  QxrdDataProcessor *dataProcessor() const;

signals:
  void printMessage(QString msg);

protected:
  void run();

private:
  QxrdDataProcessor   *m_DataProcessor;
  HEADER_IDENT("$Id: qxrddataprocessorthread.h,v 1.1 2009/07/10 22:54:23 jennings Exp $");
};

#endif // QXRDDATAPROCESSORTHREAD_H

/******************************************************************
*
*  $Log: qxrddataprocessorthread.h,v $
*  Revision 1.1  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*
*******************************************************************/
