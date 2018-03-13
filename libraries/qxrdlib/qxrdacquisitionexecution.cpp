#include "qxrdacquisitionexecution.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qxrdacqcommon.h"
#include <QTimer>
#include "qxrdacquisitionparameterpack.h"
#include "qxrddarkacquisitionparameterpack.h"
#include "qxrdprocessor-ptr.h"
#include "qxrdprocessor.h"
#include "qcepimagedata.h"
#include "qcepmaskdata.h"
#include "qxrddetectorsettings.h"
#include "qcepallocator.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdacquisitionextrainputs.h"
#include <QVector>

QxrdAcquisitionExecution::QxrdAcquisitionExecution(QString name)
  : inherited(name),
    m_Acquiring(this, "acquiring", 0, "Is acquiring?"),
    m_Acquisition()
{
#ifndef QT_NO_DEBUG
  printf("Constructing acquisition execution\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecution::QxrdAcquisitionExecution(%p)\n", this);
  }

  m_IdleTimer = QTimerPtr(new QTimer());

  connect(m_IdleTimer.data(), &QTimer::timeout,
          this,               &QxrdAcquisitionExecution::doAcquireIdle);
}

void QxrdAcquisitionExecution::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  m_Acquisition = QxrdAcqCommon::findAcquisition(parent);

  startIdling();
}

QxrdAcquisitionExecution::~QxrdAcquisitionExecution()
{
#ifndef QT_NO_DEBUG
  printf("Deleting acquisition execution\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdAcquisitionExecution::~QxrdAcquisitionExecution(%p)\n", this);
  }

  if (qcepDebug(DEBUG_APP)) {
    printMessage("QxrdAcquisitionExecution::~QxrdAcquisitionExecution");
  }
}

void QxrdAcquisitionExecution::setAcquisition(QxrdAcqCommonWPtr acq)
{
  m_Acquisition = acq;
}

void QxrdAcquisitionExecution::doAcquire()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionParameterPackPtr parmsp = acq -> acquisitionParameterPack();

    if (parmsp) {
      stopIdling();
      executeAcquisition(parmsp);
      startIdling();
    }
  }
}

void QxrdAcquisitionExecution::doAcquireOnce()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionParameterPackPtr parmsp = acq -> acquisitionParameterPack();

    if (parmsp) {
      parmsp->forceSingle();

      stopIdling();
      executeAcquisition(parmsp);
      startIdling();
    }
  }
}

void QxrdAcquisitionExecution::doAcquireDark()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdDarkAcquisitionParameterPackPtr parmsp = acq -> darkAcquisitionParameterPack();

    if (parmsp) {
      stopIdling();
      executeDarkAcquisition(parmsp);
      startIdling();
    }
  }
}

void QxrdAcquisitionExecution::doAcquireIdle()
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    int n = 0;

    for (int i=0; i<acq->detectorCount(); i++) {
      QxrdDetectorSettingsPtr det = acq->detector(i);

      if (det && det->isEnabled()) {
        n += det->availableImageCount();
        det->acquireFrameIfAvailable();
      }
    }

    if (n>0) {
      printMessage(tr("%1 images at idle").arg(n));
    }
  }
}

void QxrdAcquisitionExecution::startIdling()
{
  set_Acquiring(false);

  m_IdleTimer->start(5000);

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    if (acq->get_AcquisitionCancelsLiveView()) {
      acq->set_LiveViewAtIdle(false);
    }
  }
}

void QxrdAcquisitionExecution::stopIdling()
{
  set_Acquiring(true);

  m_IdleTimer->stop();
}

void QxrdAcquisitionExecution::executeAcquisition(QxrdAcquisitionParameterPackPtr parmsp)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && parmsp) {
    QTime acqTimer;
    acqTimer.start();

    stopIdling();

    QString fileBase = parmsp -> fileBase();
    int fileIndex    = acq    -> get_FileIndex();
    double exposure  = parmsp -> exposure();
    int nsummed      = parmsp -> nsummed();
    int preTrigger   = parmsp -> preTrigger();
    int postTrigger  = parmsp -> postTrigger();
    int nphases      = parmsp -> nphases();
    int skipBefore   = parmsp -> skipBefore();

    if (skipBefore <= 0) {
      if (acq -> get_LastAcquired() != 1) {
        skipBefore = 1;
      }
    }

    acq -> set_LastAcquired(1);

    int skipBetween = parmsp->skipBetween();
    int nPreTriggered = 0;

    if (preTrigger <= 0) {
      acq -> set_Triggered(true);
    }

    int nDet = 0;
    QVector<QxrdDetectorSettingsPtr> dets;
    QVector<QxrdProcessorPtr> procs;
    QVector<QVector<QVector<QcepUInt32ImageDataPtr> > >res;
    QVector<QVector<QVector<QcepMaskDataPtr> > >      ovf;

    for (int i=0; i<acq->detectorCount(); i++) {
      QxrdDetectorSettingsPtr det = acq->detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        res.resize(nDet);
        ovf.resize(nDet);

        res[nDet-1].resize(nphases);
        ovf[nDet-1].resize(nphases);

        for (int p=0; p<nphases; p++) {
          res[nDet-1][p].resize(preTrigger+1);
          ovf[nDet-1][p].resize(preTrigger+1);
        }
      }
    }


    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->prepareForAcquisition(parmsp);
    }

    if (acq->acquisitionExtraInputs()) {
      acq->acquisitionExtraInputs()->prepareForAcquisition(parmsp);
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    printMessage(tr("acquire(\"%1\", %2, %3, %4, %5, %6) // fileIndex = %7")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(postTrigger).arg(preTrigger).arg(nphases).arg(fileIndex));


    acq->set_CurrentPhase(0);
    acq->set_CurrentSummation(-1);
    acq->set_CurrentFile(fileIndex);

    for (int i=0; i<skipBefore; i++) {
      if (acq->cancelling()) goto cancel;
      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        dets[d]->acquireFrame();
      }
    }

    for (int i=0; i<postTrigger; i += (acq->get_Triggered() ? 1:0)) {
      if (acq->cancelling()) goto cancel;

      acq->set_CurrentFile(fileIndex);

      if (i != 0) {
        for (int k=0; k<skipBetween; k++) {
          if (acq->cancelling()) goto cancel;
          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Skipping %1 of %2").arg(k+1).arg(skipBetween));
          }

          for (int d=0; d<nDet; d++) {
            dets[d]->acquireFrame();
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("Frame after %1 msec").arg(acqTimer.restart()));
          }
        }
      }

      if (acq->cancelling()) goto cancel;

      for (int s=0; s<nsummed;) {
        acq->set_CurrentSummation(s);

        for (int p=0; p<nphases; p++) {
          acq->set_CurrentPhase(p);

          for (int d=0; d<nDet; d++) {
            QxrdDetectorSettingsPtr det = dets[d];

            det->beginFrame();
          }

          for (int d=0; d<nDet; d++) {
            QxrdDetectorSettingsPtr det = dets[d];
            int nCols = det->get_NCols();
            int nRows = det->get_NRows();

            if (res[d][p][0] == NULL) {
              QcepUInt32ImageDataPtr nres = QcepAllocator::newInt32Image(sharedFromThis(),
                                                                         tr("acc-%1-%2").arg(d).arg(p),
                                                                         nCols, nRows,
                                                                         QcepAllocator::AllocateFromReserve);

              res[d][p][0] = nres;

              if (nres == NULL) {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage("Dropped frame allocation...");
                }
                acq->indicateDroppedFrame(i);
              } else {
                QString fb, fn;
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage(tr("Newly allocated image number %1").arg(nres->get_ImageNumber()));
                }

                nres -> set_SummedExposures(0);

                acq->getFileBaseAndName(fileBase, det->get_Extension(),
                                   det->get_DetectorNumber(),
                                   fileIndex, p, nphases, fb, fn);

                nres -> set_FileBase(fb);
                nres -> set_FileName(fn);
              }
            }

            if (ovf[d][p][0] == NULL) {
              QcepMaskDataPtr novf = QcepAllocator::newMask(sharedFromThis(),
                                                            tr("mask-%1-%2").arg(d).arg(p),
                                                            nCols, nRows, 0,
                                                            QcepAllocator::AllocateFromReserve);
              ovf[d][p][0] = novf;

              if (novf == NULL) {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage("Dropped mask frame allocation...");
                }
                acq->indicateDroppedFrame(i);
              } else {
                if (qcepDebug(DEBUG_ACQUIRETIME)) {
                  printMessage(tr("Newly allocated mask number %1").arg(novf->get_ImageNumber()));
                }
              }

              if (novf) ovf[d][p][0] -> set_SummedExposures(0);
            }

            if (res[d][p][0]) {
              emit acq->acquiredFrame(res[d][p][0]->get_FileBase(),
                  p, nphases,
                  s, nsummed,
                  i, postTrigger);
            }

            QcepImageDataBasePtr   img = det -> acquireFrame();

            if (img && res[d][p][0] && ovf[d][p][0]) {
              QcepUInt16ImageDataPtr i16 = qSharedPointerDynamicCast<QcepUInt16ImageData>(img);
              QcepUInt32ImageDataPtr i32 = qSharedPointerDynamicCast<QcepUInt32ImageData>(img);

              if (i32) {
                accumulateAcquiredImage(i32, res[d][p][0], ovf[d][p][0]);
              } else if (i16) {
                accumulateAcquiredImage(i16, res[d][p][0], ovf[d][p][0]);
              }
            } else if (!acq->cancelling()){
              acq->indicateDroppedFrame(i);
            }

            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage(tr("accumulateAcquiredImage %1 msec idx:%2 post:%3 sum: %4 ph:%5")
                           .arg(acqTimer.restart())
                           .arg(fileIndex)
                           .arg(i)
                           .arg(s)
                           .arg(p)
                           );
            }

            if (acq->cancelling()) goto saveCancel;
          }
        }

        if (acq->get_RetryDropped()) {
          int minSum = nsummed+10;

          for (int p=0; p<nphases; p++) {
            for (int d=0; d<nDet; d++) {
              if (res[d][p][0]) {
                int ns = res[d][p][0]->get_SummedExposures();

                if (ns < minSum) {
                  minSum = ns;
                }
              } else {
                minSum = 0;
              }
            }
          }

          if (qcepDebug(DEBUG_ACQUIRETIME)) {
            printMessage(tr("i = %1, Minsum = %2, s = %3, nsummed = %4").arg(i).arg(minSum).arg(s).arg(nsummed));
          }

          if (minSum == nsummed+10) {
            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage("No acquired images allocated");
            }
            //          s = s+1;
          } else {
            s = minSum;
          }
        } else {
          s = s+1;
        }
      }

saveCancel:
      if (acq->get_Triggered()) {
        int nPre = qMin(preTrigger, nPreTriggered);

        for (int ii=nPre; ii >= 1; ii--) {
          for (int p=0; p<nphases; p++) {
            for (int d=0; d<nDet; d++) {

//              procs[d] -> processAcquiredImage(res[d][p][ii], ovf[d][p][ii], fileIndex, p, nphases, false);
              INVOKE_CHECK(
                    QMetaObject::invokeMethod(procs[d].data(), "processAcquiredImage",
                                              Q_ARG(QcepUInt32ImageDataPtr, res[d][p][ii]),
                                              Q_ARG(QcepMaskDataPtr, ovf[d][p][ii]),
                                              Q_ARG(int, fileIndex),
                                              Q_ARG(int, p),
                                              Q_ARG(int, nphases),
                                              Q_ARG(bool, false)));

              if (qcepDebug(DEBUG_ACQUIRETIME)) {
                printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                             .arg(__LINE__)
                             .arg(acqTimer.restart())
                             .arg(fileIndex)
                             .arg(ii)
                             .arg(p)
                             );
              }

              res[d][p].pop_back();
              ovf[d][p].pop_back();
            }
          }

          fileIndex++;
          acq->set_FileIndex(fileIndex);
        }

        nPreTriggered = 0;

        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
//            procs[d] -> processAcquiredImage(res[d][p][0], ovf[d][p][0], fileIndex, p, nphases, true);

            INVOKE_CHECK(
                  QMetaObject::invokeMethod(procs[d].data(), "processAcquiredImage",
                                            Q_ARG(QcepUInt32ImageDataPtr, res[d][p][0]),
                                            Q_ARG(QcepMaskDataPtr, ovf[d][p][0]),
                                            Q_ARG(int, fileIndex),
                                            Q_ARG(int, p),
                                            Q_ARG(int, nphases),
                                            Q_ARG(bool, true)));

            if (qcepDebug(DEBUG_ACQUIRETIME)) {
              printMessage(tr("processAcquiredImage(line %1) %2 msec idx:%3 pre:%4 ph:%5")
                           .arg(__LINE__)
                           .arg(acqTimer.restart())
                           .arg(fileIndex)
                           .arg(i)
                           .arg(p)
                           );
            }

            res[d][p][0] = QcepUInt32ImageDataPtr();
            ovf[d][p][0] = QcepMaskDataPtr();
          }
        }
        fileIndex++;
        acq->set_FileIndex(fileIndex);
      } else {
        nPreTriggered++;
        for (int p=0; p<nphases; p++) {
          for (int d=0; d<nDet; d++) {
            res[d][p].push_front(QcepUInt32ImageDataPtr());
            ovf[d][p].push_front(QcepMaskDataPtr());
            res[d][p].pop_back();
            ovf[d][p].pop_back();
          }
        }
      }
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->finishedAcquisition();
    }

    for (int d=0; d<nDet; d++) {
      dets[d]->endAcquisition();
    }

    QVector<double> scalers;

    scalers.append(acq->get_FileIndex()-1);

    if (acq->acquisitionExtraInputs()) {
      scalers += acq->acquisitionExtraInputs()->evaluateChannels();
    }

    for (int d=0; d<nDet; d++) {
      scalers += dets[d]->scalerCounts();
    }

    acq->set_ScalerValues(scalers);
  }

  acq->unlock();

  emit acq->acquireComplete();
}

void QxrdAcquisitionExecution::executeDarkAcquisition(QxrdDarkAcquisitionParameterPackPtr parmsp)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && parmsp) {
    QString fileBase = parmsp->fileBase();
    int fileIndex = acq->get_FileIndex();
    double exposure = parmsp->exposure();
    int nsummed = parmsp->nsummed();
    int skipBefore = parmsp->skipBefore();

    if (skipBefore <= 0) {
      if (acq->get_LastAcquired() != -1) {
        skipBefore = 1;
      }
    }

    acq->set_LastAcquired(-1);

    printMessage(tr("acquireDark(\"%1\", %2, %3) // fileIndex = %4")
                 .arg(fileBase).arg(exposure).arg(nsummed).arg(fileIndex));

    int nDet = 0;
    QVector<QxrdDetectorSettingsPtr> dets;
    QVector<QxrdProcessorPtr> procs;
    QVector<QcepDoubleImageDataPtr> res;
    QVector<QcepMaskDataPtr> overflow;

    acq->set_CurrentPhase(-1);
    acq->set_CurrentSummation(0);
    acq->set_CurrentFile(fileIndex);

    for (int i=0; i<acq->detectorCount(); i++) {
      QxrdDetectorSettingsPtr det = acq->detector(i);

      if (det && det->isEnabled()) {
        nDet++;
        dets.append(det);
        procs.append(det->processor());

        QcepDoubleImageDataPtr img = QcepAllocator::newDoubleImage(sharedFromThis(),
                                                                   tr("dark-%1").arg(i),
                                                                   det->get_NCols(), det->get_NRows(),
                                                                   QcepAllocator::AllocateFromReserve);

        res.append(img);

        QcepMaskDataPtr msk = QcepAllocator::newMask(sharedFromThis(),
                                                     tr("mask-%1").arg(i),
                                                     det->get_NCols(), det->get_NRows(),0,
                                                     QcepAllocator::AllocateFromReserve);

        overflow.append(msk);
      }
    }

    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->prepareForDarkAcquisition(parmsp);
    }

    QString fb, fn;

    for (int d=0; d<nDet; d++) {
      if (res[d] == NULL || overflow[d] == NULL) {
        criticalMessage("Insufficient memory for acquisition operation");
        goto cancel;
      }
    }

    for (int d=0; d<nDet; d++) {
      dets[d] -> beginAcquisition(exposure);
    }

    for (int d=0; d<nDet; d++) {
      QxrdDetectorSettingsPtr det = dets[d];

      acq->getFileBaseAndName(fileBase, det->get_Extension(), det->get_DetectorNumber(), fileIndex, -1, 1, fb, fn);

      res[d] -> set_FileBase(fb);
      res[d] -> set_FileName(fn);
    }

    for (int i=0; i<skipBefore; i++) {
      if (acq->cancelling()) goto cancel;

      acq->set_CurrentSummation(-1);

      for (int d=0; d<nDet; d++) {
        dets[i] -> beginFrame();
      }

      if (qcepDebug(DEBUG_ACQUIRETIME)) {
        printMessage(tr("Skipping %1 of %2").arg(i+1).arg(skipBefore));
      }

      for (int d=0; d<nDet; d++) {
        dets[i] -> acquireFrame();
      }
    }

    for (int i=0; i<nsummed;) {
      if (acq->cancelling()) goto cancel;

      acq->set_CurrentSummation(i);

      for (int d=0; d<nDet; d++) {
        dets[d] -> beginFrame();
      }

      for (int d=0; d<nDet; d++) {
        emit acq->acquiredFrame(res[d]->get_FileBase(), 0, 1, i, nsummed, 0, 1);

        QcepImageDataBasePtr   img = dets[d] -> acquireFrame();

        if (img) {
          QcepUInt16ImageDataPtr i16 = qSharedPointerDynamicCast<QcepUInt16ImageData>(img);
          QcepUInt32ImageDataPtr i32 = qSharedPointerDynamicCast<QcepUInt32ImageData>(img);

          if (i16) {
            accumulateAcquiredImage(i16, res[d], overflow[d]);
          } else {
            accumulateAcquiredImage(i32, res[d], overflow[d]);
          }
        } else {
          if (!acq->cancelling()){
            acq->indicateDroppedFrame(0);
          } else {
            goto cancel;
          }
        }
      }

      if (acq->get_RetryDropped()) {
        int minSum = nsummed+10;

        for (int d=0; d<nDet; d++) {
          if (res[d]) {
            int ns = res[d]->get_SummedExposures();

            if (ns < minSum) {
              minSum = ns;
            }
          } else {
            minSum = 0;
          }
        }

        i = minSum;
      } else{
        i = i+1;
      }
    }

    //saveCancel:

    for (int d=0; d<nDet; d++) {
//      procs[d]->processDarkImage(res[d], overflow[d], fileIndex);

      INVOKE_CHECK(
            QMetaObject::invokeMethod(procs[d].data(), "processDarkImage",
                                      Q_ARG(QcepDoubleImageDataPtr, res[d]),
                                      Q_ARG(QcepMaskDataPtr, overflow[d]),
                                      Q_ARG(int, fileIndex)));
    }

    statusMessage(tr("Acquisition complete"));
    printMessage(tr("Acquisition complete"));

cancel:
    if (acq->synchronizedAcquisition()) {
      acq->synchronizedAcquisition()->finishedAcquisition();
    }
  }

  acq->unlock();

  emit acq->acquireComplete();
}

void QxrdAcquisitionExecution::accumulateAcquiredImage(QcepUInt16ImageDataPtr image,
                                              QcepUInt32ImageDataPtr accum,
                                              QcepMaskDataPtr        overflow)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    int ovflwlvl = acq  ->get_OverflowLevel();
    quint16* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int srcsum  = image->get_SummedExposures();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(srcsum);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(srcsum);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

void QxrdAcquisitionExecution::accumulateAcquiredImage(QcepUInt32ImageDataPtr image,
                                              QcepUInt32ImageDataPtr accum,
                                              QcepMaskDataPtr        overflow)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    quint32 ovflwlvl = acq->get_OverflowLevel();
    quint32* src = image->data();
    quint32* dst = accum->data();
    short int* ovf = overflow->data();
    int srcsum  = image->get_SummedExposures();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(srcsum);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(srcsum);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

void QxrdAcquisitionExecution::accumulateAcquiredImage(QcepUInt16ImageDataPtr image,
                                              QcepDoubleImageDataPtr accum,
                                              QcepMaskDataPtr        overflow)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    int ovflwlvl = acq->get_OverflowLevel();
    quint16* src = image->data();
    double* dst = accum->data();
    short int* ovf = overflow->data();
    int srcsum  = image->get_SummedExposures();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(srcsum);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint16 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(srcsum);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

void QxrdAcquisitionExecution::accumulateAcquiredImage(QcepUInt32ImageDataPtr image,
                                              QcepDoubleImageDataPtr accum,
                                              QcepMaskDataPtr        overflow)
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq && image && accum && overflow) {
    long nPixels = image->get_Height()*image->get_Width();
    int ovflwlvl = acq->get_OverflowLevel();
    quint32* src = image->data();
    double* dst = accum->data();
    short int* ovf = overflow->data();
    int srcsum  = image->get_SummedExposures();
    int nsummed = accum->get_SummedExposures();

    if (nsummed == 0) {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 saved").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ = 1;
        } else {
          *ovf++ = 0;
        }

        *dst++ = val;
      }

      accum->set_Normalization(image->get_Normalization());
      accum->set_ExtraInputs(image->get_ExtraInputs());
      accum->set_SummedExposures(srcsum);
      accum->set_ImageSequenceNumber(image->get_ImageSequenceNumber());
    } else {
      if (qcepDebug(DEBUG_ACQUIRE)) {
        printMessage(tr("Frame %1 summed").arg(nsummed));
      }

      for (long i=0; i<nPixels; i++) {
        quint32 val = *src++;

        if (val>ovflwlvl) {
          *ovf++ += 1;
        } else {
          ovf++;
        }

        *dst++ += val;
      }

      accum->prop_Normalization()   -> incValue(image->get_Normalization());
      accum->prop_ExtraInputs()     -> incValue(image->get_ExtraInputs());
      accum->prop_SummedExposures() -> incValue(srcsum);
      accum->prop_ImageSequenceNumber() -> incValue(image->get_ImageSequenceNumber());
    }
  }
}

