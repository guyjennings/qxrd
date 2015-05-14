#ifndef QXRDCALIBRANT_H
#define QXRDCALIBRANT_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qxrdcalibrantlibrary-ptr.h"
#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qxrdcalibrant-ptr.h"
#include <qscriptengine.h>

Q_DECLARE_METATYPE(QxrdCalibrantWPtr)

class QxrdCalibrant : public QcepObject
{
  Q_OBJECT

public:
  QxrdCalibrant(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCalibrantLibraryWPtr lib);
  ~QxrdCalibrant();

public slots:

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdCalibrantWPtr &cal);
  static void fromScriptValue(const QScriptValue &obj, QxrdCalibrantWPtr &cal);

public:
  Q_PROPERTY(QString description READ get_Description WRITE set_Description)
  QCEP_STRING_PROPERTY(Description)

  Q_PROPERTY(int flags READ get_Flags WRITE set_Flags)
  QCEP_INTEGER_PROPERTY(Flags)

  Q_PROPERTY(int symmetry READ get_Symmetry WRITE set_Symmetry)
  QCEP_INTEGER_PROPERTY(Symmetry)

  Q_PROPERTY(double a READ get_A WRITE set_A)
  QCEP_DOUBLE_PROPERTY(A)

  Q_PROPERTY(double b READ get_B WRITE set_B)
  QCEP_DOUBLE_PROPERTY(B)

  Q_PROPERTY(double c READ get_C WRITE set_C)
  QCEP_DOUBLE_PROPERTY(C)

  Q_PROPERTY(double alpha READ get_Alpha WRITE set_Alpha)
  QCEP_DOUBLE_PROPERTY(Alpha)

  Q_PROPERTY(double beta READ get_Beta WRITE set_Beta)
  QCEP_DOUBLE_PROPERTY(Beta)

  Q_PROPERTY(double gamma READ get_Gamma WRITE set_Gamma)
  QCEP_DOUBLE_PROPERTY(Gamma)

private:
  mutable QMutex            m_Mutex;
  QxrdExperimentWPtr        m_Experiment;
  QxrdCalibrantLibraryWPtr  m_CalibrantLibrary;
};

#endif // QXRDCALIBRANT_H
