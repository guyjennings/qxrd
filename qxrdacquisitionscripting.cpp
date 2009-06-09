#include "qxrdacquisitionscripting.h"

#include "qxrdacquisitionthread.h"
#include <QMetaProperty>

QxrdAcquisitionScripting *QxrdAcquisitionScripting::g_Acquisition = NULL;

QxrdAcquisitionScripting::QxrdAcquisitionScripting(QxrdAcquisitionThread *thread)
  : inherited(),
    m_Mutex(QMutex::Recursive),
    m_AcquisitionThread(thread)
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

QScriptValue QxrdAcquisitionScripting::acquireFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (!g_Acquisition) return QScriptValue(engine, -1);

  int nArgs = context->argumentCount();

  switch (nArgs) {
  default:
  case 4:
    g_Acquisition -> setFilesInSequence(context -> argument(3).toUInt32());

  case 3:
    g_Acquisition -> setSummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> setExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> setFilePattern(context -> argument(0).toString());

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
    g_Acquisition -> setDarkSummedExposures(context -> argument(2).toUInt32());

  case 2:
    g_Acquisition -> setExposureTime(context -> argument(1).toNumber());

  case 1:
    g_Acquisition -> setFilePattern(context -> argument(0).toString());

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
    g_Acquisition -> setReadoutMode(context->argument(0).toInt32());
  }

  return QScriptValue(engine, g_Acquisition -> readoutMode());
}

QScriptValue QxrdAcquisitionScripting::exposureTimeFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setExposureTime(context->argument(0).toNumber());
  }

  return QScriptValue(engine, g_Acquisition -> exposureTime());
}

QScriptValue QxrdAcquisitionScripting::summedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setSummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> summedExposures());
}

QScriptValue QxrdAcquisitionScripting::darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setDarkSummedExposures(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> darkSummedExposures());
}

QScriptValue QxrdAcquisitionScripting::filesInSequenceFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setFilesInSequence(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> filesInSequence());
}

QScriptValue QxrdAcquisitionScripting::filePatternFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setFilePattern(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> filePattern());
}

QScriptValue QxrdAcquisitionScripting::outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setOutputDirectory(context->argument(0).toString());
  }

  return QScriptValue(engine, g_Acquisition -> outputDirectory());
}

QScriptValue QxrdAcquisitionScripting::fileIndexFunc(QScriptContext *context, QScriptEngine *engine)
{
  if (context->argumentCount() != 0) {
    g_Acquisition -> setFileIndex(context->argument(0).toUInt32());
  }

  return QScriptValue(engine, g_Acquisition -> fileIndex());
}

//QScriptValue QxrdAcquisitionScripting::setCameraModeFunc(QScriptContext *context, QScriptEngine *engine)
//{
//  if (context->argumentCount() != 0) {
//    g_Acquisition -> setCameraMode(context->argument(0).toInt32());
//  }
//
//  return QScriptValue(engine, g_Acquisition -> cameraMode());
//}
//
//QScriptValue QxrdAcquisitionScripting::setFrameSyncModeFunc(QScriptContext *context, QScriptEngine *engine)
//{
//  if (context->argumentCount() != 0) {
//    g_Acquisition -> setFrameSyncMode(context->argument(0).toInt32());
//  }
//
//  return QScriptValue(engine, g_Acquisition -> frameSyncMode());
//}
//
//QScriptValue QxrdAcquisitionScripting::setTimerSyncFunc(QScriptContext *context, QScriptEngine *engine)
//{
//  if (context->argumentCount() != 0) {
//    g_Acquisition -> setTimerSync(context->argument(0).toInt32());
//  }
//
//  return QScriptValue(engine, g_Acquisition -> timerSync());
//}
//
//QScriptValue QxrdAcquisitionScripting::setCameraGainFunc(QScriptContext *context, QScriptEngine *engine)
//{
//  if (context->argumentCount() != 0) {
//    g_Acquisition -> setCameraGain(context->argument(0).toInt32());
//  }
//
//  return QScriptValue(engine, g_Acquisition -> cameraGain());
//}

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
