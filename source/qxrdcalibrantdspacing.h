#ifndef QXRDCALIBRANTDSPACING_H
#define QXRDCALIBRANTDSPACING_H

#include "qcepmacros.h"
#include <QSettings>
#include <QScriptValue>
#include <QScriptEngine>
#include "qcepproperty.h"

class QxrdCalibrantDSpacing
{
public:
  QxrdCalibrantDSpacing(int calIndex, int h, int k, int l, int n, double d, double tth);
  QxrdCalibrantDSpacing(const QxrdCalibrantDSpacing& spc);
  QxrdCalibrantDSpacing();

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
  int m_Index;
  int m_H;
  int m_K;
  int m_L;
  int m_N;
  double m_D;
  double m_TTH;
};

Q_DECLARE_METATYPE(QxrdCalibrantDSpacing)

#ifndef QT_NO_DATASTREAM

extern QDataStream &operator<<(QDataStream &stream, const QxrdCalibrantDSpacing &pt);
extern QDataStream &operator>>(QDataStream &stream, QxrdCalibrantDSpacing &pt);

#endif

#endif // QXRDCALIBRANTDSPACING_H
