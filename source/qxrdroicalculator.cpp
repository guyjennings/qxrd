#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrdroicoordinateslistmodel.h"
#include <QtConcurrentMap>
#include "qxrdroicoordinates.h"
#include "qcepimagedata.h"
#include "qxrddetectorprocessor.h"

QxrdROICalculator::QxrdROICalculator(QString name)
  : QcepObject(name),
    m_ROICoordinatesModel(new QxrdROICoordinatesListModel())
{
#ifndef QT_NO_DEBUG
  printf("Constructing ROI Calculator\n");
#endif

  if (qcepDebug(DEBUG_CONSTRUCTORS)) {
    printf("QxrdROICalculator::QxrdROICalculator(%p)\n", this);
  }
}

QxrdROICalculatorPtr QxrdROICalculator::newROICalculator()
{
  QxrdROICalculatorPtr calc(new QxrdROICalculator("roiCalculator"));

  return calc;
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

void QxrdROICalculator::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  if (m_ROICoordinatesModel) {
    m_ROICoordinatesModel->readSettings(settings, section+"/coords");
  }
}

void QxrdROICalculator::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  if (m_ROICoordinatesModel) {
    m_ROICoordinatesModel->writeSettings(settings, section+"/coords");
  }
}

int QxrdROICalculator::roiCount()
{
  if (m_ROICoordinatesModel) {
    return m_ROICoordinatesModel->roiCount();
  } else {
    return 0;
  }
}

QxrdROICoordinatesListModelWPtr QxrdROICalculator::roiModel()
{
  return m_ROICoordinatesModel;
}

QxrdROICoordinatesWPtr QxrdROICalculator::roi(int i)
{
  if (m_ROICoordinatesModel) {
    return m_ROICoordinatesModel->roi(i);
  } else {
    return QxrdROICoordinatesPtr();
  }
}

QVector<double> QxrdROICalculator::values(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  QVector<double> res;

  if (img && m_ROICoordinatesModel) {
    m_ROICoordinatesModel->recalculate(img, mask);

    int nVals = m_ROICoordinatesModel->roiCount();

    for (int i=0; i<nVals; i++) {
      QxrdROICoordinatesPtr roi = m_ROICoordinatesModel->roi(i);

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

//  if (img && m_ROICoordinatesModel) {
//    QxrdROICoordinatesPtr roi = m_ROICoordinatesModel->roi(i);

//    if (roi) {
//      switch (roi->get_RoiType()) {
//      case QxrdROICoordinates::SumInRectangle:
//        res = img->sumInRectangle(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::AverageInRectangle:
//        res = img->averageInRectangle(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::MinInRectangle:
//        res = img->minInRectangle(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::MaxInRectangle:
//        res = img->maxInRectangle(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::SumInEllipse:
//        res = img->sumInEllipse(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::AverageInEllipse:
//        res = img->averageInEllipse(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::MinInEllipse:
//        res = img->minInEllipse(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::MaxInEllipse:
//        res = img->maxInEllipse(roi->get_Coords());
//        break;

//      case QxrdROICoordinates::SumInPeak:
//        res = img->sumInPeak(roi->get_Coords());
//        break;
//      }

//      QModelIndex index = m_ROICoordinatesModel->index(i, QxrdROICoordinatesListModel::ValueCol);

//      m_ROICoordinatesModel->setData(index, res, Qt::DisplayRole);
//    }
//  }

//  return res;
//}
