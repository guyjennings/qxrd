#include "qxrddistortioncorrectiondialog.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddebug.h"
#include "qxrddistortioncorrection.h"
#include <stdio.h>

QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QWidget *parent) :
  QDockWidget(parent),
  m_DistortionCorrection(dis)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(%p)\n", this);
  }

  setupUi(this);

  m_DistortionCorrection->prop_DistortionImagePath()->linkTo(m_DistortionCalibrationPath);
  m_DistortionCorrection->prop_P0()->linkTo(m_DistortionP0X, m_DistortionP0Y);
  m_DistortionCorrection->prop_P1()->linkTo(m_DistortionP1X, m_DistortionP1Y);
  m_DistortionCorrection->prop_P2()->linkTo(m_DistortionP2X, m_DistortionP2Y);
  m_DistortionCorrection->prop_N1()->linkTo(m_DistortionN1);
  m_DistortionCorrection->prop_N2()->linkTo(m_DistortionN2);
  m_DistortionCorrection->prop_F0()->linkTo(m_DistortionF0X, m_DistortionF0Y);
  m_DistortionCorrection->prop_F1()->linkTo(m_DistortionF1X, m_DistortionF1Y);
  m_DistortionCorrection->prop_F2()->linkTo(m_DistortionF2X, m_DistortionF2Y);

  m_DistortionCorrection->prop_WMin()->linkTo(m_DistortionWMin);
  m_DistortionCorrection->prop_WMax()->linkTo(m_DistortionWMax);
  m_DistortionCorrection->prop_WNom()->linkTo(m_DistortionWNom);
  m_DistortionCorrection->prop_RatMin()->linkTo(m_DistortionRatMin);
  m_DistortionCorrection->prop_HgtMin()->linkTo(m_DistortionHgtMin);
  m_DistortionCorrection->prop_DistMax()->linkTo(m_DistortionDistMaxX, m_DistortionDistMaxY);
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

