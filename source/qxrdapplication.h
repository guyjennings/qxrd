/******************************************************************
*
*  $Id: qxrdapplication.h,v 1.2 2010/09/13 20:00:39 jennings Exp $
*
*******************************************************************/

#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include "qcepmacros.h"

#include <QApplication>
#include <QScriptEngine>
#include <QScriptEngineDebugger>

#include "qxrdforwardtypes.h"
#include "qxrddataprocessor.h"

class QxrdApplication : public QApplication
{
  Q_OBJECT;

public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisitionThreadPtr acquisitionThread();
  QxrdAcquisitionPtr acquisition() const;
  QxrdWindowPtr window();
  QxrdDataProcessorPtr dataProcessor() const;

  void loadPlugins();

  void readSettings();
  void writeSettings();

  void tiffWarning(const char* module, const char *msg);
  void tiffError(const char* module, const char *msg);

  static QxrdApplication* application();

public slots:
  void shutdownThreads();
  void possiblyQuit();
  void editPreferences();
  void debugChanged(int newValue);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public:
  bool wantToQuit();

private:
  void setupTiffHandlers();

public:
  Q_PROPERTY(int    detectorType  READ get_DetectorType WRITE set_DetectorType);
  QCEP_INTEGER_PROPERTY(DetectorType);

  Q_PROPERTY(int    processorType READ get_ProcessorType WRITE set_ProcessorType);
  QCEP_INTEGER_PROPERTY(ProcessorType);

  Q_PROPERTY(int    debug         READ get_Debug WRITE set_Debug);
  QCEP_INTEGER_PROPERTY(Debug);

  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer);
  QCEP_INTEGER_PROPERTY(RunSpecServer);

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort);
  QCEP_INTEGER_PROPERTY(SpecServerPort);

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer);
  QCEP_INTEGER_PROPERTY(RunSimpleServer);

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort);
  QCEP_INTEGER_PROPERTY(SimpleServerPort);

private:
  QxrdWindowPtr                   m_Window;
  QxrdServerThreadPtr             m_ServerThread;
  QxrdServerPtr                   m_Server;
  QxrdSimpleServerThreadPtr       m_SimpleServerThread;
  QxrdSimpleServerPtr             m_SimpleServer;
  QxrdDataProcessorThreadPtr      m_DataProcessorThread;
  QxrdDataProcessorPtr            m_DataProcessor;
  QxrdAcquisitionThreadPtr        m_AcquisitionThread;
  QxrdAcquisitionPtr              m_Acquisition;
  QxrdAllocatorThreadPtr          m_AllocatorThread;
  QxrdAllocatorPtr                m_Allocator;
  QxrdFileSaverThreadPtr          m_FileSaverThread;
  QxrdScriptEngineThreadPtr       m_ScriptEngineThread;
  QxrdScriptEnginePtr             m_ScriptEngine;
  QScriptEngineDebugger          *m_ScriptEngineDebugger;

  HEADER_IDENT("$Id: qxrdapplication.h,v 1.2 2010/09/13 20:00:39 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdapplication.h,v $
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.4  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.3  2010/08/02 21:09:51  jennings
*  Disabled script debugger
*
*  Revision 1.1.2.2  2010/07/22 23:20:58  jennings
*  Attempting to load plugins at startup
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.25.4.11  2010/06/09 19:19:13  jennings
*  Removed references to the QxrdFileSaver - all public accesses now through QxrdFileSaverThread
*
*  Revision 1.25.4.10  2010/05/22 04:28:53  jennings
*  *** empty log message ***
*
*  Revision 1.25.4.9  2010/05/21 20:04:42  jennings
*  Detector and Processor type names and numbers are intialized by the corresponding thread object
*
*  Revision 1.25.4.8  2010/05/21 18:30:32  jennings
*  Implemented preferences dialog allowing choise of detector type, processor type and debug level
*
*  Revision 1.25.4.7  2010/05/20 20:17:57  jennings
*  Added Allocator and FileSaver threads
*
*  Revision 1.25.4.6  2010/05/17 21:06:32  jennings
*  Various PE related mods
*
*  Revision 1.25.4.5  2010/05/17 15:59:53  jennings
*  Changed debugging output to use QCEP_DEBUG macro
*
*  Revision 1.25.4.4  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.25.4.3  2010/04/26 20:53:25  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.25.4.2  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.25.4.1  2010/04/21 19:51:06  jennings
*  Added application preferences to choose what type of detector and what type of processor to use
*
*  Revision 1.25  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.24  2009/09/10 21:33:30  jennings
*  Added TIFF error handling
*
*  Revision 1.23  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.22  2009/06/28 11:21:58  jennings
*  Implemented app scripting engine connections
*
*  Revision 1.21  2009/06/28 04:00:39  jennings
*  Partial implementation of separate thread for script engine
*
*  Revision 1.20  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

