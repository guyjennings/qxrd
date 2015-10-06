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

}

int QxrdAcquisitionScalerModel::rowCount(const QModelIndex &parent) const
{
  int nRows = 1;

  QxrdAcquisitionPtr acq(m_Acquisition);

  if (acq) {
    QxrdAcquisitionExtraInputsPtr xtra = acq->acquisitionExtraInputs();
    QVector<QxrdAcquisitionExtraInputsChannelPtr> xchans;

    if (xtra) {
      xchans = xtra->channels();
    }

    nRows += xchans.count();

    int nDet = acq->get_DetectorCount();

    for (int det=0; det<nDet; det++) {
      QxrdDetectorPtr d = acq->detector(det);

      if (d && d->isEnabled()) {
        nRows += 1;
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
          return "NIDAQ Inputs";
        }
      } else {
        row -= 1;

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
                  return tr("Detector %1 : %2").arg(det).arg(d->get_DetectorName());
                }
              } else {
                row -= 1;

                int nROI = d->roiCount();

                if (row < nROI) {
                  if (col == NumCol) {
                    return scalerchan+row;
                  } else if (col == ValueCol) {
                    QxrdROICoordinatesPtr rr = d->roi(row);

                    if (rr) {
                      return rr->get_Value();
                    }
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
