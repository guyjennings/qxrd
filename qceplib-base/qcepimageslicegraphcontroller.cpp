#include "qcepimageslicegraphcontroller.h"
#include <stdio.h>
#include "qcepdataobjectgraphwindow.h"
#include "qcepimageslicegraphcontrols.h"
#include "qcepimagedata.h"
#include "qcepimagedata-ptr.h"
#include "qwt_plot_piecewise_curve.h"

QcepImageSliceGraphController::QcepImageSliceGraphController(QcepDataObjectGraphWindow *window, int mode, QcepDataObjectWPtr object)
  : QcepDataObjectGraphController(window, mode, object)
{

}

QcepImageSliceGraphController::~QcepImageSliceGraphController()
{
  delete(m_Widget);
}

void QcepImageSliceGraphController::activate()
{
  m_Widget = new QcepImageSliceGraphControls(m_Window, m_Mode, m_Object);

  m_Window->m_Splitter->addWidget(m_Widget);

  QcepDataObjectGraphController::activate();
}

void QcepImageSliceGraphController::updateDisplay()
{
  m_Window -> clearPlot();
  int curveNumber = 0;

  QcepImageDataBase* data = qobject_cast<QcepImageDataBase*>(m_Object.data());

  if (data) {
    int mode = m_Window -> currentGraphMode();

    int nRows = data->get_Height();
    int nCols = data->get_Width();

    if (mode == QcepDataObjectGraphWindow::HorizontalSlice) {
      int start   = m_Window->get_SliceHStart();
      int summed  = m_Window->get_SliceHSummed();
      int skipped = m_Window->get_SliceHSkipped();
      int repeats = m_Window->get_SliceHRepeats();

      if (summed < 1)  summed = 1;
      if (skipped < 0) skipped = 0;
      if (repeats < 1) repeats = 1;

      for (int rpt = 0; rpt<repeats; rpt++) {
        QVector<double> x,y;

        for (int col=0; col<nCols; col++) {
          double s=0;
          int n=0;

          for (int sum=0; sum<summed; sum++) {
            int row=start+rpt*(summed+skipped)+sum;

            if (row < nRows) {
              double v = data->getImageData(col, row);

              if (v==v) {
                s += v;
                n += 1;
              }
            }
          }

          x.append(col);
          y.append(s/n);
        }

        if (x.count() > 0) {
          int st = start+rpt*(summed+skipped);
          int en = st+summed-1;

          if (en >= nRows) {
            en = nRows-1;
          }

          QwtPlotCurve *curve = NULL;

          if (st == en) {
            curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("row %1").arg(st));
          } else {
            curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("rows %1 to %2").arg(st).arg(en));
          }

          curve->setSamples(x, y);

          m_Window->m_ImagePlot->setPlotCurveStyle(curveNumber++, curve);

          m_Window->appendCurve(curve);
        }
      }
    } else if (mode == QcepDataObjectGraphWindow::VerticalSlice) {
      int start   = m_Window->get_SliceVStart();
      int summed  = m_Window->get_SliceVSummed();
      int skipped = m_Window->get_SliceVSkipped();
      int repeats = m_Window->get_SliceVRepeats();

      if (summed < 1)  summed = 1;
      if (skipped < 0) skipped = 0;
      if (repeats < 1) repeats = 1;

      for (int rpt = 0; rpt<repeats; rpt++) {
        QVector<double> x,y;

        for (int row=0; row<nRows; row++) {
          double s=0;
          int n=0;

          for (int sum=0; sum<summed; sum++) {
            int col=start+rpt*(summed+skipped)+sum;

            if (col < nCols) {
              double v = data->getImageData(col, row);

              if (v==v) {
                s += v;
                n += 1;
              }
            }
          }

          x.append(row);
          y.append(s/n);
        }

        if (x.count() > 0) {
          int st = start+rpt*(summed+skipped);
          int en = st+summed-1;

          if (en >= nCols) {
            en = nCols-1;
          }

          QwtPlotPiecewiseCurve *curve = NULL;

          if (st == en) {
            curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("col %1").arg(st));
          } else {
            curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("cols %1 to %2").arg(st).arg(en));
          }

          curve->setSamples(x, y);

          m_Window->m_ImagePlot->setPlotCurveStyle(curveNumber++, curve);

          m_Window->appendCurve(curve);
        }
      }
    }
  }

  m_Window -> m_ImagePlot -> replot();

  QcepDataObjectGraphController::updateDisplay();
}
