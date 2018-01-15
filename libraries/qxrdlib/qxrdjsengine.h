#ifndef QXRDJSENGINE_H
#define QXRDJSENGINE_H

#include "qxrdlib_global.h"
#include <QtQml>
#include <QStringList>
#include "qxrdapplication-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdwindow-ptr.h"
#include "qxrdacquisition-ptr.h"
#include "qxrddataprocessor-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrddetectorsettings-ptr.h"
#include "qxrdacquisitionextrainputschannel-ptr.h"
#include "qcepdataarray-ptr.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qxrdroicoordinates-ptr.h"
#include "qcepdatagroup-ptr.h"
#include "qcepintegrateddata-ptr.h"

//TODO: replace QxrdScriptEngine with QxrdJSEngine
class QXRD_EXPORT QxrdJSEngine : public QQmlEngine
{
  Q_OBJECT

public:
  QxrdJSEngine(QxrdApplicationWPtr app,
               QxrdExperimentWPtr exp);
  virtual ~QxrdJSEngine();
  void initialize();
  void setWindow(QxrdWindowWPtr win);

public slots:
  void evaluateAppCommandJS(QString cmd);
  static QString convertToString(QJSValue result);

  void printFunc(QVariant v1=QVariant(),
                 QVariant v2=QVariant(),
                 QVariant v3=QVariant(),
                 QVariant v4=QVariant(),
                 QVariant v5=QVariant(),
                 QVariant v6=QVariant(),
                 QVariant v7=QVariant());

  void acquireFunc(QString fp=QString(),
                   double  tim=qQNaN(),
                   double  sum=qQNaN(),
                   double  post=qQNaN(),
                   double  pre=qQNaN(),
                   double  nph=qQNaN());

  void acquireDarkFunc(QString fp=QString(),
                       double  tim=qQNaN(),
                       double  sum=qQNaN());

  void acquireOnceFunc(QString fp=QString(),
                       double  tim=qQNaN(),
                       double  sum=qQNaN());

  int  statusFunc(double tim=0);
  int  acquireStatusFunc(double tim=0);
  void acquireCancelFunc();
  int  processStatusFunc(double tim=0);

  QJSValue acquireScalersFunc();
  double acquireScalersFunc(int i);
  void triggerFunc();

  double exposureTimeFunc();
  double exposureTimeFunc(double t);

  int summedExposuresFunc();
  int summedExposuresFunc(int n);

  int skippedExposuresFunc();
  int skippedExposuresFunc(int n);

  int darkSummedExposuresFuncs();
  int darkSummedExposuresFuncs(int n);

  int phasesInGroupFunc();
  int phasesInGroupFunc(int n);

  int preTriggerFilesFunc();
  int preTriggerFilesFunc(int n);

  int postTriggerFilesFunc();
  int postTriggerFilesFunc(int n);

  QString filePatternFunc();
  QString filePatternFunc(QString patt);

  QString outputDirectoryFunc();
  QString outputDirectoryFunc(QString d);

  int fileIndexFunc();
  int fileIndexFunc(int n);

  void fopenFunc(QString fn);
  void fdeleteFunc(QString p);
  void fprintFunc(QVariant v1=QVariant(),
                  QVariant v2=QVariant(),
                  QVariant v3=QVariant(),
                  QVariant v4=QVariant(),
                  QVariant v5=QVariant(),
                  QVariant v6=QVariant(),
                  QVariant v7=QVariant());
  void fcloseFunc();

  QcepImageDataBase*   dataFunc();
  QcepDoubleImageData* darkFunc();
  QcepMaskData*        maskFunc();
  QcepMaskData*        overflowFunc();
  QcepDoubleImageData* liveDataFunc();

  QString helpFunc(QString topic);

  void processFunc(QString fn, QVector<double> nm=QVector<double>());
  void setFileNormalizationFunc(QString fn, QVector<double> nm=QVector<double>());

  QStringList matchFilesFunc(QString patt);

  QxrdAcquisitionExtraInputsChannel* extraChannelFunc(int n);

  void mapUserFunctionFunc(QString func);
  double timeStampFunc();
  QJSValue detectorFunc(int n);
  QJSValue roiFunc(int n, int m);
  QJSValue calibrantFunc(int n);

  QcepDataGroup*       newDataGroupFunc(QString name);
  QcepDataArray*       newDataArrayFunc(QString name, int d1=0, int d2=0, int d3=0, int d4=0);
  QcepDataColumn*      newDataColumnFunc(QString name, int npts);
  QcepDataColumnScan*  newDataColumnScanFunc(QString name, QStringList cols);

  QcepDoubleImageData* newDataImageFunc(QString name, int width, int height);

  QcepIntegratedData*  newIntegratedDataFunc(QString name, int size);

signals:
  void appResultAvailableJS(QJSValue res);

private:
  static QString convertHelper(QJSValue result, int depth);

  void setGlobalProperty(QString name, QJSValue val, ObjectOwnership owner = CppOwnership);

  QxrdAcquisitionWPtr   acquisition();
  QxrdDataProcessorWPtr dataProcessor();

private:
  mutable QMutex         m_Mutex;
  QxrdApplicationWPtr    m_Application;
  QxrdExperimentWPtr     m_Experiment;
//  QxrdAcquisitionWPtr    m_Acquisition;
//  QxrdDataProcessorWPtr  m_DataProcessor;
  QxrdWindowWPtr         m_Window;
};

#endif // QXRDJSENGINE_H
