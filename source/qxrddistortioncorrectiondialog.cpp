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
//  m_DistortionCorrection->prop_DistortionP0()->linkTo();
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
