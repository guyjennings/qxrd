#ifndef QXRDPOLARTRANSFORM_H
#define QXRDPOLARTRANSFORM_H

#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"

#include "qxrdexperiment-ptr.h"
#include "qcepsettingssaver-ptr.h"

class QxrdPolarTransform : public QcepObject
{
  Q_OBJECT

public:
  QxrdPolarTransform(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr exp);

public:
  Q_PROPERTY(QString destination READ get_Destination WRITE set_Destination)
  QCEP_STRING_PROPERTY(Destination)

  Q_PROPERTY(int outputType READ get_OutputType WRITE set_OutputType)
  QCEP_INTEGER_PROPERTY(OutputType)

  Q_PROPERTY(int polarUnits READ get_PolarUnits WRITE set_PolarUnits)
  QCEP_INTEGER_PROPERTY(PolarUnits)

  Q_PROPERTY(double polarStep READ get_PolarStep WRITE set_PolarStep)
  QCEP_DOUBLE_PROPERTY(PolarStep)

  Q_PROPERTY(int polarNSteps READ get_PolarNSteps WRITE set_PolarNSteps)
  QCEP_INTEGER_PROPERTY(PolarNSteps)

  Q_PROPERTY(double polarStart READ get_PolarStart WRITE set_PolarStart)
  QCEP_DOUBLE_PROPERTY(PolarStart)

  Q_PROPERTY(double polarEnd READ get_PolarEnd WRITE set_PolarEnd)
  QCEP_DOUBLE_PROPERTY(PolarEnd)

  Q_PROPERTY(int radialUnits READ get_RadialUnits WRITE set_RadialUnits)
  QCEP_INTEGER_PROPERTY(RadialUnits)

  Q_PROPERTY(double radialStep READ get_RadialStep WRITE set_RadialStep)
  QCEP_DOUBLE_PROPERTY(RadialStep)

  Q_PROPERTY(int radialNSteps READ get_RadialNSteps WRITE set_RadialNSteps)
  QCEP_INTEGER_PROPERTY(RadialNSteps)

  Q_PROPERTY(double radialStart READ get_RadialStart WRITE set_RadialStart)
  QCEP_DOUBLE_PROPERTY(RadialStart)

  Q_PROPERTY(double radialEnd READ get_RadialEnd WRITE set_RadialEnd)
  QCEP_DOUBLE_PROPERTY(RadialEnd)

private:
  mutable QMutex        m_Mutex;
  QcepSettingsSaverWPtr m_Saver;
  QxrdExperimentWPtr    m_Experiment;
};

#endif // QXRDPOLARTRANSFORM_H
