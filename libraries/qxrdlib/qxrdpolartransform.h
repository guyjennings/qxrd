#ifndef QXRDPOLARTRANSFORM_H
#define QXRDPOLARTRANSFORM_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepobject.h"
#include "qcepproperty.h"

#include "qxrdexperiment-ptr.h"
#include "qxrdcenterfinder-ptr.h"
#include "qxrdintegrator-ptr.h"
#include "qxrdintegratorcache-ptr.h"
#include "qcepdataobject-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdpolartransform-ptr.h"

class QXRD_EXPORT QxrdPolarTransform : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdPolarTransform(QString name);
  virtual ~QxrdPolarTransform();
  void initialize(QObjectWPtr parent);

  QxrdExperimentWPtr experiment() const;
  QxrdIntegratorWPtr integrator() const;

//  void execute();

  QcepDataObjectPtr transform(QcepDoubleImageDataPtr img, QcepMaskDataPtr mask);

public:
//  Q_PROPERTY(QString destination READ get_Destination WRITE set_Destination)
//  QCEP_STRING_PROPERTY(Destination)

//  Q_PROPERTY(int outputType READ get_OutputType WRITE set_OutputType)
//  QCEP_INTEGER_PROPERTY(OutputType)

  Q_PROPERTY(int oversample READ get_Oversample WRITE set_Oversample)
  QCEP_INTEGER_PROPERTY(Oversample)

  Q_PROPERTY(bool enableGeometricCorrections READ get_EnableGeometricCorrections WRITE set_EnableGeometricCorrections)
  QCEP_BOOLEAN_PROPERTY(EnableGeometricCorrections)

  Q_PROPERTY(bool enablePolarizationCorrections READ get_EnablePolarizationCorrections WRITE set_EnablePolarizationCorrections)
  QCEP_BOOLEAN_PROPERTY(EnablePolarizationCorrections)

  Q_PROPERTY(double polarization READ get_Polarization WRITE set_Polarization)
  QCEP_DOUBLE_PROPERTY(Polarization)

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
  mutable QMutex         m_Mutex;
  QxrdIntegratorCachePtr m_IntegratorCache;
};

Q_DECLARE_METATYPE(QxrdPolarTransform*)

#endif // QXRDPOLARTRANSFORM_H
