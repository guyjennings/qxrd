/******************************************************************
*
*  $Id: qxrdacquisitionscripting.cpp,v 1.4 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>

QxrdAcquisitionScripting *QxrdAcquisitionScripting::g_Acquisition = NULL;

QxrdAcquisitionScripting::QxrdAcquisitionScripting(QxrdAcquisitionThread *thread)
  : QxrdAcquisitionParameters(),
    m_Mutex(QMutex::Recursive),
    m_AcquisitionThread(thread),
    SOURCE_IDENT("$Id: qxrdacquisitionscripting.cpp,v 1.4 2009/06/27 22:50:32 jennings Exp $")
{
  g_Acquisition = this;

//  printf("QxrdAcquisitionScripting::QxrdAcquisitionScripting g_Acquisition = %p\n",
//         g_Acquisition);

  m_ScriptEngine.globalObject().setProperty("acquisition",
          m_ScriptEngine.newQObject(this));

  m_ScriptEngine.globalObject().setProperty("acquire",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::acquireFunc));

  m_ScriptEngine.globalObject().setProperty("acquireDark",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::acquireDarkFunc));

  m_ScriptEngine.globalObject().setProperty("status",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::statusFunc));

  m_ScriptEngine.globalObject().setProperty("exposureTime",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::exposureTimeFunc));

  m_ScriptEngine.globalObject().setProperty("readoutMode",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::readoutModeFunc));

  m_ScriptEngine.globalObject().setProperty("summedExposures",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::summedExposuresFunc));

  m_ScriptEngine.globalObject().setProperty("darkSummedExposures",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::darkSummedExposuresFunc));

  m_ScriptEngine.globalObject().setProperty("filesInSequence",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::filesInSequenceFunc));

  m_ScriptEngine.globalObject().setProperty("filePattern",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::filePatternFunc));

  m_ScriptEngine.globalObject().setProperty("outputDirectory",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::outputDirectoryFunc));

  m_ScriptEngine.globalObject().setProperty("fileIndex",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::fileIndexFunc));

  m_ScriptEngine.globalObject().setProperty("print",
          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::printFunc));
//
//  m_ScriptEngine.globalObject().setProperty("SetCameraMode",
//          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::setCameraModeFunc));
//
//  m_ScriptEngine.globalObject().setProperty("SetFrameSyncMode",
//          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::setFrameSyncModeFunc));
//
//  m_ScriptEngine.globalObject().setProperty("SetTimerSync",
//          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::setTimerSyncFunc));
//
//  m_ScriptEngine.globalObject().setProperty("SetCameraGain",
//          m_ScriptEngine.newFunction(QxrdAcquisitionScripting::setCameraGainFunc));
}

QxrdAcquisitionThread *QxrdAcquisitionScripting::acquisitionThread() const
{
  QMutexLocker lock(&m_Mutex);

  return m_AcquisitionThread;
}

QScriptValue QxrdAcquisitionScripting::printFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();
  QString msg;

  for (int i=0; i<nArgs; i++) {
    if (i != 0) {
      msg += " ";
    }

    msg += context -> argument(i).toString();
  }

  emit g_Acquisition -> printMessage(msg);

  return QScriptValue(engine, 1);
}

QScriptValue QxrdAcquisitionScripting::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 4:
    g_Acquisition -> set_FilesInSequence(context -> argument(3).toUInt32());

  case 3:
    g_Acquisition -> set_SummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> set_ExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> set_FilePattern(context -> argument(0).toString());

  case 0:
    g_Acquisition -> acquire();
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdAcquisitionScripting::acquireDarkFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 3:
    g_Acquisition -> set_DarkSummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> set_ExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> set_FilePattern(context -> argument(0).toString());

  case 0:
    g_Acquisition -> acquireDark();
  }

  return QScriptValue(engine, 1);
}

QScriptValue QxrdAcquisitionScripting::statusFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() == 0) {
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(0));
  } else {
    double time = context->argument(0).toNumber();
    return QScriptValue(engine, g_Acquisition -> acquisitionStatus(time));
  }
}

QScriptValue QxrdAcquisitionScripting::readoutModeFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_ReadoutMode(context->argument(0).toInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_ReadoutMode());
}

QScriptValue QxrdAcquisitionScripting::exposureTimeFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_ExposureTime(context->argument(0).toNumber());
  }

  return QScriptValue(engine, g_Acquisition -> get_ExposureTime());
}

QScriptValue QxrdAcquisitionScripting::summedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_SummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_SummedExposures());
}

QScriptValue QxrdAcquisitionScripting::darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_DarkSummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_DarkSummedExposures());
}

QScriptValue QxrdAcquisitionScripting::filesInSequenceFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FilesInSequence(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_FilesInSequence());
}

QScriptValue QxrdAcquisitionScripting::filePatternFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FilePattern(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> get_FilePattern());
}

QScriptValue QxrdAcquisitionScripting::outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_OutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> get_OutputDirectory());
}

QScriptValue QxrdAcquisitionScripting::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> set_FileIndex(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> get_FileIndex());
}

QVariant QxrdAcquisitionScripting::evaluate(QString cmd)
{
//  emit printMessage(tr("Evaluate %1").arg(cmd));

  QScriptValue res = m_ScriptEngine.evaluate(cmd);

//  emit printMessage(tr("Result = %1").arg(res.toString()));

  m_AcquisitionThread -> setResult(res.toVariant());

  return res.toVariant();
}

double QxrdAcquisitionScripting::sleep(double time)
{
  acquisitionThread() -> sleep(time);

  return 42;
}

void QxrdAcquisitionScripting::message(QString msg)
{
  emit printMessage(msg);
  emit statusMessage(msg);
}

void QxrdAcquisitionScripting::propertyList()
{
  QMutexLocker lock(&m_Mutex);

  const QMetaObject *meta = metaObject();

  int count = meta->propertyCount();
  int offset = 1; /*meta->propertyOffset();*/

  for (int i=offset; i<count; i++) {
    QMetaProperty metaproperty = meta->property(i);

    const char *name = metaproperty.name();
    QVariant value = property(name);

    emit printMessage(tr("Property %1: %2 = %3").arg(i).arg(name).arg(value.toString()));
  }
}

/******************************************************************
*
*  $Log: qxrdacquisitionscripting.cpp,v $
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

