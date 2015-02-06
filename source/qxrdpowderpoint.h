#ifndef QXRDPOWDERPOINT_H
#define QXRDPOWDERPOINT_H

#include <QSettings>
#include <QString>
#include <QScriptable>
#include <QVector>

class QxrdPowderPoint
{
public:
  QxrdPowderPoint(int n1, int n2, int n3, double x, double y, double r1, double r2, double az);
  QxrdPowderPoint(const QxrdPowderPoint& pt);
  QxrdPowderPoint();

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

  bool operator == (const QxrdPowderPoint &pt) const;
  bool operator != (const QxrdPowderPoint &pt) const;

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  bool isValid() const;

  QString toString() const;

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

class QxrdPowderPointVector : public QVector<QxrdPowderPoint>
{
public:

  void setSettingsValue(QSettings *settings, QString name);
  static void customSaver(const QVariant &val, QSettings *settings, QString name);

  QString toString() const;
};

Q_DECLARE_METATYPE(QxrdPowderPoint)
Q_DECLARE_METATYPE(QxrdPowderPointVector)

#endif // QXRDPOWDERPOINT_H
