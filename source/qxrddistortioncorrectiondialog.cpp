#include "qxrddistortioncorrectiondialog.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddebug.h"
#include "qxrddistortioncorrection.h"

QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QWidget *parent) :
  QDockWidget(parent),
  m_DistortionCorrection(dis)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(%p)\n", this);
  }

  setupUi(this);

  m_DistortionCorrection->prop_DistortionImagePath()->linkTo(m_DistortionCalibrationPath);
  m_DistortionCorrection->prop_DistortionP0()->linkTo(m_DistortionP0X, m_DistortionP0Y);
  m_DistortionCorrection->prop_DistortionP1()->linkTo(m_DistortionP1X, m_DistortionP1Y);
  m_DistortionCorrection->prop_DistortionP2()->linkTo(m_DistortionP2X, m_DistortionP2Y);
}

void QxrdDistortionCorrectionDialog::init(QxrdDistortionCorrectionDialogSettingsWPtr settings)
{
}

QxrdDistortionCorrectionDialog::~QxrdDistortionCorrectionDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDistortionCorrectionDialog::~QxrdDistortionCorrectionDialog(%p)\n", this);
  }
}
