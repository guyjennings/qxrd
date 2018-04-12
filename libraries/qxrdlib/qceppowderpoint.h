#ifndef QCEPPOWDERPOINT_H
#define QCEPPOWDERPOINT_H

#include "qceplib_global.h"
#include <QSettings>
#include <QString>
#include <QScriptable>
#include <QVector>

class QCEP_EXPORT QcepPowderPoint
{
public:
  QcepPowderPoint(int n1, int n2, int n3, double x, double y, double r1, double r2, double az);
  QcepPowderPoint(const QcepPowderPoint& pt);
  QcepPowderPoint();

public:
  int n1() const { return m_N1; }
  int n2() const { return m_N2; }
  int n3() const { return m_N3; }
  double x() const { return m_X; }
  double y() const { return m_Y; }
  double r1() const { return m_R1; }
  double r2() const { return m_R2; }
  double az() const { return m_Az; }

  int& n1() { return m_N1; }
  int& n2() { return m_N2; }
  int& n3() { return m_N3; }
  double& x() { return m_X; }
  double& y() { return m_Y; }
  double& r1() { return m_R1; }
  double& r2() { return m_R2; }
  double& az() { return m_Az; }

  bool operator == (const QcepPowderPoint &pt) const;
  bool operator != (const QcepPowderPoint &pt) const;

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  bool isValid() const;

  QString toString() const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepPowderPoint &pt);
  static void fromScriptValue(const QScriptValue &obj, QcepPowderPoint &pt);

protected:
  int m_N1;
  int m_N2;
  int m_N3;
  double m_X;
  double m_Y;
  double m_R1;
  double m_R2;
  double m_Az;
};

Q_DECLARE_METATYPE(QcepPowderPoint)

#ifndef QT_NO_DATASTREAM

extern QCEP_EXPORT QDataStream &operator<<(QDataStream &stream, const QcepPowderPoint &pt);
extern QCEP_EXPORT QDataStream &operator>>(QDataStream &stream, QcepPowderPoint &pt);

#endif

class QCEP_EXPORT QcepPowderPointVector : public QVector<QcepPowderPoint>
{
public:

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString() const;

  static void registerMetaTypes();
  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepPowderPointVector &vec);
  static void fromScriptValue(const QScriptValue &obj, QcepPowderPointVector &vec);
};

Q_DECLARE_METATYPE(QcepPowderPointVector)

#endif // QCEPPOWDERPOINT_H
