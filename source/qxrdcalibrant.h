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

class QxrdCalibrantDSpacing;
class QxrdCalibrantDSpacingVector;

class QxrdCalibrant : public QcepObject
{
  Q_OBJECT

public:
  QxrdCalibrant(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCalibrantLibraryWPtr lib);
  ~QxrdCalibrant();

public slots:
  QxrdCalibrantDSpacing dSpacing(int h, int k, int l);
  QxrdCalibrantDSpacingVector dSpacings(double energy);

public:
  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdCalibrantWPtr &cal);
  static void fromScriptValue(const QScriptValue &obj, QxrdCalibrantWPtr &cal);

  typedef enum  {
    Triclinic,
    SimpleMonoclinic,
    BaseCenteredMonoclinic,
    SimpleOrthorhombic,
    BaseCenteredOrthorhombic,
    BodyCenteredOrthorhombic,
    FaceCenteredOrthorhombic,
    Rhombohedral,
    SimpleTetragonal,
    BodyCenterdTetragonal,
    Hexagonal,
    SimpleCubic,
    BodyCenteredCubic,
    FaceCenteredCubic,
    DiamondCubic
  } Bravais;

private:
  QxrdCalibrantDSpacingVector dSpacingsCubic(double energy);

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

Q_DECLARE_METATYPE(QxrdCalibrantWPtr)

class QxrdCalibrantDSpacing
{
public:
  QxrdCalibrantDSpacing(int h, int k, int l, double d, double tth);
  QxrdCalibrantDSpacing(const QxrdCalibrantDSpacing& spc);
  QxrdCalibrantDSpacing();

public:
  int     h() const { return m_H; }
  int     k() const { return m_K; }
  int     l() const { return m_L; }
  double  d() const { return m_D; }
  double  tth() const { return m_TTH; }

  int&    h() { return m_H; }
  int&    k() { return m_K; }
  int&    l() { return m_L; }
  double& d() { return m_D; }
  double& tth() { return m_TTH; }

  bool operator == ( const QxrdCalibrantDSpacing &spc) const;
  bool operator != ( const QxrdCalibrantDSpacing &spc) const;

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  bool isValid() const;

  QString toString() const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdCalibrantDSpacing &spc);
  static void fromScriptValue(const QScriptValue &obj, QxrdCalibrantDSpacing &spc);

private:
  int m_H;
  int m_K;
  int m_L;
  double m_D;
  double m_TTH;
};

Q_DECLARE_METATYPE(QxrdCalibrantDSpacing)

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const QxrdCalibrantDSpacing &pt);
extern QDataStream &operator>>(QDataStream &stream, QxrdCalibrantDSpacing &pt);

#endif

class QxrdCalibrantDSpacingVector : public QVector<QxrdCalibrantDSpacing>
{
public:
  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString() const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdCalibrantDSpacingVector &vec);
  static void fromScriptValue(const QScriptValue &obj, QxrdCalibrantDSpacingVector &vec);
};

Q_DECLARE_METATYPE(QxrdCalibrantDSpacingVector)

#endif // QXRDCALIBRANT_H
