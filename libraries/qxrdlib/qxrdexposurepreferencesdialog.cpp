#include "qxrdexposurepreferencesdialog.h"
#include "ui_qxrdexposurepreferencesdialog.h"
#include "qxrdacqcommon.h"
#include <QGridLayout>
#include <QCheckBox>
#include <QSpinBox>
#include "qxrddetectorsettings.h"

QxrdExposurePreferencesDialog::QxrdExposurePreferencesDialog(QxrdAcqCommonWPtr acq, QWidget *parent) :
  QDialog(parent),
  m_Acquisition(acq)
{
  setupUi(this);

  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp->prop_ExposureTime() -> copyTo(m_ExposureTime);

    int nDet = acqp->detectorCount();

    for (int i=0; i<nDet; i++) {
      QxrdDetectorSettingsPtr det(acqp->detector(i));

      QLabel    *iLabel      = new QLabel(tr("%1").arg(i));
      QCheckBox *isEnabled   = new QCheckBox();
      QSpinBox  *relExposure = new QSpinBox();

      m_CheckBoxes.append(isEnabled);
      m_SpinBoxes.append(relExposure);

      if (det) {
        isEnabled->setChecked(det->get_Enabled());
        relExposure->setValue(det->get_ExposureFactor());
        relExposure->setMinimum(1);
        relExposure->setMaximum(100);
      }

      m_ExposureGrid -> addWidget(iLabel, 2+i, 0, 1, 1, Qt::AlignCenter);
      m_ExposureGrid -> addWidget(isEnabled, 2+i, 1, 1, 1, Qt::AlignCenter);
      m_ExposureGrid -> addWidget(relExposure, 2+i, 2, 1, 1, Qt::AlignCenter);
    }
  }
}

QxrdExposurePreferencesDialog::~QxrdExposurePreferencesDialog()
{
}

void QxrdExposurePreferencesDialog::accept()
{
  QxrdAcqCommonPtr acqp(m_Acquisition);

  if (acqp) {
    acqp->prop_ExposureTime() -> copyFrom(m_ExposureTime);

    int nDet = acqp->detectorCount();

    for (int i=0; i<nDet; i++) {
      QxrdDetectorSettingsPtr det(acqp->detector(i));

      if (det) {
        det->prop_Enabled()        -> copyFrom(m_CheckBoxes.value(i));
        det->prop_ExposureFactor() -> copyFrom(m_SpinBoxes.value(i));
      }
    }
  }

  inherited::accept();
}
