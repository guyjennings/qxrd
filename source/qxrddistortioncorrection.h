#ifndef QXRDDISTORTIONCORRECTION_H
#define QXRDDISTORTIONCORRECTION_H

#include "qcepmacros.h"
#include <QObject>
#include "qcepproperty.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdobjectnamer.h"

class QxrdDistortionCorrection : public QObject
{
  Q_OBJECT
public:
  explicit QxrdDistortionCorrection(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt);

private:
  QxrdObjectNamer       m_ObjectNamer;

public:
  Q_PROPERTY(QString distortionImagePath READ get_DistortionImagePath WRITE set_DistortionImagePath)
  QCEP_STRING_PROPERTY(DistortionImagePath)

  Q_PROPERTY(QPointF distortionP0 READ get_DistortionP0 WRITE set_DistortionP0)
  QCEP_DOUBLE_POINT_PROPERTY(DistortionP0)

  Q_PROPERTY(QPointF distortionP1 READ get_DistortionP1 WRITE set_DistortionP1)
  QCEP_DOUBLE_POINT_PROPERTY(DistortionP1)

  Q_PROPERTY(QPointF distortionP2 READ get_DistortionP2 WRITE set_DistortionP2)
  QCEP_DOUBLE_POINT_PROPERTY(DistortionP2)

signals:

public slots:
  QPointF transform(QPointF pt);
  QPointF inverse(QPointF pt);

public:
  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

private:
  mutable QMutex             m_Mutex;
  QxrdExperimentWPtr         m_Experiment;
  QxrdDoubleImageDataPtr     m_Data;
};

#endif // QXRDDISTORTIONCORRECTION_H
