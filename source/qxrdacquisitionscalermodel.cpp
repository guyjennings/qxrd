#include "qxrdacquisitionscalermodel.h"
#include "qxrdacquisition.h"
#include "qxrddetector.h"
#include "qxrdacquisitionextrainputs.h"
#include "qxrddetectorprocessor.h"
#include "qxrdroicalculator.h"
#include "qxrdroicoordinateslist.h"
#include "qxrdroicoordinates.h"

QxrdAcquisitionScalerModel::QxrdAcquisitionScalerModel(QxrdAcquisitionWPtr acq)
  : m_Acquisition(acq)
{
  QxrdAcquisitionPtr acqr(m_Acquisition);

  if (acqr) {
    connect(acqr.data(),               &QxrdAcquisition::detectorStateChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);

    connect(acqr.data(),               &QxrdAcquisition::extraInputsChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);

    connect(acqr->prop_ScalerValues(), &QcepDoubleVectorProperty::valueChanged,
            this,                      &QxrdAcquisitionScalerModel::forceFullUpdate);
  }
}

int QxrdAcquisitionScalerModel::rowCount(const QModelIndex &parent) const
{
  int nRows = 2;

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionExtraInputsPtr xtra = acq->acquisitionExtraInputs();
    QVector<QxrdAcquisitionExtraInputsChannelPtr> xchans;

    if (xtra) {
      xchans = xtra->channels();
    }

    int nXtra = xchans.count();

    if (nXtra) {
      nRows += nXtra+1;
    }

    int nDet = acq->get_DetectorCount();

    for (int det=0; det<nDet; det++) {
      QxrdDetectorPtr d = acq->detector(det);

      if (d && d->isEnabled()) {
        nRows += 1;
//        nRows += QxrdDetector::ExtraScalers;
        nRows += d->roiCount();
      }
    }
  }

  return nRows;
}

int QxrdAcquisitionScalerModel::columnCount(const QModelIndex &parent) const
{
  return ColCount;
}

QVariant QxrdAcquisitionScalerModel::data(const QModelIndex &index, int role) const
{
  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    int scalerchan = 0;

    QxrdAcquisitionExtraInputsPtr xtra = acq->acquisitionExtraInputs();
    QVector<QxrdAcquisitionExtraInputsChannelPtr> xchans;

    if (xtra) {
      xchans = xtra->channels();
    }

    int row = index.row();
    int col = index.column();
    int nXtra = xchans.count();
    int nDet = acq->get_DetectorCount();

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
              QxrdAcquisitionExtraInputsChannelPtr chan = xchans.value(row);

              if (chan) {
                return chan->evaluateChannel();
              }
            } else if (col == DescriptionCol) {
              return tr("NIDAQ Channel %1").arg(row);
            }
          } else {
            row -= nXtra;
            scalerchan += nXtra;

            for (int det=0; det<nDet; det++) {
              QxrdDetectorPtr d = acq->detector(det);

              if (d && d->isEnabled()) {
                if (row == 0) {
                  if (col == DescriptionCol) {
                    return tr("====== Detector %1 : %2 ======").arg(det).arg(d->get_DetectorName());
                  }
                } else {
                  row -= 1;

                  int nROI = d->roiCount();

                  if (row < nROI) {
                    if (col == NumCol) {
                      return scalerchan+row;
                    } else if (col == ValueCol) {
                      double val = d->scalerCounts(row);

                      return val;
                    } else if (col == DescriptionCol) {
                      return tr("Detector %1 : ROI %2").arg(det).arg(row);
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
