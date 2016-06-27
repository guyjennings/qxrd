#include "qxrdjsengine.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qxrddebug.h"
#include "qcepallocator.h"
#include "qxrdapplication.h"
#include "qxrddataprocessor.h"
#include "qxrdroicoordinates.h"
#include "qxrddetectorsettings.h"

QxrdJSEngine::QxrdJSEngine(QxrdApplicationWPtr app, QxrdExperimentWPtr exp) :
  m_Application(app),
  m_Experiment(exp)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdJSEngine::QxrdJSEngine(%p)\n", this);
  }
}

QxrdJSEngine::~QxrdJSEngine()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdJSEngine::~QxrdJSEngine(%p)\n", this);
  }

//  if (m_ScriptOutput) {
//    fclose(m_ScriptOutput);
//  }
}

void QxrdJSEngine::initialize()
{

  installExtensions(QJSEngine::AllExtensions);

  qRegisterMetaType< QVector<qreal> >("QVector<qreal>");

  qmlRegisterType<QxrdROICoordinates>();
  qmlRegisterType<QxrdDetectorSettings>();
//  qmlRegisterType<QPointF>();
//  qmlRegisterType<QRectF>();
//  qmlRegisterType<QPolygonF>();
//  qmlRegisterType<QVector<double> >();

  setGlobalProperty("global", globalObject());
  setGlobalProperty("scripting", newQObject(this));

  setGlobalProperty("acquire", newQObject(this).property("acquireFunc"));
  setGlobalProperty("acquireDark", newQObject(this).property("acquireDarkFunc"));
  setGlobalProperty("status", newQObject(this).property("statusFunc"));
  setGlobalProperty("acquireStatus", newQObject(this).property("acquireStatusFunc"));
  setGlobalProperty("processStatus", newQObject(this).property("processStatusFunc"));
  setGlobalProperty("acquireCancel", newQObject(this).property("acquireCancelFunc"));
  setGlobalProperty("acquireScalers", newQObject(this).property("acquireScalersFunc"));

  setGlobalProperty("trigger", newQObject(this).property("triggerFunc"));
  setGlobalProperty("exposureTime", newQObject(this).property("exposureTimeFunc"));
  setGlobalProperty("summedExposures", newQObject(this).property("summedExposuresFunc"));
  setGlobalProperty("skippedExposures", newQObject(this).property("skippedExposuresFunc"));
  setGlobalProperty("darkSummedExposures", newQObject(this).property("darkSummedExposuresFunc"));
  setGlobalProperty("phasesInGroup", newQObject(this).property("phasesInGroupFunc"));
  setGlobalProperty("preTriggerFiles", newQObject(this).property("preTriggerFilesFunc"));
  setGlobalProperty("postTriggerFiles", newQObject(this).property("postTriggerFilesFunc"));
  setGlobalProperty("filePattern", newQObject(this).property("filePatternFunc"));
  setGlobalProperty("outputDirectory", newQObject(this).property("outputDirectoryFunc"));
  setGlobalProperty("fileIndex", newQObject(this).property("fileIndexFunc"));
  setGlobalProperty("print", newQObject(this).property("printFunc"));
  setGlobalProperty("fopen", newQObject(this).property("fopenFunc"));
  setGlobalProperty("fdelete", newQObject(this).property("fdeleteFunc"));
  setGlobalProperty("fprint", newQObject(this).property("fprintFunc"));
  setGlobalProperty("fclose", newQObject(this).property("fcloseFunc"));
  setGlobalProperty("printMessage", newQObject(this).property("printFunc"));
  setGlobalProperty("data", newQObject(this).property("dataFunc"));
  setGlobalProperty("dark", newQObject(this).property("darkFunc"));
  setGlobalProperty("mask", newQObject(this).property("maskFunc"));
  setGlobalProperty("overflow", newQObject(this).property("overflowFunc"));
  setGlobalProperty("liveData", newQObject(this).property("liveDataFunc"));
  setGlobalProperty("help", newQObject(this).property("helpFunc"));
  setGlobalProperty("process", newQObject(this).property("processFunc"));
  setGlobalProperty("setFileNormalization", newQObject(this).property("setFileNormalizationFunc"));
  setGlobalProperty("matchFiles", newQObject(this).property("matchFilesFunc"));
  setGlobalProperty("extraChannel", newQObject(this).property("extraChannelFunc"));
  setGlobalProperty("mapUserFunction", newQObject(this).property("mapUserFunctionFunc"));
  setGlobalProperty("timeStamp", newQObject(this).property("timeStampFunc"));

  setGlobalProperty("detector", newQObject(this).property("detectorFunc"));
  setGlobalProperty("roi", newQObject(this).property("roiFunc"));

  setGlobalProperty("newDataGroup", newQObject(this).property("newDataGroupFunc"));
  setGlobalProperty("newDataArray", newQObject(this).property("newDataArrayFunc"));
  setGlobalProperty("newDataColumn", newQObject(this).property("newDataColumnFunc"));
  setGlobalProperty("newDataColumnScan", newQObject(this).property("newDataColumnScanFunc"));
  setGlobalProperty("newDataImage", newQObject(this).property("newDataImageFunc"));
  setGlobalProperty("newIntegratedData", newQObject(this).property("newIntegratedDataFunc"));

  QxrdApplicationPtr app(m_Application);

  if (app) {
//    QCEP_DOC_OBJECT("application", "The QXRD Application Object");
    setGlobalProperty("application", newQObject(app.data()));
  }

  if (g_Allocator) {
//    QCEP_DOC_OBJECT("allocator", "The QXRD Memory Allocator");
    setGlobalProperty("allocator", newQObject(g_Allocator));
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    setGlobalProperty("experiment", newQObject(expt.data()));

    QxrdAcquisitionPtr acq(expt->acquisition());

    if (acq) {
      setGlobalProperty("acquisition", newQObject(acq.data()));
    }
  }
}

void QxrdJSEngine::setWindow(QxrdWindowWPtr win)
{
}

void QxrdJSEngine::setGlobalProperty(QString name, QJSValue val, ObjectOwnership owner)
{
  globalObject().setProperty(name, val);

  setObjectOwnership(val.toQObject(), owner);
}

void QxrdJSEngine::evaluateAppCommandJS(QString cmd)
{
  THREAD_CHECK;

  QJSValue result = evaluate(cmd);

  if(qcepDebug(DEBUG_LOGSCRIPTS)) {
    QxrdExperimentPtr exp(m_Experiment);

    if (exp) {
      exp->printMessage(tr("Evaluate %1 = %2").arg(cmd).arg(result.toString()));
    }
  }

  emit appResultAvailableJS(result);
}

QString QxrdJSEngine::convertToString(QJSValue result)
{
  return convertHelper(result, 0);
}

QString QxrdJSEngine::convertHelper(QJSValue result, int depth)
{
  if (depth >= 4) {
    return "...";
  } else if (result.isError()) {
    return "ERROR : "+result.property("error").toString();
  } else if (result.isArray()) {
    int len = result.property("length").toInt();

    QString s = "[";

    for (int i=0; i<len; i++) {
      s += convertHelper(result.property(tr("%1").arg(i)), depth+1);

      if (i<len-1) {
        s += ", ";
      }
    }

    s += "]";

    return s;

  } else if (result.isObject()) {
    QJSValueIterator it(result);

    QString s = "{";

    while(it.hasNext()) {
      it.next();

      s += it.name()+":";
      s += convertHelper(it.value(), depth+1);

      if (it.hasNext()) {
        s += ", ";
      }
    }

    s += "}";

    return s;

  } else {
    return result.toString();
  }
}

QxrdAcquisitionWPtr QxrdJSEngine::acquisition()
{
  QxrdAcquisitionWPtr res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    res = expt->acquisition();
  }

  return res;
}

QxrdDataProcessorWPtr QxrdJSEngine::dataProcessor()
{
  QxrdDataProcessorWPtr res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    res = expt->dataProcessor();
  }

  return res;
}

void QxrdJSEngine::printFunc(QVariant v1,
                             QVariant v2,
                             QVariant v3,
                             QVariant v4,
                             QVariant v5,
                             QVariant v6,
                             QVariant v7)
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QString res;

    if (v1.isValid()) res += v1.toString();
    if (v2.isValid()) res += v2.toString();
    if (v3.isValid()) res += v3.toString();
    if (v4.isValid()) res += v4.toString();
    if (v5.isValid()) res += v5.toString();
    if (v6.isValid()) res += v6.toString();
    if (v7.isValid()) res += v7.toString();

    expt->printLine(res);
  }
}

void QxrdJSEngine::acquireFunc(QString fp,
                               double  tim,
                               double  sum,
                               double  post,
                               double  pre,
                               double  nph)
{
  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    if (fp.length()) {
      acq->set_FilePattern(fp);
    }

    if (tim==tim) {
      acq->set_ExposureTime(tim);
    }

    if (sum==sum) {
      acq->set_SummedExposures(sum);
    }

    if (post==post) {
      acq->set_PostTriggerFiles(post);
    }

    if (pre==pre) {
      acq->set_PreTriggerFiles(pre);
    }

    if (nph==nph) {
      acq->set_PhasesInGroup(nph);
    }

    acq->acquire();
  }
}

void QxrdJSEngine::acquireDarkFunc(QString fp,
                                   double  tim,
                                   double  sum)
{
  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    if (fp.length()) {
      acq->set_FilePattern(fp);
    }

    if (tim==tim) {
      acq->set_ExposureTime(tim);
    }

    if (sum==sum) {
      acq->set_DarkSummedExposures(sum);
    }
  }
}

int  QxrdJSEngine::statusFunc(double tim)
{
  int res = -1;

  QxrdAcquisitionPtr acq(acquisition());
  QxrdDataProcessorPtr proc(dataProcessor());

  if (acq && proc) {
    res = acq->acquisitionStatus(tim);

    if (res == 1) {
      res = proc->status(tim);
    }
  }

  return res;
}

int  QxrdJSEngine::acquireStatusFunc(double tim)
{
  int res = -1;

  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    res = acq->acquisitionStatus(tim);
  }

  return res;
}

void QxrdJSEngine::acquireCancelFunc()
{
  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    acq->cancel();
  }
}

int  QxrdJSEngine::processStatusFunc(double tim)
{
  int res = -1;

  QxrdDataProcessorPtr proc(dataProcessor());

  if (proc) {
    res = proc->status(tim);
  }

  return res;
}

QJSValue QxrdJSEngine::acquireScalersFunc()
{
  QJSValue res = newArray();

  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    QcepDoubleVector v = acq->get_ScalerValues();

    for (int i=0; i<v.length(); i++) {
      res.setProperty(i, v.value(i));
    }
  }

  return res;
}

double QxrdJSEngine::acquireScalersFunc(int i)
{
  double res;

  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    res = acq->scalerValue(i);
  }

  return res;
}

void QxrdJSEngine::triggerFunc()
{
}

double QxrdJSEngine::exposureTimeFunc()
{
  return qQNaN();
}

double QxrdJSEngine::exposureTimeFunc(double t)
{
  return qQNaN();
}

int QxrdJSEngine::summedExposuresFunc()
{
  return -1;
}

int QxrdJSEngine::summedExposuresFunc(int n)
{
  return -1;
}

int QxrdJSEngine::skippedExposuresFunc()
{
  return -1;
}

int QxrdJSEngine::skippedExposuresFunc(int n)
{
  return -1;
}

int QxrdJSEngine::darkSummedExposuresFuncs()
{
  return -1;
}

int QxrdJSEngine::darkSummedExposuresFuncs(int n)
{
  return -1;
}

int QxrdJSEngine::phasesInGroupFunc()
{
  return -1;
}

int QxrdJSEngine::phasesInGroupFunc(int n)
{
  return -1;
}

int QxrdJSEngine::preTriggerFilesFunc()
{
  return -1;
}

int QxrdJSEngine::preTriggerFilesFunc(int n)
{
  return -1;
}

int QxrdJSEngine::postTriggerFilesFunc()
{
  return -1;
}

int QxrdJSEngine::postTriggerFilesFunc(int n)
{
  return -1;
}

QString QxrdJSEngine::filePatternFunc()
{
  return "";
}

QString QxrdJSEngine::filePatternFunc(QString patt)
{
  return "";
}

QString QxrdJSEngine::outputDirectoryFunc()
{
  return "";
}

QString QxrdJSEngine::outputDirectoryFunc(QString d)
{
  return "";
}

int QxrdJSEngine::fileIndexFunc()
{
  return -1;
}

int QxrdJSEngine::fileIndexFunc(int n)
{
  return -1;
}

void QxrdJSEngine::fopenFunc(QString fn)
{
}

void QxrdJSEngine::fdeleteFunc(QString p)
{
}

void QxrdJSEngine::fprintFunc(QVariant v1,
                              QVariant v2,
                              QVariant v3,
                              QVariant v4,
                              QVariant v5,
                              QVariant v6,
                              QVariant v7)
{
}

void QxrdJSEngine::fcloseFunc()
{
}

QcepDoubleImageData* QxrdJSEngine::dataFunc()
{
  return NULL;
}

QcepDoubleImageData* QxrdJSEngine::darkFunc()
{
  return NULL;
}

QcepMaskData*        QxrdJSEngine::maskFunc()
{
  return NULL;
}

QcepMaskData*        QxrdJSEngine::overflowFunc()
{
  return NULL;
}

QcepDoubleImageData* QxrdJSEngine::liveDataFunc()
{
  return NULL;
}

QString QxrdJSEngine::helpFunc(QString topic)
{
  return "";
}

void QxrdJSEngine::processFunc(QString fn, QVector<double> nm)
{
}

void QxrdJSEngine::setFileNormalizationFunc(QString fn, QVector<double> nm)
{
}

QStringList QxrdJSEngine::matchFilesFunc(QString patt)
{
  return QStringList();
}

QxrdAcquisitionExtraInputsChannel* QxrdJSEngine::extraChannelFunc(int n)
{
  return NULL;
}

void QxrdJSEngine::mapUserFunctionFunc(QString func)
{
}

double QxrdJSEngine::timeStampFunc()
{
  return qQNaN();
}

QxrdDetectorSettings* QxrdJSEngine::detectorFunc(int n)
{
  QxrdDetectorSettingsWPtr res;

  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    res = acq->detector(n);
  }

  return res.data();
}

QxrdROICoordinates*   QxrdJSEngine::roiFunc(int n, int m)
{
  QxrdROICoordinatesWPtr res;

  QxrdAcquisitionPtr acq(acquisition());

  if (acq) {
    QxrdDetectorSettingsPtr det = acq->detector(n);

    if (det) {
      res = det->roi(m);
    }
  }

  return res.data();
}

QcepDataGroup*       QxrdJSEngine::newDataGroupFunc(QString name)
{
  return NULL;
}

QcepDataArray*       QxrdJSEngine::newDataArrayFunc(QString name, int d1, int d2, int d3, int d4)
{
  return NULL;
}

QcepDataColumn*      QxrdJSEngine::newDataColumnFunc(QString name, int npts)
{
  return NULL;
}

QcepDataColumnScan*  QxrdJSEngine::newDataColumnScanFunc(QString name, QStringList cols)
{
  return NULL;
}

QcepDoubleImageData* QxrdJSEngine::newDataImageFunc(QString name, int width, int height)
{
  return NULL;
}

QcepIntegratedData*  QxrdJSEngine::newIntegratedDataFunc(QString name, int size)
{
  return NULL;
}
