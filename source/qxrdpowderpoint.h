#ifndef QXRDPOWDERPOINT_H
#define QXRDPOWDERPOINT_H

#include <QSettings>
#include <QString>
#include <QScriptable>

class QxrdPowderPoint
{
public:
  QxrdPowderPoint(int n1, int n2, double x, double y);
  QxrdPowderPoint();

public:
  int n1() const { return m_N1; }
  int n2() const { return m_N2; }
  double x() const { return m_X; }
  double y() const { return m_Y; }

  int& n1() { return m_N1; }
  int& n2() { return m_N2; }
  double& x() { return m_X; }
  double& y() { return m_Y; }

protected:
  int m_N1;
  int m_N2;
  double m_X;
  double m_Y;
};

#endif // QXRDPOWDERPOINT_H
