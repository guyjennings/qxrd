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
