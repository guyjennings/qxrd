#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrdroimodel.h"
#include <QtConcurrentMap>
#include "qxrdroi.h"
#include "qcepimagedata.h"
#include "qxrdprocessor.h"

QxrdROICalculator::QxrdROICalculator(QString name)
  : inherited(name),
    m_ROIModel(new QxrdROIModel())
{
#ifndef QT_NO_DEBUG
  printf("Constructing ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::QxrdROICalculator(%p)\n", this);
  }

  m_ROIModel = QxrdROIModelPtr(
        new QxrdROIModel());
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

void QxrdROICalculator::initialize(QObjectWPtr parent)
{
  inherited::initialize(parent);

//  m_ROIModel -> initialize(parent);
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

  if (m_ROIModel) {
    settings->beginGroup("coords");
    m_ROIModel->readSettings(settings);
    settings->endGroup();
  }
}

void QxrdROICalculator::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);

  if (m_ROIModel) {
    settings->beginGroup("coords");
    m_ROIModel->writeSettings(settings);
    settings->endGroup();
  }
}

int QxrdROICalculator::roiCount()
{
  if (m_ROIModel) {
    return m_ROIModel->roiCount();
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
  if (m_ROIModel) {
    return m_ROIModel->roi(i);
  } else {
    return QxrdROIPtr();
  }
}

QVector<double> QxrdROICalculator::values(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QVector<double> res;

  if (img && m_ROIModel) {
    m_ROIModel->recalculate(img, mask);

    int nVals = m_ROIModel->roiCount();

    for (int i=0; i<nVals; i++) {
      QxrdROIPtr roi = m_ROIModel->roi(i);

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
