#ifndef QXRDSCRIPTENGINE_H
#define QXRDSCRIPTENGINE_H

#include "qxrdlib_global.h"
#include <QObject>
#include <QScriptValue>
#include <QDateTime>
#include <QMutex>
#include <QRegExp>
#include <QPointF>

#include "qcepscriptengine.h"

#include "qxrdappcommon-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdacqcommon-ptr.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qceproi-ptr.h"

//TODO: replace by QxrdJSEngine
class QXRD_EXPORT QxrdScriptEngine : public QcepScriptEngine
{
  Q_OBJECT

private:
  typedef QcepScriptEngine inherited;

public:
  Q_INVOKABLE QxrdScriptEngine(QString name);
  virtual ~QxrdScriptEngine();
  void initialize(QcepObjectWPtr parent);

  void lock();
  void unlock();

  int  nextDataIndex();

public slots:
  void evaluateAppCommand(QString cmd);
  void evaluateSimpleServerCommand(QString cmd);
  void evaluateSpecCommand(QString cmd);
  void evaluateCommand(int index, QString cmd);
  void loadScript(QString path);

signals:
  void resultAvailable(int index, QScriptValue res);
  void appResultAvailable(QScriptValue res);
  void simpleServerResultAvailable(QScriptValue cmd);
  void specResultAvailable(QScriptValue cmd);

public:
  bool hasUncaughtException() const;
  int  uncaughtExceptionLineNumber() const;
  QString uncaughtExceptionString() const;
  void cancelCommand();

  QxrdAppCommonWPtr application() const;
  QxrdExperimentWPtr experiment() const;
  QxrdAcqCommonWPtr acquisition() const;
  QxrdProcessorWPtr processor() const;
  QxrdWindowWPtr window() const;

  void setWindow(QxrdWindowWPtr win);

public slots:
  static QString convertToString(QScriptValue result);

  void dumpLocks();

  void openScriptOutput(const QString& fileName);
  void writeScriptOutput(const QString& outputLine);
  void closeScriptOutput();

  QScriptValue roiFunc2(int n, int i);
  QcepROIPtr roiFunc3(int n, int i);

private slots:
  void              evaluateScript(int src, QString cmd);

private:
  static QString convertHelper(QScriptValue result, int depth);

private:
  static QScriptValue printFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue fopenFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue fdeleteFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue fprintFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue fcloseFunc(QScriptContext *context, QScriptEngine *engine, void *u);
  static QScriptValue acquireFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireDarkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireOnceFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue statusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processStatusFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireCancelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue acquireScalersFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue triggerFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue exposureTimeFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue summedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue skippedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkSummedExposuresFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue phasesInGroupFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue preTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue postTriggerFilesFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue filePatternFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue outputDirectoryFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue fileIndexFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue dataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue darkFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue maskFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue overflowFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue liveDataFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue helpFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue processFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue setFileNormalizationFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue typeNameFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue matchFilesFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue extraChannelFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue mapUserFunctionFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue timeStampFunc(QScriptContext *context, QScriptEngine *engine);

  static QScriptValue detectorFunc(QScriptContext *context, QScriptEngine *engine);
//  static QScriptValue detectorPluginFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue roiFunc(QScriptContext *context, QScriptEngine *engine);

//  static QScriptValue dataObjectFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newDataGroupFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newDataArrayFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newDataColumnFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newDataColumnScanFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newDataImageFunc(QScriptContext *context, QScriptEngine *engine);
  static QScriptValue newIntegratedDataFunc(QScriptContext *context, QScriptEngine *engine);

  static QScriptValue QPointFToScriptValue(QScriptEngine *engine, const QPointF &in);
  static void         QPointFFromScriptValue(const QScriptValue &object, QPointF &pt);

  static QScriptValue QRectFToScriptValue(QScriptEngine *engine, const QRectF &in);
  static void         QRectFFromScriptValue(const QScriptValue &object, QRectF &pt);

private:
  mutable QMutex         m_Mutex;
  QxrdAppCommonWPtr      m_Application;
  QxrdExperimentWPtr     m_Experiment;
  QxrdAcqCommonWPtr      m_Acquisition;
  QxrdProcessorWPtr      m_Processor;     //TODO: is this always appropriate?
  QxrdWindowWPtr         m_Window;
  FILE                  *m_ScriptOutput;
  int                    m_DataIndex;
};

Q_DECLARE_METATYPE(QxrdScriptEngine*)

#endif // QXRDSCRIPTENGINE_H
