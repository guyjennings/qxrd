#include "qxrdpolarnormalization.h"
#include "qxrdexperiment.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qtconcurrentrun.h"
#include "qxrdpolarintensityfitter.h"

QxrdPolarNormalization::QxrdPolarNormalization(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp) :
  QcepObject("polarNormalization", NULL),
  m_Source(saver, this, "source", "Polar/image", "Source for normalization operation"),
  m_Destination(saver, this, "destination", "Polar/normed", "Destination for normalization operation"),
  m_Saver(saver),
  m_Experiment(exp)
{
}

void QxrdPolarNormalization::execute()
{
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr ds = expt->dataset();

    QcepDoubleImageDataPtr img = ds->image(get_Source());
    QcepDoubleImageDataPtr dst = ds->newImage(get_Destination());

    if (img && dst) {
      int nCols = img->get_Width();
      int nRows = img->get_Height();

      dst->resize(nCols, nRows);

      dst->set_HStart(img->get_HStart());
      dst->set_HStep(img->get_HStep());
      dst->set_HUnits(img->get_HUnits());
      dst->set_HLabel(img->get_HLabel());

      QVector< QFuture<void> > res;

      for (int i=0; i<nCols; i++) {
        res.append(QtConcurrent::run(this, &QxrdPolarNormalization::executeCol, dst, img, i));
      }

      for (int i=0; i<nCols; i++) {
        res[i].waitForFinished();
      }
    }
  }
}

void QxrdPolarNormalization::executeCol(QcepDoubleImageDataPtr dst, QcepDoubleImageDataPtr img, int col)
{
  QxrdPolarIntensityFitter fitter(img, col);

  if (fitter.fit()) {
    double amp = fitter.fittedAmplitude();
    double avg = fitter.fittedAverage();

    int nRows = dst->get_Height();
    double vStart = dst->get_VStart();
    double vStep  = dst->get_VStep();

    for (int i=0; i<nRows; i++) {
      double x = vStart + i*vStep;
      dst->setValue(col, i, avg+amp*cos(x*M_PI/180.0));
    }
//    dst->setValue(col, 0, avg);
//    dst->setValue(col, 1, amp);
//    dst->setValue(col, 2, amp/avg);
  }
}
