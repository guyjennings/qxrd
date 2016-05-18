#include "qxrdpolarnormalization.h"
#include "qxrdexperiment.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qtconcurrentrun.h"
#include "qxrdpolarintensityfitter.h"
#include "qxrddataprocessor.h"
#include <qmath.h>
#include "qcepallocator.h"

QxrdPolarNormalization::QxrdPolarNormalization(QString name) :
  QcepSerializableObject(name),
  m_OutputType(this, "outputType", IntegratedOutput, "Output Type (0=Integ, 1=Fit, 2=Deltas"),
  m_SelfNormalize(this, "selfNormalize", false, "Self-Normalize integrated curves"),
  m_SelfNormalizeMin(this, "selfNormalizeMin", 0, "Self-Normalize Range Minimum"),
  m_SelfNormalizeMax(this, "selfNormalizeMax", 0, "Self-Normalize Range Maximum")
{
}

QxrdPolarNormalizationPtr QxrdPolarNormalization::newPolarNormalization()
{
  QxrdPolarNormalizationPtr norm(new QxrdPolarNormalization("polarNormalization"));

  return norm;
}

//void QxrdPolarNormalization::execute()
//{
//  QxrdExperimentPtr expt(m_Experiment);

//  if (expt) {
//    QcepDatasetModelPtr ds = expt->dataset();

//    QcepDoubleImageDataPtr img = ds->image(get_Source());


//    QcepDoubleImageDataPtr dst;

//    if (get_Destination().length()>0) {
//      dst = ds->image(get_Destination());

//      if (dst == NULL) {
//        dst = ds->newImage(get_Destination());
//      }
//    }

//    QcepIntegratedDataPtr  integ;

//    if (get_Integrated().length()>0) {
//      integ = ds->integratedData(get_Integrated());

//      if (integ == NULL) {
//        integ = ds->newIntegratedData(get_Integrated(), 0);
//      }
//    }

//    QcepDataColumnScanPtr cols;

//    if (get_ColumnScan().length()>0) {
//      cols = ds->columnScan(get_ColumnScan());

//      if (cols == NULL) {
//        cols = ds->newColumnScan(get_ColumnScan());
//      }
//    }

//    if (img && dst && integ && cols) {
//      int nCols = img->get_Width();
//      int nRows = img->get_Height();

//      dst->resize(nCols, nRows);

//      cols->clear();
//      QcepDataColumnPtr xc = cols->appendColumn(img->get_HLabel());
//      QcepDataColumnPtr av = cols->appendColumn("avg");
//      QcepDataColumnPtr am = cols->appendColumn("amp");
//      QcepDataColumnPtr rt = cols->appendColumn("amp/avg");

//      xc->set_ColumnType(QcepDataColumn::XValueColumn);
//      av->set_ColumnType(QcepDataColumn::WeightColumn);
//      am->set_ColumnType(QcepDataColumn::CountsColumn);
//      rt->set_ColumnType(QcepDataColumn::RatioColumn);
//      rt->set_Column1(2);
//      rt->set_Column2(1);

//      cols->resizeRows(nCols);

//      integ->resize(nCols);

//      dst->set_HStart(img->get_HStart());
//      dst->set_HStep(img->get_HStep());
//      dst->set_HUnits(img->get_HUnits());
//      dst->set_HLabel(img->get_HLabel());

//      dst->set_VStart(img->get_VStart());
//      dst->set_VStep(img->get_VStep());
//      dst->set_VUnits(img->get_VUnits());
//      dst->set_VLabel(img->get_VLabel());

//      dst->set_Name(img->get_Name());
//      cols->set_Name(img->get_Name());
//      integ->set_Name(img->get_Name());

//      QVector< QFuture<void> > res;

//      for (int i=0; i<nCols; i++) {
//        res.append(QtConcurrent::run(this, &QxrdPolarNormalization::executeCol, cols, dst, img, i));
//      }

//      for (int i=0; i<nCols; i++) {
//        res[i].waitForFinished();
//      }

//      for (int i=0; i<nCols; i++) {
//        integ->setValue(i, cols->value(0,i), cols->value(1,i));
//      }

//      if (get_SelfNormalize()) {
//        integ->selfNormalize(get_SelfNormalizeMin(), get_SelfNormalizeMax());
//      }

//      QxrdDataProcessorPtr proc(expt->dataProcessor());

//      if (proc) {
//        proc->displayIntegratedData(integ);
//      }
//    }
//  }
//}

void QxrdPolarNormalization::executeCol(QcepDataColumnScanPtr  integ,
                                        QcepDoubleImageDataPtr dst,
                                        QcepDoubleImageDataPtr img, int col)
{
  QxrdPolarIntensityFitter fitter(img, col);

  if (fitter.fit()) {
    double amp = fitter.fittedAmplitude();
    double avg = fitter.fittedAverage();

    if (dst) {
      int nRows = dst->get_Height();
      double vStart = dst->get_VStart();
      double vStep  = dst->get_VStep();

      for (int i=0; i<nRows; i++) {
        double x = vStart + i*vStep;
        dst->setValue(col, i, avg+amp*cos(2.0*x*M_PI/180.0));
      }
    }

    if (integ) {
      integ->setValue(0, col, img->hValue(col));
      integ->setValue(1, col, avg);
      integ->setValue(2, col, amp);
      integ->setValue(3, col, amp/avg);
    }
  }
}

QcepDataObjectPtr QxrdPolarNormalization::transform(QcepDoubleImageDataPtr img)
{
  QcepDataObjectPtr res;

  if (img) {
    QcepDoubleImageDataPtr fit;
    QcepDataColumnScanPtr  integ;

    int nCols = img->get_Width();
    int nRows = img->get_Height();

    if (get_OutputType() == IntegratedOutput) {
      integ = QcepAllocator::newColumnScan(img->get_FileBase(), QStringList(), 0, QcepAllocator::NullIfNotAvailable);

      QcepDataColumnPtr xc = integ->appendColumn(img->get_HLabel());
      QcepDataColumnPtr av = integ->appendColumn("avg");
      QcepDataColumnPtr am = integ->appendColumn("amp");
      QcepDataColumnPtr rt = integ->appendColumn("amp/avg");

      xc->set_ColumnType(QcepDataColumn::XValueColumn);
      av->set_ColumnType(QcepDataColumn::WeightColumn);
      am->set_ColumnType(QcepDataColumn::CountsColumn);
      rt->set_ColumnType(QcepDataColumn::RatioColumn);
      rt->set_Column1(2);
      rt->set_Column2(1);

      integ->resizeRows(nCols);

      integ->setNameAndSuffix(img->get_FileBase(), "integ");

      res = integ;
    } else if (get_OutputType() == FittedData || get_OutputType() == FittedDeltas) {
      fit = QcepAllocator::newDoubleImage(img->get_FileBase(), nCols, nRows, QcepAllocator::NullIfNotAvailable);

      fit->set_HStart(img->get_HStart());
      fit->set_HStep(img->get_HStep());
      fit->set_HUnits(img->get_HUnits());
      fit->set_HLabel(img->get_HLabel());

      fit->set_VStart(img->get_VStart());
      fit->set_VStep(img->get_VStep());
      fit->set_VUnits(img->get_VUnits());
      fit->set_VLabel(img->get_VLabel());

      fit->setNameAndSuffix(img->get_FileBase(), "fit");

      res = fit;
    }

    QVector< QFuture<void> > w;

    for (int i=0; i<nCols; i++) {
      w.append(QtConcurrent::run(this, &QxrdPolarNormalization::executeCol, integ, fit, img, i));
    }

    for (int i=0; i<nCols; i++) {
      w[i].waitForFinished();
    }

    if (fit) {
      if (get_OutputType() == FittedData) {
        fit->setNameAndSuffix(img->get_FileBase(), "fit");
      } else {
        fit->setNameAndSuffix(img->get_FileBase(), "fitdel");
        fit->subtract(img);
        fit->multiply(-1);
      }
    }
  }

  return res;
}
