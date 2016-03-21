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
  m_ColumnScan(saver, this, "columnScan", "Polar/columnScan", "Data Column Scan result"),
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


    QcepDoubleImageDataPtr dst;

    if (get_Destination().length()>0) {
      dst = ds->image(get_Destination());

      if (dst == NULL) {
        dst = ds->newImage(get_Destination());
      }
    }

    QcepIntegratedDataPtr  integ;

    if (get_Integrated().length()>0) {
      integ = ds->integratedData(get_Integrated());

      if (integ == NULL) {
        integ = ds->newIntegratedData(get_Integrated(), 0);
      }
    }

    QcepDataColumnScanPtr cols;

    if (get_ColumnScan().length()>0) {
      cols = ds->columnScan(get_ColumnScan());

      if (cols == NULL) {
        cols = ds->newColumnScan(get_ColumnScan());
      }
    }

    if (img && dst && integ && cols) {
      int nCols = img->get_Width();
      int nRows = img->get_Height();

      dst->resize(nCols, nRows);

      cols->clear();
      QcepDataColumnPtr xc = cols->appendColumn(img->get_HLabel());
      QcepDataColumnPtr av = cols->appendColumn("avg");
      QcepDataColumnPtr am = cols->appendColumn("amp");
      QcepDataColumnPtr rt = cols->appendColumn("amp/avg");

      xc->set_ColumnType(QcepDataColumn::XValueColumn);
      av->set_ColumnType(QcepDataColumn::WeightColumn);
      am->set_ColumnType(QcepDataColumn::CountsColumn);
      rt->set_ColumnType(QcepDataColumn::RatioColumn);
      rt->set_Column1(2);
      rt->set_Column2(1);

      cols->resizeRows(nCols);

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
      cols->set_Title(img->get_Title());
      integ->set_Title(img->get_Title());

      QVector< QFuture<void> > res;

      for (int i=0; i<nCols; i++) {
        res.append(QtConcurrent::run(this, &QxrdPolarNormalization::executeCol, cols, dst, img, i));
      }

      for (int i=0; i<nCols; i++) {
        res[i].waitForFinished();
      }

      for (int i=0; i<nCols; i++) {
        integ->setValue(i, cols->value(0,i), cols->value(1,i));
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

void QxrdPolarNormalization::executeCol(QcepDataColumnScanPtr  integ,
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

    integ->setValue(0, col, img->hValue(col));
    integ->setValue(1, col, avg);
    integ->setValue(2, col, amp);
    integ->setValue(3, col, amp/avg);
  }
}
