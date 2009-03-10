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
  QScriptValue evaluate(QString cmd);

 public slots:
  void shutdownThreads();
  void serverRunning();
  void acquisitionRunning();

  void printMessage(QString msg);
  void executeCommand(QString cmd);
  void newDataAvailable();
  void doAcquire();
  void doCancel();
  void acquireComplete();
  void saveData();
  void loadData();

  int acquire();
  int acquisitionStatus(double time);

 signals:
  void finishedCommand(QString cmd);

 public:
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);

 private:
  QxrdWindow                    *m_Window;
  QxrdServerThread              *m_ServerThread;
  QxrdAcquisitionThread         *m_AcquisitionThread;
  QScriptEngine                  m_ScriptEngine;
  int                            m_Acquiring;
};

#endif
