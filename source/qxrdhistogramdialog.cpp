#include "qxrdhistogramdialog.h"
#include "ui_qxrdhistogramdialog.h"
#include <QSettings>
#include "qcepsettingssaver.h"
#include "qxrddebug.h"
#include "qwt_plot_piecewise_curve.h"
#include "qxrdexperiment.h"
#include "qxrdacquisition.h"
#include "qxrdhistogramdialogsettings.h"
#include "qxrdhistogramplotsettings.h"

QxrdHistogramDialog::QxrdHistogramDialog(QxrdHistogramDialogSettingsWPtr settings,
                                         QxrdExperimentWPtr expt,
                                         QWidget *parent) :
  QDockWidget(parent),
  m_Experiment(expt),
  m_HistogramDialogSettings(settings)
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::QxrdHistogramDialog(%p)\n", this);
  }

  setupUi(this);

  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    m_HistogramPlot->init(set->histogramPlotSettings());
  }
}

QxrdHistogramDialog::~QxrdHistogramDialog()
{
  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdHistogramDialog::~QxrdHistogramDialog(%p)\n", this);
  }
}

void QxrdHistogramDialog::onProcessedImageAvailable(QcepDoubleImageDataPtr image, QcepMaskDataPtr /*overflow*/)
{
  m_Image = image;

  recalculateHistogram();
}

void QxrdHistogramDialog::histogramSelectionChanged(QRectF rect)
{
  QxrdHistogramDialogSettingsPtr set(m_HistogramDialogSettings);

  if (set) {
    set->set_HistogramRect(rect);

    recalculateHistogram();
  }
}

void QxrdHistogramDialog::updateHistogramNeeded()
{
  recalculateHistogram();
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

      m_HistogramPlot->detachItems();

      QwtPlotPiecewiseCurve *pc0 = new QwtPlotPiecewiseCurve(m_HistogramPlot, "Entire Image");

      pc0->setSamples(x0, h0);
      pc0->setPen(QPen(Qt::red));

      pc0->attach(m_HistogramPlot);

      QwtPlotPiecewiseCurve *pc1 = new QwtPlotPiecewiseCurve(m_HistogramPlot,
                                                             tr("[%1,%2]-[%3,%4]")
                                                             .arg(rect.left()).arg(rect.bottom())
                                                             .arg(rect.right()).arg(rect.top()));

      pc1->setSamples(x0, h1);
      pc1->setPen(QPen(Qt::darkRed));

      pc1->attach(m_HistogramPlot);

      m_HistogramPlot->replot();

      if (qcepDebug(DEBUG_HISTOGRAM)) {
        expt -> printMessage(tr("Histogram of data took %1 msec").arg(tic.elapsed()));
      }
    }
  }
}
