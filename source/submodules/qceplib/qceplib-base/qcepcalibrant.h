#ifndef QCEPCALIBRANT_H
#define QCEPCALIBRANT_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qcepcalibrant-ptr.h"
#include "qcepcalibrantdspacing.h"
#include "qcepcalibrantdspacings.h"
#include <qscriptengine.h>

class QCEP_EXPORT QcepCalibrant : public QcepObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepCalibrant(QString name);
  ~QcepCalibrant();

public slots:
  QcepCalibrantDSpacing dSpacing(int h, int k, int l);
  QcepCalibrantDSpacings dSpacings(double energy);

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepCalibrantWPtr &cal);
  static void fromScriptValue(const QScriptValue &obj, QcepCalibrantWPtr &cal);
  bool isLocked();

  typedef enum  {
//    Triclinic,
//    SimpleMonoclinic,
//    BaseCenteredMonoclinic,
//    SimpleOrthorhombic,
//    BaseCenteredOrthorhombic,
//    BodyCenteredOrthorhombic,
//    FaceCenteredOrthorhombic,
//    Rhombohedral,
//    SimpleTetragonal,
//    BodyCenterdTetragonal,
    Hexagonal,
    RHexagonal,
    SimpleCubic,
    BodyCenteredCubic,
    FaceCenteredCubic,
    DiamondCubic
  } Bravais;

private:
  QcepCalibrantDSpacings dSpacingsCubic(double energy);
  QcepCalibrantDSpacings dSpacingsHexagonal(double energy);

public:
  Q_PROPERTY(int isUsed READ get_IsUsed WRITE set_IsUsed)
  QCEP_INTEGER_PROPERTY(IsUsed)

  Q_PROPERTY(int index READ get_Index WRITE set_Index STORED false)
  QCEP_INTEGER_PROPERTY(Index)

  Q_PROPERTY(int flags READ get_Flags WRITE set_Flags)
  QCEP_INTEGER_PROPERTY(Flags)

  Q_PROPERTY(QString description READ get_Description WRITE set_Description)
  QCEP_STRING_PROPERTY(Description)

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
};

Q_DECLARE_METATYPE(QcepCalibrantWPtr)

#endif // QCEPCALIBRANT_H
