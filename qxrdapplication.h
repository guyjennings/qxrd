#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include <QApplication>
#include <QScriptEngine>

class QxrdWindow;
class QxrdServerThread;
class QxrdAcquisitionThread;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

 public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisitionThread *acquisitionThread();
  QxrdServerThread      *serverThread();

 public slots:
  void shutdownThreads();
  void serverRunning();
  void acquisitionRunning();

  void printMessage(QString msg);
  void executeCommand(QString cmd);
  void newDataAvailable();

 signals:
  void finishedCommand(QString cmd);

 private:
  QxrdWindow                    *m_Window;
  QxrdServerThread              *m_ServerThread;
  QxrdAcquisitionThread         *m_AcquisitionThread;
  QScriptEngine                  m_ScriptEngine;
};

#endif
