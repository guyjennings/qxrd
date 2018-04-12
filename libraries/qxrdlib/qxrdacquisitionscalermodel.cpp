#include "qxrdacquisitionscalermodel.h"
#include "qxrdacqcommon.h"
#include "qxrddetectorsettings.h"
#include "qxrdsynchronizedacquisition.h"
#include "qxrdsynchronizedinputchannel.h"
#include "qxrdprocessor.h"
#include "qceproicalculator.h"
#include "qceproi.h"

QxrdAcquisitionScalerModel::QxrdAcquisitionScalerModel(QxrdAcqCommonWPtr acq)
  : m_Acquisition(acq)
{
  QxrdAcqCommonPtr acqr(m_Acquisition);

  if (acqr) {
    connect(acqr.data(),               &QxrdAcqCommon::detectorStateChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);

    connect(acqr.data(),               &QxrdAcqCommon::extraInputsChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);

    connect(acqr->prop_ScalerValues(), &QcepDoubleVectorProperty::valueChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);

    QxrdSynchronizedAcquisitionPtr sync(acqr->synchronizedAcquisition());

    if (sync) {
      connect(sync.data(),   &QxrdSynchronizedAcquisition::inputCountChanged,
              this,          &QxrdAcquisitionScalerModel::forceFullUpdate);
    }
  }
}

int QxrdAcquisitionScalerModel::rowCount(const QModelIndex & /*parent*/) const
{
  int nRows = 2;

  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    QxrdSynchronizedAcquisitionPtr sync(acq->synchronizedAcquisition());
    QVector<QxrdSynchronizedInputChannelPtr> xchans;

    if (sync) {
      int nChans = sync->inputCount();

      for (int i=0; i<nChans; i++) {
        xchans.append(sync->input(i));
      }
    }

    int nXtra = xchans.count();

    if (nXtra) {
      nRows += nXtra+1;
    }

    int nDet = acq->detectorCount();

    for (int det=0; det<nDet; det++) {
      QxrdDetectorSettingsPtr d = acq->detector(det);

      if (d && d->isEnabled()) {
        nRows += 1;
//        nRows += QxrdDetector::ExtraScalers;
        nRows += d->roiCount()*QcepROI::OutputCount;
      }
    }
  }

  return nRows;
}

int QxrdAcquisitionScalerModel::columnCount(const QModelIndex & /*parent*/) const
{
  return ColCount;
}

QVariant QxrdAcquisitionScalerModel::data(const QModelIndex &index, int role) const
{
  QxrdAcqCommonPtr acq(m_Acquisition);

  if (acq) {
    int scalerchan = 0;

    QxrdSynchronizedAcquisitionPtr sync = acq->synchronizedAcquisition();
    QVector<QxrdSynchronizedInputChannelPtr> xchans;

    if (sync) {
      int nChan = sync->inputCount();

      for (int i=0; i<nChan; i++) {
        xchans.append(sync->input(i));
      }
    }

    int row = index.row();
    int col = index.column();
    int nXtra = xchans.count();
    int nDet = acq->detectorCount();

    if (role == Qt::DisplayRole) {
      if (row == 0) {
        if (col == DescriptionCol) {
          return "====== Acquisition ======";
        }
      } else if (row == 1) {
        if (col == NumCol) {
          return 0;
        } else if (col == ValueCol) {
          return acq->get_FileIndex()-1;
        } else if (col == DescriptionCol) {
          return "File Index";
        }
      } else {
        row -= 2;
        scalerchan += 1;

        if (nXtra && row == 0) {
          if (col == DescriptionCol) {
            return "====== NIDAQ Inputs ======";
          }
        } else {
          if (nXtra) {
            row -= 1;
          }

          if (row < nXtra) {
            if (col == NumCol) {
              return scalerchan+row;
            } else if (col == ValueCol) {
              QxrdSynchronizedInputChannelPtr chan = xchans.value(row);

              if (chan) {
                return chan->evaluateInput();
              }
            } else if (col == DescriptionCol) {
              return tr("NIDAQ Channel %1").arg(row);
            }
          } else {
            row -= nXtra;
            scalerchan += nXtra;

            for (int det=0; det<nDet; det++) {
              QxrdDetectorSettingsPtr d = acq->detector(det);

              if (d && d->isEnabled()) {
                if (row == 0) {
                  if (col == DescriptionCol) {
                    return tr("====== Detector %1 : %2 ======").arg(det).arg(d->get_DetectorName());
                  }
                } else {
                  row -= 1;

                  int nROI = d->roiCount()*QcepROI::OutputCount;

                  if (row < nROI) {
                    if (col == NumCol) {
                      return scalerchan+row;
                    } else if (col == ValueCol) {
                      double val = d->scalerCounts(row);

                      return val;
                    } else if (col == DescriptionCol) {
                      int roiNum = row / QcepROI::OutputCount;
                      int optNum = row % QcepROI::OutputCount;

                      return tr("Detector %1 : ROI %2 : %3")
                          .arg(det).arg(roiNum).arg(QcepROI::outputName(optNum));
                    }
                  } else {
                    row -= nROI;
                    scalerchan += nROI;
                  }
                }
              }
            }
          }
        }
      }
    }
  }

  return QVariant();
}

QVariant QxrdAcquisitionScalerModel::headerData(int section, Qt::Orientation orientation, int role) const
{
  if (orientation == Qt::Horizontal) {
    if (role == Qt::DisplayRole) {
      if (section == NumCol) {
        return "Scaler#";
      } else if (section == ValueCol) {
        return "Value";
      } else if (section == DescriptionCol) {
        return "Description";
      }
    }
  }

  return QVariant();
}

void QxrdAcquisitionScalerModel::forceFullUpdate()
{
  beginResetModel();

  endResetModel();
}
