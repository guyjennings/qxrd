#ifndef QXRDAPPLICATION_H
#define QXRDAPPLICATION_H

#include <QApplication>
#include <QScriptEngine>

class QxrdWindow;
class QxrdServer;
class QxrdAcquisitionThread;

class QxrdApplication : public QApplication
{
  Q_OBJECT;

 public:
  QxrdApplication(int &argc, char **argv);
  ~QxrdApplication();

  QxrdAcquisitionThread *acquisitionThread();
  QScriptValue evaluate(QString cmd);
  QxrdWindow *window();

 public slots:
  void shutdownThreads();
  void serverRunning();
  void acquisitionRunning();

  void possiblyQuit();

  void executeCommand(QString cmd);

//  int acquire();
//  int acquisitionStatus(double time);

 signals:
  void finishedCommand(QString cmd);
  void printMessage(QString msg);

 public:
  bool wantToQuit();

  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue subframesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSubframesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue framesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filenameFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue directoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);

 private:
  QxrdWindow                    *m_Window;
  QxrdServer                    *m_Server;
  QxrdAcquisitionThread         *m_AcquisitionThread;
  QScriptEngine                  m_ScriptEngine;
};

#endif
