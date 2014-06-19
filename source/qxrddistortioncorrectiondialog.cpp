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

  connect(m_UpdateGridButton, SIGNAL(clicked()), this, SLOT(updateGridTable()));
  connect(m_FitGridButton,    SIGNAL(clicked()), m_DistortionCorrection.data(), SLOT(fitCalibrationGrid()));
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

void QxrdDistortionCorrectionDialog::updateGridTable()
{
  m_DistortionGridTable->setRowCount(0);

  QcepIntVector iVals = m_DistortionCorrection->get_IVals();
  QcepIntVector jVals = m_DistortionCorrection->get_JVals();
  QcepDoubleVector xVals = m_DistortionCorrection->get_XVals();
  QcepDoubleVector yVals = m_DistortionCorrection->get_YVals();
  QcepDoubleVector fxVals = m_DistortionCorrection->get_FXVals();
  QcepDoubleVector fyVals = m_DistortionCorrection->get_FYVals();
  QcepDoubleVector dxVals = m_DistortionCorrection->get_DXVals();
  QcepDoubleVector dyVals = m_DistortionCorrection->get_DYVals();

  int nRows = qMax(iVals.count(), jVals.count());
  nRows = qMax(nRows, xVals.count());
  nRows = qMax(nRows, yVals.count());
  nRows = qMax(nRows, fxVals.count());
  nRows = qMax(nRows, fyVals.count());
  nRows = qMax(nRows, dxVals.count());
  nRows = qMax(nRows, dyVals.count());

  m_DistortionGridTable->setRowCount(nRows);

  for (int r = 0; r<nRows; r++) {
    if (r < iVals.count()) {
      m_DistortionGridTable->setItem(r,0, new QTableWidgetItem(tr("%1").arg(iVals[r])));
    }

    if (r < jVals.count()) {
      m_DistortionGridTable->setItem(r,1, new QTableWidgetItem(tr("%1").arg(jVals[r])));
    }

    if (r < xVals.count()) {
      m_DistortionGridTable->setItem(r,2, new QTableWidgetItem(tr("%1").arg(xVals[r])));
    }

    if (r < yVals.count()) {
      m_DistortionGridTable->setItem(r,3, new QTableWidgetItem(tr("%1").arg(yVals[r])));
    }

    if (r < fxVals.count()) {
      m_DistortionGridTable->setItem(r,4, new QTableWidgetItem(tr("%1").arg(fxVals[r])));
    }

    if (r < fyVals.count()) {
      m_DistortionGridTable->setItem(r,5, new QTableWidgetItem(tr("%1").arg(fyVals[r])));
    }

    if (r < dxVals.count()) {
      m_DistortionGridTable->setItem(r,6, new QTableWidgetItem(tr("%1").arg(dxVals[r])));
    }

    if (r < dyVals.count()) {
      m_DistortionGridTable->setItem(r,7, new QTableWidgetItem(tr("%1").arg(dyVals[r])));
    }
  }

  m_DistortionGridTable->resizeColumnsToContents();
  m_DistortionGridTable->resizeRowsToContents();
}
