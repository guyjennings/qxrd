#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrdroivector.h"
#include "qxrdroimodel.h"
#include <QtConcurrentMap>
#include "qxrdroi.h"
#include "qcepimagedata.h"
#include "qxrdprocessor.h"

QxrdROICalculator::QxrdROICalculator(QString name,
                                     QxrdROIVectorWPtr rois,
                                     QxrdROIModelWPtr model)
  : inherited(name),
    m_ROIVector(rois),
    m_ROIModel(model)
{
#ifndef QT_NO_DEBUG
  printf("Constructing ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::QxrdROICalculator(%p)\n", this);
  }
}

QxrdROICalculator::~QxrdROICalculator()
{
#ifndef QT_NO_DEBUG
  printf("Deleting ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::~QxrdROICalculator(%p)\n", this);
  }
}

void QxrdROICalculator::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QScriptValue QxrdROICalculator::toScriptValue(QScriptEngine *engine, const QxrdROICalculatorPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QxrdROICalculator::fromScriptValue(const QScriptValue &obj, QxrdROICalculatorPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICalculator *f = qobject_cast<QxrdROICalculator*>(qobj);

    if (f) {
      proc = QxrdROICalculatorPtr(f);
    }
  }
}

void QxrdROICalculator::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);

//  if (m_ROIModel) {
//    settings->beginGroup("coords");
//    m_ROIModel->readSettings(settings);
//    settings->endGroup();
//  }
}

void QxrdROICalculator::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

//  if (m_ROIModel) {
//    settings->beginGroup("coords");
//    m_ROIModel->writeSettings(settings);
//    settings->endGroup();
//  }
}

int QxrdROICalculator::roiCount()
{
  QxrdROIModelPtr m(m_ROIModel);

  if (m) {
    return m->roiCount();
  } else {
    return 0;
  }
}

QxrdROIModelWPtr QxrdROICalculator::roiModel()
{
  return m_ROIModel;
}

QxrdROIWPtr QxrdROICalculator::roi(int i)
{
  QxrdROIModelPtr m(m_ROIModel);

  if (m) {
    return m->roi(i);
  } else {
    return QxrdROIPtr();
  }
}

QVector<double> QxrdROICalculator::values(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QVector<double> res;

  QxrdROIModelPtr m(m_ROIModel);

  if (img && m) {
    m->recalculate(img, mask);

    int nVals = m->roiCount();

    for (int i=0; i<nVals; i++) {
      QxrdROIPtr roi = m->roi(i);

      if (roi) {
//        res.append();

        res += roi->values();
      }
    }
  }

  return res;
}

//double QxrdROICalculator::value(QcepImageDataBasePtr img, int i)
//{
//  double res = 0;

//  if (img && m_ROIModel) {
//    QxrdROIPtr roi = m_ROIModel->roi(i);

//    if (roi) {
//      switch (roi->get_RoiType()) {
//      case QxrdROI::SumInRectangle:
//        res = img->sumInRectangle(roi->get_Coords());
//        break;

//      case QxrdROI::AverageInRectangle:
//        res = img->averageInRectangle(roi->get_Coords());
//        break;

//      case QxrdROI::MinInRectangle:
//        res = img->minInRectangle(roi->get_Coords());
//        break;

//      case QxrdROI::MaxInRectangle:
//        res = img->maxInRectangle(roi->get_Coords());
//        break;

//      case QxrdROI::SumInEllipse:
//        res = img->sumInEllipse(roi->get_Coords());
//        break;

//      case QxrdROI::AverageInEllipse:
//        res = img->averageInEllipse(roi->get_Coords());
//        break;

//      case QxrdROI::MinInEllipse:
//        res = img->minInEllipse(roi->get_Coords());
//        break;

//      case QxrdROI::MaxInEllipse:
//        res = img->maxInEllipse(roi->get_Coords());
//        break;

//      case QxrdROI::SumInPeak:
//        res = img->sumInPeak(roi->get_Coords());
//        break;
//      }

//      QModelIndex index = m_ROIModel->index(i, QxrdROIModel::ValueCol);

//      m_ROIModel->setData(index, res, Qt::DisplayRole);
//    }
//  }

//  return res;
//}
