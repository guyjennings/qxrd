#include "qxrdroicoordinateslist.h"
#include "qxrdroicoordinates.h"
#include "qcepmutexlocker.h"
#include "qxrdexperiment.h"

QxrdROICoordinatesList::QxrdROICoordinatesList(QcepSettingsSaverWPtr saver,
                                               QxrdExperimentWPtr    exp)
  : QcepObject("roiList", exp.data()),
    m_Saver(saver),
    m_Experiment(exp),
    m_RoiCount(saver, this, "roiCount", 0, "Number of ROIs")
{
}

QxrdROICoordinatesList::~QxrdROICoordinatesList()
{
}

void QxrdROICoordinatesList::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::readSettings(settings, section);

  int n = settings->beginReadArray(section+"/roi");

  m_ROICoordinates.resize(0);

  for (int i=0; i<n; i++) {
    settings->setArrayIndex(i);

    int roiType = settings->value("roiType", 0).toInt();

    QxrdROICoordinatesPtr roi = newROI(roiType);

    if (roi) {
      roi->readSettings(settings, "");
      m_ROICoordinates.append(roi);
    }
  }

  settings->endArray();
}

void QxrdROICoordinatesList::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepObject::writeSettings(settings, section);

  settings->beginWriteArray(section+"/roi");

  for (int i=0; i<m_ROICoordinates.count(); i++) {
    settings->setArrayIndex(i);
    QxrdROICoordinatesPtr roi = m_ROICoordinates.value(i);

    if (roi) {
      roi->writeSettings(settings, "");
    }
  }

  settings->endArray();
}

QScriptValue QxrdROICoordinatesList::toScriptValue(QScriptEngine *engine,
                                                   const QxrdROICoordinatesListPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QxrdROICoordinatesList::fromScriptValue(const QScriptValue &obj,
                                             QxrdROICoordinatesListPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICoordinatesList *qcoords = qobject_cast<QxrdROICoordinatesList*>(qobj);

    if (qcoords) {
      coords = QxrdROICoordinatesListPtr(qcoords);
    }
  }
}

QxrdROICoordinatesPtr QxrdROICoordinatesList::newROI(int roiType)
{
  return QxrdROICoordinatesPtr(new QxrdROICoordinates(m_Saver, m_Experiment, roiType));
}

void QxrdROICoordinatesList::clear()
{
  m_ROICoordinates.resize(0);

  set_RoiCount(0);
}

void QxrdROICoordinatesList::appendROI(int roiType, double left, double top, double right, double bottom)
{
  m_ROICoordinates.append(
        QxrdROICoordinatesPtr(
          new QxrdROICoordinates(m_Saver, m_Experiment, roiType, left, top, right, bottom)));

  set_RoiCount(m_ROICoordinates.count());
}

QxrdROICoordinatesPtr QxrdROICoordinatesList::roi(int i)
{
  return m_ROICoordinates.value(i);
}