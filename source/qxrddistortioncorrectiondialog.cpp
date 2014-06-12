#include "qxrddistortioncorrectiondialog.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddebug.h"

QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QWidget *parent) :
  QDockWidget(parent),
  m_DistortionCorrection(dis)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(%p)\n", this);
  }

  setupUi(this);
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
