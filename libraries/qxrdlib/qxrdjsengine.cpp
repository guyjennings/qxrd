#include "qxrdjsengine.h"
#include "qxrdexperiment.h"
#include "qxrdacqcommon.h"
#include "qxrddebug.h"
#include "qcepallocator.h"
#include "qxrdapplication.h"
#include "qxrdprocessor.h"
#include "qceproi.h"
#include "qxrddetectorsettings.h"
#include "qcepcalibrantlibrary.h"
#include "qcepcalibrant.h"
#include "qcepdatasetmodel.h"
#include "qxrdgeneratetestimage.h"
#include "qcepdataexportparameters.h"
#include "qcepdataimportparameters.h"
#include "qxrdsynchronizedacquisition.h"
#include "qcepcenterfinder.h"
#include "qxrdintegrator.h"
#include "qxrdpolartransform.h"
#include "qxrdpolarnormalization.h"
#include "qxrdsynchronizerplugin.h"
#include "qxrdapplication-ptr.h"
#include "qxrdapplication.h"

QxrdJSEngine::QxrdJSEngine(QString name) :
  m_Application(),
  m_Experiment()
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

void QxrdJSEngine::initialize(QcepObjectWPtr parent)
{
  m_Application = QxrdAppCommon::findApplication(parent);
  m_Experiment  = QxrdExperiment::findExperiment(parent);

  installExtensions(QJSEngine::AllExtensions);

  qRegisterMetaType< QVector<qreal> >("QVector<qreal>");

  qmlRegisterType<QcepROI>();
  qmlRegisterType<QxrdDetectorSettings>();
//  qmlRegisterType<QxrdPowderPoint>();
  qmlRegisterType<QcepCalibrant>();
//  qmlRegisterType<QxrdCalibrantPtr>();
//  qmlRegisterType<QPointF>();
//  qmlRegisterType<QRectF>();
//  qmlRegisterType<QPolygonF>();
//  qmlRegisterType<QVector<double> >();

//  qmlRegisterType<QcepDoubleImageData>();
//  qmlRegisterType<QcepDoubleImageDataPtr>();
//  qmlRegisterType<QcepDoubleImageDataWPtr>();

  qmlRegisterType<QxrdDetectorSettings>();
//  qmlRegisterType<QxrdDetectorSettingsWPtr>();
//  qmlRegisterType<QxrdDetectorSettingsPtr>();

  setGlobalProperty("global", globalObject());
  setGlobalProperty("scripting", newQObject(this));

  setGlobalProperty("acquire", newQObject(this).property("acquireFunc"));
  setGlobalProperty("acquireDark", newQObject(this).property("acquireDarkFunc"));
  setGlobalProperty("acquireOnce", newQObject(this).property("acquireOnceFunc"));
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
  setGlobalProperty("calibrant", newQObject(this).property("calibrantFunc"));

  setGlobalProperty("newDataGroup", newQObject(this).property("newDataGroupFunc"));
  setGlobalProperty("newDataArray", newQObject(this).property("newDataArrayFunc"));
  setGlobalProperty("newDataColumn", newQObject(this).property("newDataColumnFunc"));
  setGlobalProperty("newDataColumnScan", newQObject(this).property("newDataColumnScanFunc"));
  setGlobalProperty("newDataImage", newQObject(this).property("newDataImageFunc"));
  setGlobalProperty("newIntegratedData", newQObject(this).property("newIntegratedDataFunc"));

  QxrdAppCommonPtr app(m_Application);

  if (app) {
//    QCEP_DOC_OBJECT("application", "The QXRD Application Object");
    setGlobalProperty("application", newQObject(app.data()));

    QxrdApplicationPtr appp(
          qSharedPointerDynamicCast<QxrdApplication>(app));

    if (appp) {
      QxrdSynchronizerPluginPtr sync = appp->synchronizerPlugin();

      if (sync) {
        setGlobalProperty("syncPlugin", newQObject(sync.data()));
      }
    }
  }

  if (g_Allocator) {
//    QCEP_DOC_OBJECT("allocator", "The QXRD Memory Allocator");
    setGlobalProperty("allocator", newQObject(g_Allocator));
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    setGlobalProperty("experiment", newQObject(expt.data()));

    QxrdAcqCommonPtr acq(acquisition());

    if (acq) {
      setGlobalProperty("acquisition", newQObject(acq.data()));

      QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());

      if (sync) {
        //      QCEP_DOC_OBJECT("synchronization", "Synchronized Acquisition");
        setGlobalProperty("synchronization", newQObject(sync.data()));
      }
    }

//    QcepSimpleServerPtr ssrv(expt->simpleServer());

//    if (ssrv) {
////      QCEP_DOC_OBJECT("simpleServer", "Remote Control Text Based Socket Server");
//      setGlobalProperty("simpleServer", newQObject(ssrv.data()));
//    }

//    QcepSpecServerPtr srv(expt->specServer());

//    if (srv) {
////      QCEP_DOC_OBJECT("specServer", "Remote Control Server for use with Spec");
//      setGlobalProperty("specServer", newQObject(srv.data()));
//    }

    //TODO: is this the appropriate one...
    QxrdProcessorPtr dp(expt->processor());

    if (dp) {
//      QCEP_DOC_OBJECT("processor", "Control Data Processing Options");
      setGlobalProperty("processor",       newQObject(dp.data()));

//      QCEP_DOC_OBJECT("centering", "Beam Center and Detector Alignment Options");

      QcepCenterFinderPtr cf(dp->centerFinder());

      if (cf) {
        setGlobalProperty("centering",       newQObject(cf.data()));
      }

//      QCEP_DOC_OBJECT("integrator", "Image Circular Integration Options");

      QxrdIntegratorPtr integ(dp->integrator());

      if (integ) {
        setGlobalProperty("integrator",      newQObject(integ.data()));
      }

//      QCEP_DOC_OBJECT("polarTransform", "Polar Transform Options");

      QxrdPolarTransformPtr pt(dp->polarTransform());

      if (pt) {
        setGlobalProperty("polarTransform",      newQObject(pt.data()));
      }

//      QCEP_DOC_OBJECT("polarNormalization", "Polar Normalization Options");

      QxrdPolarNormalizationPtr pn(dp->polarNormalization());

      if (pn) {
        setGlobalProperty("polarNormalization",      newQObject(pn.data()));
      }

      QxrdGenerateTestImagePtr gti(dp->generateTestImage());

      if (gti) {
//        QCEP_DOC_OBJECT("testImage", "Object for generating test images");
        setGlobalProperty("testImage",       newQObject(gti.data()));
      }
    }

    QcepCalibrantLibraryPtr cals(expt->calibrantLibrary());

    if (cals) {
//      QCEP_DOC_OBJECT("calibrants", "Calibrant Library");
      setGlobalProperty("calibrants", newQObject(cals.data()));
    }

    QcepDatasetModelPtr ds = expt->dataset();

    if (ds) {
      setGlobalProperty("dataset", newQObject(ds.data()));
    }

    QcepDataExportParametersPtr exp = expt->dataExportParameters();

    if (exp) {
      setGlobalProperty("exportParameters", newQObject(exp.data()));
    }

    QcepDataImportParametersPtr imp = expt->dataImportParameters();

    if (imp) {
      setGlobalProperty("importParameters", newQObject(imp.data()));
    }
  }
}

void QxrdJSEngine::setWindow(QxrdWindowWPtr /*win*/)
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
  } else if (result.isCallable()) {
    return result.toString();
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

  } else if (result.isObject() || result.isQObject()) {
    QString s = "{";

    if (depth == 0) s += "\n";

    QJSValueIterator it(result);

    while(it.hasNext()) {
      it.next();

      if (depth == 0) s += "  ";

      s += it.name()+":";
      s += convertHelper(it.value(), depth+1);

      if (it.hasNext()) {
        if (depth == 0) {
          s += ",\n";
        } else {
          s += ", ";
        }
      }
    }

    if (depth == 0) {
      s += "\n}";
    } else {
      s += "}";
    }

    return s;

  } else {
    return result.toString();
  }
}

QxrdAcqCommonWPtr QxrdJSEngine::acquisition()
{
  QxrdAcqCommonWPtr res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    res = expt->acquisition();
  }

  return res;
}

QxrdProcessorWPtr QxrdJSEngine::processor()
{
  QxrdProcessorWPtr res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    res = expt->processor();
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
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  QxrdAcqCommonPtr acq(acquisition());

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

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquire call took %1 msec").arg(tick.elapsed()));
    }
  }
}

void QxrdJSEngine::acquireDarkFunc(QString fp,
                                   double  tim,
                                   double  sum)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  QxrdAcqCommonPtr acq(acquisition());

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

    acq->acquireDark();

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireDark call took %1 msec").arg(tick.elapsed()));
    }
  }
}

void QxrdJSEngine::acquireOnceFunc(QString fp,
                                   double  tim,
                                   double  sum)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  QxrdAcqCommonPtr acq(acquisition());

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

    acq->acquireOnce();

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireOnce call took %1 msec").arg(tick.elapsed()));
    }
  }
}

int  QxrdJSEngine::statusFunc(double tim)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  int res = -1;

  QxrdAcqCommonPtr acq(acquisition());
  QxrdProcessorPtr proc(processor());

  if (acq && proc) {
    res = acq->acquisitionStatus(tim);

    if (res == 1) {
      res = proc->status(tim);
    }

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("status call took %1 msec").arg(tick.elapsed()));
    }
  }

  return res;
}

int  QxrdJSEngine::acquireStatusFunc(double tim)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  int res = -1;

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    res = acq->acquisitionStatus(tim);

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireStatus call took %1 msec").arg(tick.elapsed()));
    }
  }

  return res;
}

void QxrdJSEngine::acquireCancelFunc()
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    acq->cancel();

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireCancel call took %1 msec").arg(tick.elapsed()));
    }
  }
}

int  QxrdJSEngine::processStatusFunc(double tim)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  int res = -1;

  QxrdProcessorPtr proc(processor());

  if (proc) {
    res = proc->status(tim);

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      proc->printMessage(tr("processStatus call took %1 msec").arg(tick.elapsed()));
    }
  }

  return res;
}

QJSValue QxrdJSEngine::acquireScalersFunc()
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  QJSValue res = newArray();

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    QcepDoubleVector v = acq->get_ScalerValues();

    for (int i=0; i<v.length(); i++) {
      res.setProperty(i, v.value(i));
    }

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireScalers() call took %1 msec").arg(tick.elapsed()));
    }
  }

  return res;
}

double QxrdJSEngine::acquireScalersFunc(int i)
{
  QTime tick;

  if (qcepDebug(DEBUG_ACQUIRETIME)) {
    tick.start();
  }

  double res = 0;

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    res = acq->scalerValue(i);

    if (qcepDebug(DEBUG_ACQUIRETIME)) {
      acq->printMessage(tr("acquireScalers(%1) call took %2 msec").arg(i).arg(tick.elapsed()));
    }
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

double QxrdJSEngine::exposureTimeFunc(double /*t*/)
{
  return qQNaN();
}

int QxrdJSEngine::summedExposuresFunc()
{
  return -1;
}

int QxrdJSEngine::summedExposuresFunc(int /*n*/)
{
  return -1;
}

int QxrdJSEngine::skippedExposuresFunc()
{
  return -1;
}

int QxrdJSEngine::skippedExposuresFunc(int /*n*/)
{
  return -1;
}

int QxrdJSEngine::darkSummedExposuresFuncs()
{
  return -1;
}

int QxrdJSEngine::darkSummedExposuresFuncs(int /*n*/)
{
  return -1;
}

int QxrdJSEngine::phasesInGroupFunc()
{
  return -1;
}

int QxrdJSEngine::phasesInGroupFunc(int /*n*/)
{
  return -1;
}

int QxrdJSEngine::preTriggerFilesFunc()
{
  return -1;
}

int QxrdJSEngine::preTriggerFilesFunc(int /*n*/)
{
  return -1;
}

int QxrdJSEngine::postTriggerFilesFunc()
{
  return -1;
}

int QxrdJSEngine::postTriggerFilesFunc(int /*n*/)
{
  return -1;
}

QString QxrdJSEngine::filePatternFunc()
{
  return "";
}

QString QxrdJSEngine::filePatternFunc(QString /*patt*/)
{
  return "";
}

QString QxrdJSEngine::outputDirectoryFunc()
{
  return "";
}

QString QxrdJSEngine::outputDirectoryFunc(QString /*d*/)
{
  return "";
}

int QxrdJSEngine::fileIndexFunc()
{
  return -1;
}

int QxrdJSEngine::fileIndexFunc(int /*n*/)
{
  return -1;
}

void QxrdJSEngine::fopenFunc(QString /*fn*/)
{
}

void QxrdJSEngine::fdeleteFunc(QString /*p*/)
{
}

void QxrdJSEngine::fprintFunc(QVariant /*v1*/,
                              QVariant /*v2*/,
                              QVariant /*v3*/,
                              QVariant /*v4*/,
                              QVariant /*v5*/,
                              QVariant /*v6*/,
                              QVariant /*v7*/)
{
}

void QxrdJSEngine::fcloseFunc()
{
}

QcepImageDataBase* QxrdJSEngine::dataFunc()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    return proc->data().data();
  } else {
    return NULL;
  }
}

QcepDoubleImageData* QxrdJSEngine::darkFunc()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    return proc->dark().data();
  } else {
    return NULL;
  }
}

QcepMaskData*        QxrdJSEngine::maskFunc()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    return proc->mask().data();
  } else {
    return NULL;
  }
}

QcepMaskData*        QxrdJSEngine::overflowFunc()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    return proc->overflow().data();
  } else {
    return NULL;
  }
}

QcepDoubleImageData* QxrdJSEngine::liveDataFunc()
{
  QxrdProcessorPtr proc(processor());

  if (proc) {
    return proc->liveData().data();
  } else {
    return NULL;
  }
}

QString QxrdJSEngine::helpFunc(QString /*topic*/)
{
  return "";
}

void QxrdJSEngine::processFunc(QString /*fn*/, QVector<double> /*nm*/)
{
}

void QxrdJSEngine::setFileNormalizationFunc(QString /*fn*/, QVector<double> /*nm*/)
{
}

QStringList QxrdJSEngine::matchFilesFunc(QString /*patt*/)
{
  return QStringList();
}

//QxrdAcquisitionExtraInputsChannel* QxrdJSEngine::extraChannelFunc(int /*n*/)
//{
//  return NULL;
//}

void QxrdJSEngine::mapUserFunctionFunc(QString /*func*/)
{
}

double QxrdJSEngine::timeStampFunc()
{
  return qQNaN();
}

QJSValue QxrdJSEngine::detectorFunc(int n)
{
  QJSValue res;

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    QxrdDetectorSettingsPtr det = acq->detector(n);

    if (det) {
      res = newQObject(det.data());

      setObjectOwnership(det.data(), CppOwnership);
    }
  }

  return res;
}

QJSValue QxrdJSEngine::roiFunc(int n, int m)
{
  QJSValue res;

  QxrdAcqCommonPtr acq(acquisition());

  if (acq) {
    QxrdDetectorSettingsPtr det = acq->detector(n);

    if (det) {
      QcepROIPtr roic = det->roi(m);

      if (roic) {
        res = newQObject(roic.data());

        setObjectOwnership(roic.data(), CppOwnership);
      }
    }
  }

  return res;
}

QJSValue QxrdJSEngine::calibrantFunc(int n)
{
  QJSValue res;

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepCalibrantLibraryPtr lib(expt->calibrantLibrary());

    if (lib) {
      QcepCalibrantPtr cal(lib->calibrant(n));

      if (cal) {
        res = newQObject(cal.data());

        setObjectOwnership(cal.data(), CppOwnership);
      }
    }
  }

  return res;
}

QcepDataGroup*       QxrdJSEngine::newDataGroupFunc(QString /*name*/)
{
  return NULL;
}

QcepDataArray*       QxrdJSEngine::newDataArrayFunc(QString /*name*/, int /*d1*/, int /*d2*/, int /*d3*/, int /*d4*/)
{
  return NULL;
}

QcepDataColumn*      QxrdJSEngine::newDataColumnFunc(QString /*name*/, int /*npts*/)
{
  return NULL;
}

QcepDataColumnScan*  QxrdJSEngine::newDataColumnScanFunc(QString /*name*/, QStringList /*cols*/)
{
  return NULL;
}

QcepDoubleImageData* QxrdJSEngine::newDataImageFunc(QString /*name*/, int /*width*/, int /*height*/)
{
  return NULL;
}

QcepIntegratedData*  QxrdJSEngine::newIntegratedDataFunc(QString /*name*/, int /*size*/)
{
  return NULL;
}
