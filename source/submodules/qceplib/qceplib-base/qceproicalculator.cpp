#include "qceproicalculator.h"
#include "qcepdebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qceproivector.h"
#include "qceproimodel.h"
#include <QtConcurrentMap>
#include "qceproi.h"
#include "qcepimagedata.h"

QcepROICalculator::QcepROICalculator(QString name,
                                     QcepROIVectorWPtr rois,
                                     QcepROIModelWPtr model)
  : inherited(name),
    m_ROIVector(rois),
    m_ROIModel(model)
{
#ifndef QT_NO_DEBUG
  printf("Constructing ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepROICalculator::QcepROICalculator(%p)\n", this);
  }
}

QcepROICalculator::~QcepROICalculator()
{
#ifndef QT_NO_DEBUG
  printf("Deleting ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QcepROICalculator::~QcepROICalculator(%p)\n", this);
  }
}

void QcepROICalculator::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QScriptValue QcepROICalculator::toScriptValue(QScriptEngine *engine, const QcepROICalculatorPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QcepROICalculator::fromScriptValue(const QScriptValue &obj, QcepROICalculatorPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepROICalculator *f = qobject_cast<QcepROICalculator*>(qobj);

    if (f) {
      proc = QcepROICalculatorPtr(f);
    }
  }
}

void QcepROICalculator::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

//  if (m_ROIModel) {
//    settings->beginGroup("coords");
//    m_ROIModel->readSettings(settings);
//    settings->endGroup();
//  }
}

void QcepROICalculator::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

//  if (m_ROIModel) {
//    settings->beginGroup("coords");
//    m_ROIModel->writeSettings(settings);
//    settings->endGroup();
//  }
}

int QcepROICalculator::roiCount()
{
  QcepROIModelPtr m(m_ROIModel);

  if (m) {
    return m->roiCount();
  } else {
    return 0;
  }
}

QcepROIModelWPtr QcepROICalculator::roiModel()
{
  return m_ROIModel;
}

QcepROIWPtr QcepROICalculator::roi(int i)
{
  QcepROIModelPtr m(m_ROIModel);

  if (m) {
    return m->roi(i);
  } else {
    return QcepROIPtr();
  }
}

QVector<double> QcepROICalculator::values(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QVector<double> res;

  QcepROIModelPtr m(m_ROIModel);

  if (img && m) {
    m->recalculate(img, mask);

    int nVals = m->roiCount();

    for (int i=0; i<nVals; i++) {
      QcepROIPtr roi = m->roi(i);

      if (roi) {
//        res.append();

        res += roi->values();
      }
    }
  }

  return res;
}

//double QcepROICalculator::value(QcepImageDataBasePtr img, int i)
//{
//  double res = 0;

//  if (img && m_ROIModel) {
//    QcepROIPtr roi = m_ROIModel->roi(i);

//    if (roi) {
//      switch (roi->get_RoiType()) {
//      case QcepROI::SumInRectangle:
//        res = img->sumInRectangle(roi->get_Coords());
//        break;

//      case QcepROI::AverageInRectangle:
//        res = img->averageInRectangle(roi->get_Coords());
//        break;

//      case QcepROI::MinInRectangle:
//        res = img->minInRectangle(roi->get_Coords());
//        break;

//      case QcepROI::MaxInRectangle:
//        res = img->maxInRectangle(roi->get_Coords());
//        break;

//      case QcepROI::SumInEllipse:
//        res = img->sumInEllipse(roi->get_Coords());
//        break;

//      case QcepROI::AverageInEllipse:
//        res = img->averageInEllipse(roi->get_Coords());
//        break;

//      case QcepROI::MinInEllipse:
//        res = img->minInEllipse(roi->get_Coords());
//        break;

//      case QcepROI::MaxInEllipse:
//        res = img->maxInEllipse(roi->get_Coords());
//        break;

//      case QcepROI::SumInPeak:
//        res = img->sumInPeak(roi->get_Coords());
//        break;
//      }

//      QModelIndex index = m_ROIModel->index(i, QcepROIModel::ValueCol);

//      m_ROIModel->setData(index, res, Qt::DisplayRole);
//    }
//  }

//  return res;
//}
