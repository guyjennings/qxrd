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

    double hStart = data->get_HStart();
    double hStep  = data->get_HStep();

    double vStart = data->get_VStart();
    double vStep  = data->get_VStep();

    if (mode == QcepDataObjectGraphWindow::HorizontalSlice) {
      m_Window -> m_ImagePlot -> setAxisTitle(QwtPlot::xBottom, tr("%1 (%2)")
                                              .arg(data->get_HLabel()).arg(data->get_HUnits()));
      m_Window -> m_ImagePlot -> setAxisTitle(QwtPlot::yLeft, "Intensity");

      int start   = m_Window->get_SliceHStart();
      int summed  = m_Window->get_SliceHSummed();
      int skipped = m_Window->get_SliceHSkipped();
      int repeats = m_Window->get_SliceHRepeats();

      if (summed < 1)  summed = 1;
      if (skipped < 0) skipped = 0;
      if (repeats < 1) repeats = 1;

      for (int rpt = 0; rpt<repeats; rpt++) {
        int st = start+rpt*(summed+skipped);

        if (st < nRows) {
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

            x.append(hStart+col*hStep);
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
              curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("%1 = %2")
                                                .arg(data->get_VLabel())
                                                .arg(vStart+st*vStep)
                                                .arg(data->get_VUnits()));
            } else {
              curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("%1 = %2 to %3 %4")
                                                .arg(data->get_VLabel())
                                                .arg(vStart+st*vStep).arg(vStart+en*vStep)
                                                .arg(data->get_VUnits()));
            }

            curve->setSamples(x, y);

            m_Window->m_ImagePlot->setPlotCurveStyle(curveNumber++, curve);

            m_Window->appendCurve(curve);
          }
        }
      }
    } else if (mode == QcepDataObjectGraphWindow::VerticalSlice) {
      m_Window -> m_ImagePlot -> setAxisTitle(QwtPlot::xBottom, tr("%1 (%2)")
                                              .arg(data->get_VLabel()).arg(data->get_VUnits()));
      m_Window -> m_ImagePlot -> setAxisTitle(QwtPlot::yLeft, "Intensity");

      int start   = m_Window->get_SliceVStart();
      int summed  = m_Window->get_SliceVSummed();
      int skipped = m_Window->get_SliceVSkipped();
      int repeats = m_Window->get_SliceVRepeats();

      if (summed < 1)  summed = 1;
      if (skipped < 0) skipped = 0;
      if (repeats < 1) repeats = 1;

      for (int rpt = 0; rpt<repeats; rpt++) {
        int st = start+rpt*(summed+skipped);

        if (st < nCols) {
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

            x.append(vStart+row*vStep);
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
              curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("%1 = %2 %3")
                                                .arg(data->get_HLabel())
                                                .arg(hStart+st*hStep)
                                                .arg(data->get_HUnits()));
            } else {
              curve = new QwtPlotPiecewiseCurve(m_Window->m_ImagePlot, tr("%1 = %2 to %3 %4")
                                                .arg(data->get_HLabel())
                                                .arg(hStart+st*hStep).arg(hStart+en*hStep)
                                                .arg(data->get_HUnits()));
            }

            curve->setSamples(x, y);

            m_Window->m_ImagePlot->setPlotCurveStyle(curveNumber++, curve);

            m_Window->appendCurve(curve);
          }
        }
      }
    }
  }

  m_Window -> m_ImagePlot -> replot();

  QcepDataObjectGraphController::updateDisplay();
}
