#ifndef QCEPCALIBRANTDSPACING_H
#define QCEPCALIBRANTDSPACING_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include <QSettings>
#include <QScriptValue>
#include <QScriptEngine>
#include "qcepproperty.h"

class QCEP_EXPORT QcepCalibrantDSpacing
{
public:
  QcepCalibrantDSpacing(int calIndex, int h, int k, int l, int n, double d, double tth);
  QcepCalibrantDSpacing(const QcepCalibrantDSpacing& spc);
  QcepCalibrantDSpacing();

public:
  int     index() const { return m_Index; }
  int     h() const { return m_H; }
  int     k() const { return m_K; }
  int     l() const { return m_L; }
  int     n() const { return m_N; }
  double  d() const { return m_D; }
  double  tth() const { return m_TTH; }

  int&    index() { return m_Index; }
  int&    h() { return m_H; }
  int&    k() { return m_K; }
  int&    l() { return m_L; }
  int&    n() { return m_N; }
  double& d() { return m_D; }
  double& tth() { return m_TTH; }

  bool operator == ( const QcepCalibrantDSpacing &spc) const;
  bool operator != ( const QcepCalibrantDSpacing &spc) const;

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  bool isValid() const;

  QString toString() const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepCalibrantDSpacing &spc);
  static void fromScriptValue(const QScriptValue &obj, QcepCalibrantDSpacing &spc);

private:
  int m_Index;
  int m_H;
  int m_K;
  int m_L;
  int m_N;
  double m_D;
  double m_TTH;
};

Q_DECLARE_METATYPE(QcepCalibrantDSpacing)

#ifndef QT_NO_DATASTREAM

extern QCEP_EXPORT QDataStream &operator<<(QDataStream &stream, const QcepCalibrantDSpacing &pt);
extern QCEP_EXPORT QDataStream &operator>>(QDataStream &stream, QcepCalibrantDSpacing &pt);

#endif

#endif // QCEPCALIBRANTDSPACING_H
