/******************************************************************
*
*  $Id: qxrddataprocessorthread.cpp,v 1.3 2009/08/09 14:38:27 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(/*QxrdWindow *win, */QxrdAcquisition *acq)
  : QThread(),
    m_DataProcessor(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessorthread.cpp,v 1.3 2009/08/09 14:38:27 jennings Exp $")
{
  m_DataProcessor = new QxrdDataProcessor(/*win, */acq, NULL);
  m_DataProcessor -> moveToThread(this);
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();


  delete m_DataProcessor;
}

void QxrdDataProcessorThread::run()
{
//  m_DataProcessor -> initialize();

  exec();
}

void QxrdDataProcessorThread::shutdown()
{
  exit();

  wait(1000);
}

QxrdDataProcessor *QxrdDataProcessorThread::dataProcessor() const
{
  return m_DataProcessor;
}

/******************************************************************
*
*  $Log: qxrddataprocessorthread.cpp,v $
*  Revision 1.3  2009/08/09 14:38:27  jennings
*  Fixed a couple of cvs Id keyword typos
*
*  Revision 1.2  2009/07/13 23:19:37  jennings
*  More acquisition rearrangement
*
*  Revision 1.1  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*
*******************************************************************/
