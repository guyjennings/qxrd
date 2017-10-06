#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qxrdsettingssaver.h"
#include "qxrddebug.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include <QtConcurrentRun>

QxrdHistogramDialog::QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                                         QxrdExperimentWPtr expt,
                                         QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(expt),
  m_HistogramDialogSettings(settings)
{
  qRegisterMetaType<QwtPlotPiecewiseCurvePtr>("QwtPlotPiecewiseCurvePtr");

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::QxrdHistogramDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    m_HistogramPlot->init(set->histogramPlotSettings());
  }

  connect(this, &QxrdHistogramDialog::newHistogramCurves,
          this, &QxrdHistogramDialog::onNewHistogramCurves,
          Qt::QueuedConnection);
}

QxrdHistogramDialog::~QxrdHistogramDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::~QxrdHistogramDialog(%p)\n", this);
  }
}

void QxrdHistogramDialog::onProcessedImageAvailable(QxrdDoubleImageDataPtr image)
{
  m_Image = image;

  QtConcurrent::run(this, &QxrdHistogramDialog::recalculateHistogram);
}

void QxrdHistogramDialog::histogramSelectionChanged(QRectF rect)
{
  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    set->set_HistogramRect(rect);

    QtConcurrent::run(this, &QxrdHistogramDialog::recalculateHistogram);
  }
}

void QxrdHistogramDialog::updateHistogramNeeded()
{
  QtConcurrent::run(this, &QxrdHistogramDialog::recalculateHistogram);
}

void QxrdHistogramDialog::recalculateHistogram()
{
  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set && m_Image) {
    QxrdExperimentPtr expt(m_Experiment);

    if (expt) {
      QTime tic;
      tic.start();

      QRectF rect = set->get_HistogramRect();

      int nsum = m_Image->get_SummedExposures();

      if (nsum < 1) {
        nsum = 1;
      }

      QxrdAcquisitionPtr acq(expt->acquisition());

      double satlev = 60000;

      if (acq) {
        satlev = acq->get_OverflowLevel();
      }

      double min = 0, max = satlev*nsum;

      const int nbins=1000;

      QcepDoubleVector x0(nbins), h0(nbins), h1(nbins);

      for (int i=0; i<nbins; i++) {
//        double x = min+i*(max-min)/1000.0;
        x0[i] = i*100.0/(double)nbins;
        h0[i] = 0;
        h1[i] = 0;
      }

      int width = m_Image->get_Width();
      int height= m_Image->get_Height();

      double *data = m_Image->data();

      for (int i=0; i<width; i++) {
        for (int j=0; j<height; j++) {
          double v = *data++;

          int n;

          if (v<min) {
            n=0;
          } else if (v>max) {
            n=nbins-1;
          } else {
            n=(nbins-1)*(v-min)/(max-min);
          }

          if (n<0) {
            n=0;
          }

          if (n>=nbins) {
            n=nbins-1;
          }

          h0[n]++;

          if (rect.contains(i,j)) {
            h1[n]++;
          }
        }
      }


      QwtPlotPiecewiseCurvePtr pc0 =
          QwtPlotPiecewiseCurvePtr(new QwtPlotPiecewiseCurve(m_HistogramPlot, "Entire Image"));

      pc0->setSamples(x0, h0);
      pc0->setPen(QPen(Qt::red));

//      pc0->attach(m_HistogramPlot);

      QwtPlotPiecewiseCurvePtr pc1 =
          QwtPlotPiecewiseCurvePtr(new QwtPlotPiecewiseCurve(m_HistogramPlot,
                                                             tr("[%1,%2]-[%3,%4]")
                                                             .arg(rect.left()).arg(rect.bottom())
                                                             .arg(rect.right()).arg(rect.top())));

      pc1->setSamples(x0, h1);
      pc1->setPen(QPen(Qt::darkRed));

//      pc1->attach(m_HistogramPlot);


      if (qcepDebug(DEBUG_HISTOGRAM)) {
        expt -> printMessage(tr("Histogram of data took %1 msec").arg(tic.elapsed()));
      }

      emit newHistogramCurves(pc0, pc1);
    }
  }
}

void QxrdHistogramDialog::onNewHistogramCurves(QwtPlotPiecewiseCurvePtr totalCurve, QwtPlotPiecewiseCurvePtr selectCurve)
{
  m_TotalCurve = totalCurve;
  m_SelectCurve = selectCurve;

  m_HistogramPlot->detachItems();

  if (m_TotalCurve) {
    m_TotalCurve->attach(m_HistogramPlot);
  }

  if (m_SelectCurve) {
    m_SelectCurve->attach(m_HistogramPlot);
  }

  m_HistogramPlot->replot();
}
