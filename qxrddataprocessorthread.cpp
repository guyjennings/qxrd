/******************************************************************
*
*  $Id: qxrddataprocessorthread.cpp,v 1.5 2009/10/01 21:43:44 jennings Exp $
*
*******************************************************************/

#include "qxrddataprocessorthread.h"

#include "qxrddataprocessor.h"

QxrdDataProcessorThread::QxrdDataProcessorThread(/*QxrdWindow *win, */QxrdAcquisition *acq)
  : QThread(),
    m_DataProcessor(NULL),
    SOURCE_IDENT("$Id: qxrddataprocessorthread.cpp,v 1.5 2009/10/01 21:43:44 jennings Exp $")
{
  m_DataProcessor = new QxrdDataProcessor(NULL, acq, NULL);
  m_DataProcessor -> moveToThread(this);
}

QxrdDataProcessorThread::~QxrdDataProcessorThread()
{
  shutdown();
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

  delete m_DataProcessor;
}

QxrdDataProcessor *QxrdDataProcessorThread::dataProcessor() const
{
  return m_DataProcessor;
}

/******************************************************************
*
*  $Log: qxrddataprocessorthread.cpp,v $
*  Revision 1.5  2009/10/01 21:43:44  jennings
*  Delete QxrdDataProcessor object at program exit
*
*  Revision 1.4  2009/08/27 21:02:17  jennings
*  Partial implementation of lazy plotting
*
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
