#include "qxrdroicalculator.h"
#include "qxrddebug.h"
#include <stdio.h>
#include "qcepmutexlocker.h"
#include "qxrdroicoordinateslistmodel.h"
#include "qxrdroicoordinateslist.h"
#include <QtConcurrentMap>
#include "qxrdroicoordinates.h"
#include "qcepimagedata.h"

QxrdROICalculator::QxrdROICalculator(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdDetectorProcessorWPtr proc)
  : QcepObject("ROIcalculator", NULL),
    m_Saver(saver),
    m_Experiment(exp),
    m_Processor(proc),
    m_ROICoordinates(new QxrdROICoordinatesList(saver, exp)),
    m_ROICoordinatesModel(new QxrdROICoordinatesListModel(m_ROICoordinates))
{
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

//  if (m_Processor) {
//    m_Processor->readSettings(settings, section+"/processor");
//  }

  if (m_ROICoordinates) {
    m_ROICoordinates->readSettings(settings, section+"/coords");
  }
}

void QxrdROICalculator::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

//  if (m_Processor) {
//    m_Processor->writeSettings(settings, section+"/processor");
//  }

  if (m_ROICoordinates) {
    m_ROICoordinates->writeSettings(settings, section+"/coords");
  }
}

QxrdROICoordinatesListModelPtr QxrdROICalculator::coordinatesModel()
{
  return m_ROICoordinatesModel;
}

QxrdROICoordinatesListPtr QxrdROICalculator::coordinates()
{
  return m_ROICoordinates;
}

QxrdROICoordinatesPtr QxrdROICalculator::coordinate(int i)
{
  if (m_ROICoordinates) {
    return m_ROICoordinates->roi(i);
  } else {
    return QxrdROICoordinatesPtr();
  }
}

QVector<double> QxrdROICalculator::values(QcepImageDataBasePtr img)
{
  QVector<double> res;

  if (img && m_ROICoordinates) {
    int nVals = m_ROICoordinates->get_RoiCount();

    for (int i=0; i<nVals; i++) {
      res.append(value(img, i));
    }
  }

  return res;
}

double QxrdROICalculator::value(QcepImageDataBasePtr img, int i)
{
  double res = 0;

  if (img && m_ROICoordinates) {
    QxrdROICoordinatesPtr roi = m_ROICoordinates->roi(i);

    if (roi) {
      switch (roi->get_RoiType()) {
      case QxrdROICoordinates::SumInRectangle:
        res = img->sumInRectangle(roi->get_Coords());
        break;

      case QxrdROICoordinates::AverageInRectangle:
        res = img->averageInRectangle(roi->get_Coords());
        break;

      case QxrdROICoordinates::SumInEllipse:
        res = img->sumInEllipse(roi->get_Coords());
        break;

      case QxrdROICoordinates::AverageInEllipse:
        res = img->averageInEllipse(roi->get_Coords());
        break;

      case QxrdROICoordinates::SumInPeak:
        res = img->sumInPeak(roi->get_Coords());
        break;
      }

      QModelIndex index = m_ROICoordinatesModel->index(i, QxrdROICoordinatesListModel::ValueCol);

      m_ROICoordinatesModel->setData(index, res, Qt::DisplayRole);
    }
  }

  return res;
}
