#include "qxrdpowderpoint.h"
#include <QString>

QxrdPowderPoint::QxrdPowderPoint(int n1, int n2, double x, double y) :
  m_N1(n1),
  m_N2(n2),
  m_X(x),
  m_Y(y)
{
}

QxrdPowderPoint::QxrdPowderPoint(const QxrdPowderPoint &pt) :
  m_N1(pt.n1()),
  m_N2(pt.n2()),
  m_X(pt.x()),
  m_Y(pt.y())
{
}

QxrdPowderPoint::QxrdPowderPoint() :
  m_N1(-1),
  m_N2(-1),
  m_X(0.0),
  m_Y(0.0)
{
}

bool QxrdPowderPoint::isValid() const
{
  return (m_N1 >= 0 && m_N2 >= 0);
}

bool QxrdPowderPoint::operator == (const QxrdPowderPoint &pt) const
{
  return n1()==pt.n1() && n2()==pt.n2() && x()==pt.x() && y()==pt.y();
}

bool QxrdPowderPoint::operator != (const QxrdPowderPoint &pt) const
{
  return n1()!=pt.n1() || n2()!=pt.n2() || x()!=pt.x() || y()!=pt.y();
}

void QxrdPowderPoint::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  settings->setValue("n1", n1());
  settings->setValue("n2", n2());
  settings->setValue("x", x());
  settings->setValue("y", y());

  settings->endGroup();
}

void QxrdPowderPoint::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdPowderPoint pt = val.value<QxrdPowderPoint>();

  pt.setSettingsValue(settings, name);
}

QString QxrdPowderPoint::toString() const
{
  return QObject::tr("[%1, %2, %3, %4]").arg(n1()).arg(n2()).arg(x()).arg(y());
}

void QxrdPowderPointVector::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginWriteArray(name, count());

  for (int i=0; i<count(); i++) {
    settings->setArrayIndex(i);
    const QxrdPowderPoint &pt = at(i);
    settings->setValue("n1", pt.n1());
    settings->setValue("n2", pt.n2());
    settings->setValue("x", pt.x());
    settings->setValue("y", pt.y());
  }

  settings->endArray();
}

void QxrdPowderPointVector::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdPowderPointVector vec = val.value<QxrdPowderPointVector>();

  vec.setSettingsValue(settings, name);
}

QString QxrdPowderPointVector::toString() const
{
  QString res="[";

  for (int i=0; i<count(); i++) {
    if (i != 0) {
      res += ", ";
    }

    res += at(i).toString();
  }

  res += "]";

  return res;
}
