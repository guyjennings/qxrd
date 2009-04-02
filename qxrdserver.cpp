#include "qxrdserver.h"

#include <QHostAddress>
#include <QVariant>
#include <QScriptEngine>
#include "qxrdapplication.h"
#include "qxrdacquisitionthread.h"

static QxrdAcquisitionThread *g_AcquisitionThread;

QxrdServer::QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent)
  : QSpecServer(name, parent),
    m_AcquisitionThread(acqth)
{
  printf("QxrdServer::QxrdServer\n");

  g_AcquisitionThread = m_AcquisitionThread;

  m_ScriptEngine.globalObject().setProperty("acquire",
          m_ScriptEngine.newFunction(QxrdServer::acquireFunc));

  m_ScriptEngine.globalObject().setProperty("acquiredark",
          m_ScriptEngine.newFunction(QxrdServer::acquireDarkFunc));

  m_ScriptEngine.globalObject().setProperty("status",
          m_ScriptEngine.newFunction(QxrdServer::statusFunc));

  m_ScriptEngine.globalObject().setProperty("exposure",
          m_ScriptEngine.newFunction(QxrdServer::exposureFunc));

  m_ScriptEngine.globalObject().setProperty("subframes",
          m_ScriptEngine.newFunction(QxrdServer::subframesFunc));

  m_ScriptEngine.globalObject().setProperty("darksubframes",
          m_ScriptEngine.newFunction(QxrdServer::darkSubframesFunc));

  m_ScriptEngine.globalObject().setProperty("frames",
          m_ScriptEngine.newFunction(QxrdServer::framesFunc));

  m_ScriptEngine.globalObject().setProperty("filename",
          m_ScriptEngine.newFunction(QxrdServer::filenameFunc));

  m_ScriptEngine.globalObject().setProperty("directory",
          m_ScriptEngine.newFunction(QxrdServer::directoryFunc));

  m_ScriptEngine.globalObject().setProperty("fileindex",
          m_ScriptEngine.newFunction(QxrdServer::fileIndexFunc));

  startServer(QHostAddress::Any);
}

QVariant QxrdServer::executeCommand(QString cmd)
{
  QScriptValue result = m_ScriptEngine.evaluate(cmd);

  if (m_ScriptEngine.hasUncaughtException()) {
    return QVariant(tr("Error: ") + m_ScriptEngine.uncaughtException().toString());
  } else {
    return QVariant(result.toVariant());
  }
}

QVariant QxrdServer::readProperty(QString name)
{
  QVariant res;

  if (name=="test") {
    return 42;
  }

  return QSpecServer::readProperty(name);
}

QScriptValue QxrdServer::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_AcquisitionThread) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 4:
    g_AcquisitionThread -> setNFrames(context -> argument(3).toUInt32());

  case 3:
    g_AcquisitionThread -> setNSummed(context -> argument(2).toUInt32());

  case 2:
    g_AcquisitionThread -> setExposureTime(context -> argument(1).toNumber());

  case 1:
    g_AcquisitionThread -> setFilePattern(context -> argument(0).toString());

  case 0:
    g_AcquisitionThread -> doAcquire();
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdServer::acquireDarkFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_AcquisitionThread) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 3:
    g_AcquisitionThread -> setDarkNSummed(context -> argument(2).toUInt32());

  case 2:
    g_AcquisitionThread -> setExposureTime(context -> argument(1).toNumber());

  case 1:
    g_AcquisitionThread -> setFilePattern(context -> argument(0).toString());

  case 0:
    g_AcquisitionThread -> doAcquireDark();
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdServer::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_AcquisitionThread -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_AcquisitionThread -> acquisitionStatus(time));
  }
}

QScriptValue QxrdServer::exposureFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setExposureTime(context->argument(0).toNumber());
  }

  return QScriptValue(engine, g_AcquisitionThread -> exposureTime());
}

QScriptValue QxrdServer::subframesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setNSummed(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_AcquisitionThread -> nSummed());
}

QScriptValue QxrdServer::darkSubframesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setDarkNSummed(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_AcquisitionThread -> darkNSummed());
}

QScriptValue QxrdServer::framesFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setNFrames(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_AcquisitionThread -> nFrames());
}

QScriptValue QxrdServer::filenameFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setFilePattern(context->argument(0).toString());
  }

  return QScriptValue(engine, g_AcquisitionThread -> filePattern());
}

QScriptValue QxrdServer::directoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setOutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_AcquisitionThread -> outputDirectory());
}

QScriptValue QxrdServer::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_AcquisitionThread -> setFileIndex(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_AcquisitionThread -> fileIndex());
}

