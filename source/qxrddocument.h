#ifndef QXRDDOCUMENT_H
#define QXRDDOCUMENT_H

#include "qcepmacros.h"
#include "qcepproperty.h"

#include <QObject>
#include <QSharedPointer>
#include <QSplashScreen>
#include "qxrddataprocessorthread.h"
#include "qxrdacquisitionthread.h"
#include "qxrdserverthread.h"
#include "qxrdsimpleserverthread.h"
#include "qxrdfilesaverthread.h"
#include "qxrdsettingssaverthread.h"

class QxrdWindow;
class QxrdNIDAQPluginInterface;

class QxrdDocument : public QObject
{
  Q_OBJECT

public:
  QxrdDocument(QString path, QxrdApplication *app, QObject *parent = 0);
  virtual bool init(QSplashScreen *splash);
  virtual ~QxrdDocument();

  QxrdAcquisitionThread *acquisitionThread();
  QxrdAcquisition *acquisition() const;
  QxrdWindow *window();
  QxrdDataProcessor *dataProcessor() const;

  FILE* logFile();
  void newLogFile(QString path);

  FILE* scanFile();
  void newScanFile(QString path);

signals:

public slots:

  void readSettings();
  void writeSettings();
  void readSettings(QSettings &settings);
  void writeSettings(QSettings &settings);

  void splashMessage(const char *msg);
  void splashMessage(QString msg);

private:
  void logMessage(QString msg);
  void closeLogFile();
  void openLogFile();
  void writeLogHeader();

  void closeScanFile();
  void openScanFile();

public:  // Properties
  Q_PROPERTY(QString documentFilePath     READ get_DocumentFilePath WRITE set_DocumentFilePath)
  QCEP_STRING_PROPERTY(DocumentFilePath)

  Q_PROPERTY(QString logFilePath     READ get_LogFilePath WRITE set_LogFilePath)
  QCEP_STRING_PROPERTY(LogFilePath)

  Q_PROPERTY(QString scanFilePath     READ get_ScanFilePath WRITE set_ScanFilePath)
  QCEP_STRING_PROPERTY(ScanFilePath)

  Q_PROPERTY(int    detectorType  READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)

  Q_PROPERTY(int    processorType READ get_ProcessorType WRITE set_ProcessorType)
  QCEP_INTEGER_PROPERTY(ProcessorType)

  Q_PROPERTY(int    defaultLayout   READ get_DefaultLayout WRITE set_DefaultLayout STORED false)
  QCEP_INTEGER_PROPERTY(DefaultLayout)

  Q_PROPERTY(int    runSpecServer    READ get_RunSpecServer WRITE set_RunSpecServer)
  QCEP_INTEGER_PROPERTY(RunSpecServer)

  Q_PROPERTY(int    specServerPort    READ get_SpecServerPort WRITE set_SpecServerPort)
  QCEP_INTEGER_PROPERTY(SpecServerPort)

  Q_PROPERTY(int    runSimpleServer    READ get_RunSimpleServer WRITE set_RunSimpleServer)
  QCEP_INTEGER_PROPERTY(RunSimpleServer)

  Q_PROPERTY(int    simpleServerPort    READ get_SimpleServerPort WRITE set_SimpleServerPort)
  QCEP_INTEGER_PROPERTY(SimpleServerPort)

private:
  QSplashScreen                  *m_Splash;
  QxrdApplication                *m_Application;
  QxrdWindow                     *m_Window;
  QxrdServerThread               *m_ServerThread;
  QxrdServer                     *m_Server;
  QxrdSimpleServerThread         *m_SimpleServerThread;
  QxrdSimpleServer               *m_SimpleServer;
  QxrdDataProcessorThread        *m_DataProcessorThread;
  QxrdDataProcessor              *m_DataProcessor;
  QxrdAcquisitionThread          *m_AcquisitionThread;
  QxrdAcquisition                *m_Acquisition;
  QxrdFileSaverThread            *m_FileSaverThread;

  FILE                           *m_LogFile;
  FILE                           *m_ScanFile;
};

typedef QSharedPointer<QxrdDocument> QxrdDocumentPtr;

#endif // QXRDDOCUMENT_H
