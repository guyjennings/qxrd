#include "qxrdpowderpoint.h"
#include <QString>

QxrdPowderPoint::QxrdPowderPoint(int n1, int n2, int n3, double x, double y, double r1, double r2, double az) :
  m_N1(n1),
  m_N2(n2),
  m_N3(n3),
  m_X(x),
  m_Y(y),
  m_R1(r1),
  m_R2(r2),
  m_Az(az)
{
}

QxrdPowderPoint::QxrdPowderPoint(const QxrdPowderPoint &pt) :
  m_N1(pt.n1()),
  m_N2(pt.n2()),
  m_N3(pt.n3()),
  m_X(pt.x()),
  m_Y(pt.y()),
  m_R1(pt.r1()),
  m_R2(pt.r2()),
  m_Az(pt.az())
{
}

QxrdPowderPoint::QxrdPowderPoint() :
  m_N1(-1),
  m_N2(-1),
  m_N3(-1),
  m_X(0.0),
  m_Y(0.0),
  m_R1(0.0),
  m_R2(0.0),
  m_Az(0.0)
{
}

bool QxrdPowderPoint::isValid() const
{
  return (m_N1 >= 0 && m_N2 >= 0 && m_N3 >= 0);
}

bool QxrdPowderPoint::operator == (const QxrdPowderPoint &pt) const
{
  return n1()==pt.n1() && n2()==pt.n2() && n3()==pt.n3() && x()==pt.x() && y()==pt.y() && r1()==pt.r1() && r2() == pt.r2() && az()==pt.az();
}

bool QxrdPowderPoint::operator != (const QxrdPowderPoint &pt) const
{
  return n1()!=pt.n1() || n2()!=pt.n2() || n3()!=pt.n3() || x()!=pt.x() || y()!=pt.y() || r1()!=pt.r1() || r2() != pt.r2() || az()!=pt.az();
}

void QxrdPowderPoint::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);

  settings->setValue("n1", n1());
  settings->setValue("n2", n2());
  settings->setValue("n3", n3());
  settings->setValue("x", x());
  settings->setValue("y", y());
  settings->setValue("r1", r1());
  settings->setValue("r2", r2());
  settings->setValue("az", az());

  settings->endGroup();
}

void QxrdPowderPoint::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdPowderPoint pt = val.value<QxrdPowderPoint>();

  pt.setSettingsValue(settings, name);
}

QString QxrdPowderPoint::toString() const
{
  return QObject::tr("[%1, %2, %3, %4, %5, %6, %7, %8]").arg(n1()).arg(n2()).arg(n3()).arg(x()).arg(y()).arg(r1()).arg(r2()).arg(az());
}

void QxrdPowderPointVector::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginWriteArray(name, count());

  for (int i=0; i<count(); i++) {
    settings->setArrayIndex(i);
    const QxrdPowderPoint &pt = at(i);
    settings->setValue("n1", pt.n1());
    settings->setValue("n2", pt.n2());
    settings->setValue("n3", pt.n3());
    settings->setValue("x", pt.x());
    settings->setValue("y", pt.y());
    settings->setValue("r1", pt.r1());
    settings->setValue("r2", pt.r2());
    settings->setValue("az", pt.az());
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
      res += ", \n";
    }

    res += at(i).toString();
  }

  res += "]";

  return res;
}
