#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"

QxrdROICoordinates::QxrdROICoordinates(QcepSettingsSaverWPtr saver,
                                       QxrdExperimentWPtr    exp,
                                       int                   roiType,
                                       double                left,
                                       double                top,
                                       double                right,
                                       double                bottom)
  : QcepObject("coords", exp.data()),
    m_RoiType(saver, this, "roiType", roiType, "ROI Type"),
    m_Coords(saver, this, "coords", QRectF(left, top, right-left, bottom-top), "ROI Coords")
{
}

QxrdROICoordinates::~QxrdROICoordinates()
{
}

QScriptValue QxrdROICoordinates::toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QxrdROICoordinates::fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICoordinates *qcoords = qobject_cast<QxrdROICoordinates*>(qobj);

    if (qcoords) {
      coords = QxrdROICoordinatesPtr(qcoords);
    }
  }
}

