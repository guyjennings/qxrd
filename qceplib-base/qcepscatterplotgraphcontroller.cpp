#include "qcepscatterplotgraphcontroller.h"
#include <stdio.h>
//#include "qcepexperiment-ptr.h"
//#include "qcepexperiment.h"
#include "qcepdataobjectgraphwindow.h"
#include "qcepscatterplotgraphcontrols.h"
#include "qcepscatterplotgraphmodel.h"
#include "qcepintegrateddata-ptr.h"
#include "qcepintegrateddata.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn-ptr.h"
#include "qcepdatacolumn.h"
#include "qwt_plot_piecewise_curve.h"

QcepScatterPlotGraphController::QcepScatterPlotGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepScatterPlotGraphController::~QcepScatterPlotGraphController()
{
  delete(m_Widget);
}

void QcepScatterPlotGraphController::activate()
{
  QcepScatterPlotGraphControls *ctrl = new QcepScatterPlotGraphControls(m_Window, m_Mode, m_Object);

  if (ctrl) {
    m_Widget = ctrl;

    m_Window->m_Splitter->addWidget(m_Widget);

    m_Model = ctrl->model();

    QcepScatterPlotGraphModelPtr m(m_Model);

    if (m) {
      connect(m.data(), &QAbstractItemModel::dataChanged,
              this,     &QcepScatterPlotGraphController::onPlotDataChanged);
    }
  }

  QcepDataObjectGraphController::activate();
}

void QcepScatterPlotGraphController::onPlotDataChanged(
    const QModelIndex & /*topLeft*/, const QModelIndex & /*bottomRight*/)
{
//  QcepExperimentPtr exp(m_Window->experiment());

//  if (exp) {
//    exp->printMessage(tr("QcepScatterPlotGraphController::onPlotDataChanged([%1,%2],[%3,%4])")
//                      .arg(topLeft.column()).arg(topLeft.row())
//                      .arg(bottomRight.column()).arg(bottomRight.row()));
//  }

  updateDisplay();
}

void QcepScatterPlotGraphController::updateDisplay()
{
  m_Window -> clearPlot();
  m_Window -> m_ImagePlot -> enableAxis(QwtPlot::yRight, false);

  int curveNumber = 0;

  QcepScatterPlotGraphModelPtr model(m_Model);
  QcepDataObjectPtr            object(m_Object);
  QcepIntegratedDataPtr        integ = qSharedPointerDynamicCast<QcepIntegratedData>(m_Object);
  QcepDataColumnScanPtr        scan  = qSharedPointerDynamicCast<QcepDataColumnScan>(m_Object);

  if (model) {
    int xcol = -1;
    int nr = model->rowCount(QModelIndex());

    for (int i=0; i<nr; i++) {
      if (model->getX(i)) {
        xcol = i;
      }
    }

    if (integ) {
      const double *cols[2];
      QString       labels[2];

      cols[0] = integ->x();
      cols[1] = integ->y();
      int nr  = integ->size();
      labels[0] = integ->get_XUnitsLabel();
      labels[1] = "Y";

      if (xcol >= 0) {
        const double *xc = cols[xcol];

        m_Window->m_ImagePlot -> setAxisTitle(QwtPlot::xBottom, labels[xcol]);

        for (int i=0; i<2; i++) {
          if (model->getY(i)) {
            appendCurve(labels[i], 0, curveNumber++, xc, cols[i], nr);
          }

          if (model->getY2(i)) {
            appendCurve(labels[i], 1, curveNumber++, xc, cols[i], nr);
          }
        }
      }
    } else if (scan) {
      if (xcol >= 0) {
        QcepDataColumnPtr xCol = scan->column(xcol);

        if (xCol) {
          m_Window->m_ImagePlot -> setAxisTitle(QwtPlot::xBottom, xCol->get_Name());

          for (int i=0; i<nr; i++) {
            if (model->getY(i)) {
              QcepDataColumnPtr yCol = scan->column(i);

              if (yCol) {
                appendCurve(yCol->get_Name(), 0, curveNumber++, xCol, yCol);
              }
            }

            if (model->getY2(i)) {
              QcepDataColumnPtr y2Col = scan->column(i);

              if (y2Col) {
                appendCurve(y2Col->get_Name(), 1, curveNumber++, xCol, y2Col);
              }
            }
          }
        }
      }
    }
  }

  m_Window -> m_ImagePlot -> replot();

  QcepDataObjectGraphController::updateDisplay();
}

void QcepScatterPlotGraphController::appendCurve(
    QString name, int axis, int curveNumber, const double *x, const double *y, int npts)
{
  QwtPlotCurve *curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, name);

  curve->setSamples(x, y, npts);

  m_Window->m_ImagePlot->setPlotCurveStyle(curveNumber, curve);

  if (axis == 0) {
    curve->setYAxis(QwtPlot::yLeft);
  } else if (axis == 1) {
    curve->setYAxis(QwtPlot::yRight);

    m_Window->m_ImagePlot->enableAxis(QwtPlot::yRight, true);
  }

  m_Window->appendCurve(curve);
}

void QcepScatterPlotGraphController::appendCurve(
    QString name, int axis, int curveNumber, QcepDataColumnPtr x, QcepDataColumnPtr y)
{
  if (x && y) {
    const double *xp = x->data(), *yp = y->data();
    int np = qMin(x->count(), y->count());

    appendCurve(name, axis, curveNumber, xp, yp, np);
  }
}
