#include "qxrdprocessorexecution.h"
#include "qxrddetectorsettings.h"
#include "qxrddebug.h"
#include "qxrdprocessor.h"
#include "qxrdacqcommon.h"
#include "qxrdprocessorexecutionthread.h"
#include "qxrdexperiment.h"
#include "qxrdfilesaver.h"
#include <QThread>
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qcepallocator.h"

QxrdProcessorExecution::QxrdProcessorExecution(QString name)
  : inherited(name),
    m_Processing(this, "processing", 0, "Is processing?"),
    m_Processor(),
    m_Acquisition(),
    m_FileSaver()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecution::QxrdProcessorExecution(%p)\n", this);
  }
}

void QxrdProcessorExecution::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  QxrdDetectorSettingsPtr det = QxrdDetectorSettings::findDetectorSettings(parent);

  if (det) {
    m_Processor = det->processor();
  }

  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);

  QxrdExperimentPtr expt = QxrdExperiment::findExperiment(parent);

  if (expt) {
    m_FileSaver = expt->fileSaver();
  }

  QxrdProcessorPtr proc(m_Processor);

  if (proc == NULL) {
    printMessage("QxrdProcessorExecution::initialize m_Processor == NULL");
  }

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq == NULL) {
    printMessage("QxrdProcessorExecution::initialize m_Acquisition == NULL");
  }

  QxrdFileSaverPtr sav(m_FileSaver);

  if (sav == NULL) {
    printMessage("QxrdProcessorExecution::initialize m_FileSaver == NULL");
  }
}

QxrdProcessorExecution::~QxrdProcessorExecution()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdProcessorExecution::~QxrdProcessorExecution(%p)\n", this);
  }
}

void QxrdProcessorExecution::registerMetaTypes()
{
  qRegisterMetaType<QxrdProcessorExecution*>("QxrdProcessorExecution*");
  qRegisterMetaType<QxrdProcessorExecutionThread*>("QxrdProcessorExecutionThread*");
}

QcepImageDataBasePtr QxrdProcessorExecution::doDarkSubtraction(QcepImageDataBasePtr img)
{
  THREAD_CHECK;

  QcepImageDataBasePtr   res;

  QxrdProcessorPtr proc(m_Processor);

  if (proc) {
    QcepDoubleImageDataPtr dark = proc->dark();

    if (dark == NULL) {
      printMessage("No dark image available, skipping dark subtraction");
    } else if (img && dark) {
      if (img->get_ExposureTime() != dark->get_ExposureTime()) {
        printMessage("Exposure times of acquired data and dark image are different, skipping");
        return res;
      }

      if (img->get_Width() != dark->get_Width() ||
          img->get_Height() != dark->get_Height()) {
        printMessage("Dimensions of acquired data and dark image are different, skipping");
        return res;
      }

      if (img->get_CameraGain() != dark->get_CameraGain()) {
        printMessage("Gains of acquired data and dark image are different, skipping");
        return res;
      }

      int height = img->get_Height();
      int width  = img->get_Width();
      int nres = img -> get_SummedExposures();
      int ndrk = dark -> get_SummedExposures();
      int npixels = width*height;

      if (nres <= 0) nres = 1;


      double ratio = ((double) nres)/((double) ndrk);

      QcepDoubleImageDataPtr result = QcepAllocator::newDoubleImage(sharedFromThis(),
                                                                    "image",
                                                                    width, height,
                                                                    QcepAllocator::NullIfNotAvailable);

      if (result) {
        result->copyPropertiesFrom(img);

        double sumraw = 0, sumdark = 0;

        double *resptr = result->data();
        double *drkptr = dark->data();

        QcepUInt16ImageDataPtr i16 = qSharedPointerDynamicCast<QcepUInt16ImageData>(img);

        if (i16) {
          quint16 *imgptr = i16->data();

          for (int i=0; i<npixels; i++) {
            double valraw = imgptr[i];
            double valdark = drkptr[i];

            sumraw  += valraw;
            sumdark += valdark;

            resptr[i] = valraw - ratio*valdark;
          }
        } else {
          QcepUInt32ImageDataPtr i32 = qSharedPointerDynamicCast<QcepUInt32ImageData>(img);

          if (i32) {
            quint32 *imgptr = i32->data();

            for (int i=0; i<npixels; i++) {
              double valraw = imgptr[i];
              double valdark = drkptr[i];

              sumraw  += valraw;
              sumdark += valdark;

              resptr[i] = valraw - ratio*valdark;
            }
          } else {
            npixels = 0;

            for (int row=0; row<height; row++) {
              for (int col=0; col<width; col++) {
                double valraw  = img  -> getImageData(col, row);
                double valdark = dark -> getImageData(col, row);
                if (valraw == valraw && valdark == valdark) { // Check for NaNs
                  sumraw += valraw; sumdark += valdark;
                  npixels += 1;
                  double resval = valraw - ratio*valdark;

                  result->setImageData(col, row, resval);
                }
              }
            }
          }
        }

        if (result) {
          setSubtractedImageProperties(result);
        }
      }

      res = result;
    }
  }

  return res;
}

QcepImageDataBasePtr QxrdProcessorExecution::doBadPixels(QcepImageDataBasePtr img)
{
  THREAD_CHECK;

  printMessage("Bad Pixel Correction not yet implemented");

  return QcepImageDataBasePtr();
}

QcepImageDataBasePtr QxrdProcessorExecution::doGainCorrection(QcepImageDataBasePtr img)
{
  THREAD_CHECK;

  printMessage("Gain Correction not yet implemented");

  return QcepImageDataBasePtr();
}

QcepDoubleVector QxrdProcessorExecution::doCalculateROICounts(QcepImageDataBasePtr img)
{
  THREAD_CHECK;

  QcepDoubleVector res;

  //TODO: re-implement
//  if (img && m_ROICalculator) {
//    res = m_ROICalculator->values(img, mask());
//  }

  if (qcepDebug(DEBUG_ACQUIRE)) {
    QString s = "[";

    for (int i=0; i<res.count(); i++) {
      if (i == 0) {
        s.append(tr("%1").arg(res.value(i)));
      } else {
        s.append(tr(", %1").arg(res.value(i)));
      }
    }

    s.append("]");

    printMessage(tr("ROI Values = %1").arg(s));
  }

  return res;
}

void QxrdProcessorExecution::doSaveRawImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  THREAD_CHECK;

  QxrdFileSaverPtr  fsav(m_FileSaver);

  if (fsav && img) {
    QxrdFileSaver *f = fsav.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, [=]() { f -> saveImageData(img, ovf, QxrdFileSaver::NoOverwrite); } ));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, "saveImageData",
            Qt::QueuedConnection,
            Q_ARG(QcepImageDataBasePtr, img),
            Q_ARG(QcepMaskDataPtr, ovf),
            Q_ARG(int, QxrdFileSaver::NoOverwrite)));
#endif
  }
}

void QxrdProcessorExecution::doSaveDarkImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  THREAD_CHECK;

  QxrdFileSaverPtr  fsav(m_FileSaver);

  if (fsav && img) {
    QxrdFileSaver *f = fsav.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, [=]() { f -> saveImageData(img, ovf, QxrdFileSaver::NoOverwrite); } ));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, "saveImageData",
            Q_ARG(QcepImageDataBasePtr, img),
            Q_ARG(QcepMaskDataPtr, ovf),
            Q_ARG(int, QxrdFileSaver::NoOverwrite)));
#endif
  }
}

void QxrdProcessorExecution::doSaveSubtractedImage(QcepImageDataBasePtr img, QcepMaskDataPtr ovf)
{
  THREAD_CHECK;

  QxrdFileSaverPtr  fsav(m_FileSaver);

  if (fsav && img) {
    QxrdFileSaver *f = fsav.data();

#if QT_VERSION >= QT_VERSION_CHECK(5, 10, 0)
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, [=]() { f -> saveImageData(img, ovf, QxrdFileSaver::NoOverwrite); } ));
#else
    INVOKE_CHECK(
          QMetaObject::invokeMethod(
            f, "saveImageData",
            Q_ARG(QcepImageDataBasePtr, img),
            Q_ARG(QcepMaskDataPtr, ovf),
            Q_ARG(int, QxrdFileSaver::NoOverwrite)));
#endif
  }
}

void QxrdProcessorExecution::setCommonImageProperties(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (image && acq) {
    QxrdDetectorSettingsPtr det(acq->detector(image->get_DetectorNumber()));

    QDateTime now = QDateTime::currentDateTime();
    double msec = QcepImageDataBase::secondsSinceEpoch();

//    image -> set_Name             (image -> get_FileBase());
//    image -> set_ExposureTime     (acq   -> get_ExposureTime());
    image -> set_DateTime         (now);
    image -> set_TimeStamp        (msec);

    if (det) {
      image -> set_HBinning         (det   -> get_HBinning());
      image -> set_VBinning         (det   -> get_VBinning());
      image -> set_FileExtension    (det   -> get_Extension());
    } else {
      image -> set_HBinning         (1);
      image -> set_VBinning         (1);
      image -> set_FileExtension    (".tif");
    }


    image -> set_UserComment1     (acq   -> get_UserComment1());
    image -> set_UserComment2     (acq   -> get_UserComment2());
    image -> set_UserComment3     (acq   -> get_UserComment3());
    image -> set_UserComment4     (acq   -> get_UserComment4());
    image -> set_ObjectSaved      (false);
    image -> set_Triggered        (image -> get_ImageNumber() >= 0);
    image -> set_Normalization    (acq   -> get_Normalization());

    acq -> copyDynamicProperties(image.data());
  }
}

void QxrdProcessorExecution::setAcquiredImageProperties(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);

  if (image && proc) {
    setCommonImageProperties(image);

    if (image -> get_SummedExposures() <= 1) {
      image -> set_DataType(QcepImageDataBase::Raw16Data);
    } else {
      image -> set_DataType(QcepImageDataBase::Raw32Data);
    }

    image -> set_FileTypeName(".raw");
    image -> set_FileDirectory(proc -> rawOutputDirectory());
  }
}

void QxrdProcessorExecution::setDarkImageProperties(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);

  if (image && proc) {
    setCommonImageProperties(image);

    image -> set_DataType(QcepImageDataBase::DarkData);
    image -> set_FileTypeName(".dark");
    image -> set_FileDirectory(proc -> darkOutputDirectory());
  }
}

void QxrdProcessorExecution::setIdleImageProperties(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  if (image) {
    setCommonImageProperties(image);

    image -> set_DataType(QcepImageDataBase::IdleData);
    image -> set_FileTypeName(".idle");
    image -> set_FileDirectory(QString());
  }
}

void QxrdProcessorExecution::setSubtractedImageProperties(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);

  if (image && proc) {
    setCommonImageProperties(image);

    image -> set_DataType(QcepImageDataBase::SubtractedData);
    image -> set_FileTypeName("");
    image -> set_FileDirectory(proc -> subtractedOutputDirectory());
  }
}

void QxrdProcessorExecution::processAcquiredImage(QcepUInt32ImageDataPtr image,
                                                  QcepMaskDataPtr        overflow)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);
  bool rawWasSaved    = false;
  bool needsExtraSave = false;

  if (image && proc) {
    proc -> newData(image);
    proc -> newOverflow(overflow);

    QcepDoubleVector scalers;

    QcepImageDataBasePtr img = image;

    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processAcquiredImage(\"%1\",...)")
                   .arg(img->get_FileName()));
    }

    QTime tic;
    tic.start();

    setAcquiredImageProperties(img);

    if (proc -> get_SaveRawImages()) {
      doSaveRawImage(img, overflow);

      rawWasSaved = true;

      int saveTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Save took %1 msec").arg(saveTime));
      }
    }

    if (img && proc -> get_PerformDarkSubtraction()) {
      QcepImageDataBasePtr subt = doDarkSubtraction(img);

      if (subt) {
        img = subt;
        needsExtraSave = true;
      }

      int subTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Subtraction took %1 msec").arg(subTime));
      }
    }

    if (img && proc -> get_PerformBadPixels()) {
      QcepImageDataBasePtr badp = doBadPixels(img);

      if (badp) {
        img = badp;
        needsExtraSave = true;
      }

      int pxlTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Bd pixels took %1 msec").arg(pxlTime));
      }
    }

    if (img && proc -> get_PerformGainCorrection()) {
      QcepImageDataBasePtr gainC = doGainCorrection(img);

      if (gainC) {
        img = gainC;
        needsExtraSave = true;
      }

      int gainTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Gain correction took %1 msec").arg(gainTime));
      }
    }

    if (img) {
      proc->newData(img);
    }

//    if (ctrl && get_DetectorDisplayMode() == ImageDisplayMode) {
//      ctrl->displayNewData(img, overflow);

//      int displayTime = tic.restart();

//      if (qcepDebug(DEBUG_ACQUIRETIME)) {
//        printMessage(tr("Display took %1 msec").arg(displayTime));
//      }
//    }

    if (img && proc -> get_CalculateROICounts()) {
      const QcepDoubleVector s = doCalculateROICounts(img);

      scalers += s;

      proc -> set_RoiCounts(scalers);

      int roiTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("ROI calculation took %1 msec").arg(roiTime));
      }
    }

    if (img && proc -> get_SaveSubtracted() && (needsExtraSave || !rawWasSaved)) {
      doSaveSubtractedImage(img, overflow);

      proc -> set_DataPath(img -> get_FilePath());

      int saveTime = tic.restart();

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Save Subtracted took %1 msec").arg(saveTime));
      }
    }

    //    if (get_PerformIntegration()) {
    //      integ = doPerformIntegration(img);

    //      if (ctrl && get_DisplayIntegratedData()) {
    //        ctrl->displayIntegratedData(integ);
    //      }

    //      if (get_SaveIntegratedData()) {
    //        doSaveIntegratedData(integ);
    //      }

    //      if (get_SaveIntegratedDataSeparate()) {
    //        doSaveIntegratedDataSeparate(integ);
    //      }

    //      if (get_AccumulateIntegrated2D()) {
    //        doAccumulateIntegrated2D(integ);
    //      }
    //    }
  }
}

void QxrdProcessorExecution::processDarkImage(QcepDoubleImageDataPtr image,
                                             QcepMaskDataPtr overflow)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);

  if (image && proc) {
    if (qcepDebug(DEBUG_ACQUIRE)) {
      printMessage(tr("QxrdDetectorProcessor::processDarkImage(\"%1\",...)")
                   .arg(image->get_FileName()));
    }

    setDarkImageProperties(image);

    if (proc -> get_SaveDarkImages()) {
      doSaveDarkImage(image, overflow);

      proc -> set_DarkImagePath(image->get_FilePath());
    }

    proc->newDark(image);
  }
}

void QxrdProcessorExecution::processIdleImage(QcepImageDataBasePtr image)
{
  THREAD_CHECK;

  QxrdProcessorPtr proc(m_Processor);

  if (image && proc) {
    QxrdAcqCommonPtr acq(m_Acquisition);

    if (acq && acq->get_LiveViewAtIdle()) {
      QcepDoubleVector scalers;

      QcepImageDataBasePtr img = image;

      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("QxrdDetectorProcessor::processIdleImage(\"%1\")")
                     .arg(image->get_FileName()));
      }

      QTime tic;
      tic.start();

      setIdleImageProperties(img);

      //        QxrdDetectorControlWindowPtr ctrl(m_ControlWindow);

      if (img && proc -> get_PerformDarkSubtraction()) {
        img = doDarkSubtraction(img);

        int subTime = tic.restart();

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Subtraction took %1 msec").arg(subTime));
        }
      }

      if (img && proc -> get_PerformBadPixels()) {
        img = doBadPixels(img);

        int pxlTime = tic.restart();

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Bd pixels took %1 msec").arg(pxlTime));
        }
      }

      if (img && proc -> get_PerformGainCorrection()) {
        img = doGainCorrection(img);

        int gainTime = tic.restart();

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("Gain correction took %1 msec").arg(gainTime));
        }
      }

      //        if (ctrl && get_DetectorDisplayMode() == ImageDisplayMode) {
      //          ctrl->displayNewData(img, QcepMaskDataWPtr());

      //          int displayTime = tic.restart();

      //          if (qcepDebug(DEBUG_ACQUIRETIME)) {
      //            printMessage(tr("Display took %1 msec").arg(displayTime));
      //          }
      //        }

      if (img && proc -> get_CalculateROICounts()) {
        const QcepDoubleVector s = doCalculateROICounts(img);

        scalers += s;

        proc -> set_RoiCounts(scalers);

        int roiTime = tic.restart();

        if (qcepDebug(DEBUG_ACQUIRETIME)) {
          printMessage(tr("ROI calculation took %1 msec").arg(roiTime));
        }
      }
    }
  }
}
