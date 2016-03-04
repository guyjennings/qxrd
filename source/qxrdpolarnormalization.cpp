#include "qxrdpolarnormalization.h"
#include "qxrdexperiment.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qtconcurrentrun.h"
#include "qxrdpolarintensityfitter.h"
#include "qxrddataprocessor.h"
#include <qmath.h>

QxrdPolarNormalization::QxrdPolarNormalization(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp) :
  QcepObject("polarNormalization", NULL),
  m_Source(saver, this, "source", "Polar/image", "Source for normalization operation"),
  m_Destination(saver, this, "destination", "Polar/normed", "Destination for normalization operation"),
  m_Integrated(saver, this, "integrated", "Polar/integrated", "Integrated result"),
  m_SelfNormalize(saver, this, "selfNormalize", false, "Self-Normalize integrated curves"),
  m_SelfNormalizeMin(saver, this, "selfNormalizeMin", 0, "Self-Normalize Range Minimum"),
  m_SelfNormalizeMax(saver, this, "selfNormalizeMax", 0, "Self-Normalize Range Maximum"),
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
    QcepDoubleImageDataPtr dst = ds->image(get_Destination());

    if (dst == NULL) {
      dst = ds->newImage(get_Destination());
    }

    QcepIntegratedDataPtr  integ = ds->integratedData(get_Integrated());

    if (integ == NULL) {
      integ = ds->newIntegratedData(get_Integrated(), 0);
    }

    if (img && dst && integ) {
      int nCols = img->get_Width();
      int nRows = img->get_Height();

      dst->resize(nCols, nRows);
      integ->resize(nCols);

      dst->set_HStart(img->get_HStart());
      dst->set_HStep(img->get_HStep());
      dst->set_HUnits(img->get_HUnits());
      dst->set_HLabel(img->get_HLabel());

      dst->set_VStart(img->get_VStart());
      dst->set_VStep(img->get_VStep());
      dst->set_VUnits(img->get_VUnits());
      dst->set_VLabel(img->get_VLabel());

      dst->set_Title(img->get_Title());

      integ->set_Title(img->get_Title());

      QVector< QFuture<void> > res;

      for (int i=0; i<nCols; i++) {
        res.append(QtConcurrent::run(this, &QxrdPolarNormalization::executeCol, integ, dst, img, i));
      }

      for (int i=0; i<nCols; i++) {
        res[i].waitForFinished();
      }

      if (get_SelfNormalize()) {
        integ->selfNormalize(get_SelfNormalizeMin(), get_SelfNormalizeMax());
      }

      QxrdDataProcessorPtr proc(expt->dataProcessor());

      if (proc) {
        proc->displayIntegratedData(integ);
      }
    }
  }
}

void QxrdPolarNormalization::executeCol(QcepIntegratedDataPtr  integ,
                                        QcepDoubleImageDataPtr dst,
                                        QcepDoubleImageDataPtr img, int col)
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
      dst->setValue(col, i, avg+amp*cos(2.0*x*M_PI/180.0));
    }
//    dst->setValue(col, 0, avg);
//    dst->setValue(col, 1, amp);
//    dst->setValue(col, 2, amp/avg);

    integ->setValue(col, img->hValue(col), avg);
  }
}
