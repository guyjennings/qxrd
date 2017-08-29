#include "qxrddistortioncorrectiondialog.h"
#include "ui_qxrddistortioncorrectiondialog.h"
#include "qxrddebug.h"
#include "qxrddistortioncorrection.h"
#include <stdio.h>
#include "qxrdwindow.h"
#include "qxrdplotvectorfieldcurve.h"
#include "qxrddistortioncorrectiondialogsettings.h"

QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(QxrdDistortionCorrectionPtr dis, QxrdWindowPtr parent) :
  QDockWidget(parent.data()),
  m_Window(parent),
  m_DistortionCorrection(dis)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdDistortionCorrectionDialog::QxrdDistortionCorrectionDialog(%p)\n", this);
  }

  setupUi(this);

  if (dis) {
    dis->prop_DistortionImagePath()->linkTo(m_DistortionCalibrationPath);
    dis->prop_P0()->linkTo(m_DistortionP0X, m_DistortionP0Y);
    dis->prop_P1()->linkTo(m_DistortionP1X, m_DistortionP1Y);
    dis->prop_P2()->linkTo(m_DistortionP2X, m_DistortionP2Y);
    dis->prop_N1()->linkTo(m_DistortionN1);
    dis->prop_N2()->linkTo(m_DistortionN2);
    dis->prop_F0()->linkTo(m_DistortionF0X, m_DistortionF0Y);
    dis->prop_F1()->linkTo(m_DistortionF1X, m_DistortionF1Y);
    dis->prop_F2()->linkTo(m_DistortionF2X, m_DistortionF2Y);

    dis->prop_WMin()->linkTo(m_DistortionWMin);
    dis->prop_WMax()->linkTo(m_DistortionWMax);
    dis->prop_WNom()->linkTo(m_DistortionWNom);
    dis->prop_RatMin()->linkTo(m_DistortionRatMin);
    dis->prop_HgtMin()->linkTo(m_DistortionHgtMin);
    dis->prop_DistMax()->linkTo(m_DistortionDistMaxX, m_DistortionDistMaxY);

    connect(m_FitGridButton,     &QAbstractButton::clicked, dis.data(), &QxrdDistortionCorrection::fitCalibrationGrid);
  }

  connect(m_UpdateGridButton,  &QAbstractButton::clicked, this, &QxrdDistortionCorrectionDialog::updateGridTable);
  connect(m_UpdateGraphButton, &QAbstractButton::clicked, this, &QxrdDistortionCorrectionDialog::updateDistortionGraph);
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
  QxrdDistortionCorrectionPtr dis(m_DistortionCorrection);

  if (dis) {
    m_DistortionGridTable->setRowCount(0);

    QcepIntVector iVals = dis->get_IVals();
    QcepIntVector jVals = dis->get_JVals();
    QcepDoubleVector xVals = dis->get_XVals();
    QcepDoubleVector yVals = dis->get_YVals();
    QcepDoubleVector fxVals = dis->get_FXVals();
    QcepDoubleVector fyVals = dis->get_FYVals();
    QcepDoubleVector dxVals = dis->get_DXVals();
    QcepDoubleVector dyVals = dis->get_DYVals();

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
}

void QxrdDistortionCorrectionDialog::updateDistortionGraph()
{
  QxrdDistortionCorrectionPtr dis(m_DistortionCorrection);
  QxrdWindowPtr               win(m_Window);

  if (dis && win) {
    QxrdDistortionCorrectionPlot *img = win->m_DistortionCorrectionPlot;

    if (img) {
      img->detachItems(QwtPlotItem::Rtti_PlotCurve);
      img->detachItems(QwtPlotItem::Rtti_PlotMarker);

      QxrdPlotVectorFieldCurve *curve = new QxrdPlotVectorFieldCurve();

      QxrdPlotVectorFieldData *data = new QxrdPlotVectorFieldData();

      QcepDoubleVector xVals = dis->get_XVals();
      QcepDoubleVector yVals = dis->get_YVals();
      QcepDoubleVector dxVals = dis->get_DXVals();
      QcepDoubleVector dyVals = dis->get_DYVals();

      int n = xVals.count();

      for (int i=0; i<n; i++) {
        data->append(QxrdPoint4D(xVals.value(i), yVals.value(i), dxVals.value(i), dyVals.value(i)));
      }

      curve -> setSamples(data);

      curve -> attach(img);

      img->replot();
    }
  }
}
