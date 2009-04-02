#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qspecserver.h"

class QxrdApplication;
class QxrdAcquisitionThread;

#include <QScriptEngine>

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

public:
  QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent=0);

signals:
  void printMessage(QString msg);

protected:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);

private:
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
  QxrdAcquisitionThread    *m_AcquisitionThread;
  QScriptEngine             m_ScriptEngine;
};

#endif
