#include "qcepdebug.h"
#include "qxrdpowderpoint.h"
#include <QString>
#include <QScriptEngine>
#include "qcepproperty.h"

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

void QxrdPowderPoint::registerMetaTypes()
{
  qRegisterMetaType< QxrdPowderPoint >("QxrdPowderPoint");

  qRegisterMetaTypeStreamOperators< QxrdPowderPoint >("QxrdPowderPoint");

  QcepProperty::registerCustomSaver("QxrdPowderPoint", QxrdPowderPoint::customSaver);
}

QScriptValue QxrdPowderPoint::toScriptValue(QScriptEngine *engine, const QxrdPowderPoint &pt)
{
  QScriptValue obj = engine->newObject();

  obj.setProperty("n1", pt.n1());
  obj.setProperty("n2", pt.n2());
  obj.setProperty("n3", pt.n3());
  obj.setProperty("x",  pt.x());
  obj.setProperty("y",  pt.y());
  obj.setProperty("r1", pt.r1());
  obj.setProperty("r2", pt.r2());
  obj.setProperty("az", pt.az());

  return obj;
}

void QxrdPowderPoint::fromScriptValue(const QScriptValue &obj, QxrdPowderPoint &pt)
{
  pt.n1() = obj.property("n1").toInteger();
  pt.n2() = obj.property("n2").toInteger();
  pt.n3() = obj.property("n3").toInteger();
  pt.x()  = obj.property("x").toNumber();
  pt.y()  = obj.property("y").toNumber();
  pt.r1() = obj.property("r1").toNumber();
  pt.r2() = obj.property("r2").toNumber();
  pt.az() = obj.property("az").toNumber();
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

void QxrdPowderPointVector::registerMetaTypes()
{
  qRegisterMetaType< QxrdPowderPointVector >("QxrdPowderPointVector");

  qRegisterMetaTypeStreamOperators< QxrdPowderPointVector >("QxrdPowderPointVector");

  QcepProperty::registerCustomSaver("QxrdPowderPointVector", QxrdPowderPointVector::customSaver);
}

QScriptValue QxrdPowderPointVector::toScriptValue(QScriptEngine *engine,
                                                          const QxrdPowderPointVector &vec)
{
  int n = vec.count();

  QScriptValue res = engine->newArray(n);

  for (int i=0; i<n; i++) {
    const QxrdPowderPoint &pt = vec.at(i);
    QScriptValue obj = engine->newObject();

    obj.setProperty("n1", pt.n1());
    obj.setProperty("n2", pt.n2());
    obj.setProperty("n3", pt.n3());
    obj.setProperty("x", pt.x());
    obj.setProperty("y", pt.y());
    obj.setProperty("r1", pt.r1());
    obj.setProperty("r2", pt.r2());
    obj.setProperty("az", pt.az());

    res.setProperty(i, obj);
  }

  return res;
}

void QxrdPowderPointVector::fromScriptValue(const QScriptValue &obj,
                                                    QxrdPowderPointVector &vec)
{
  int n=obj.property("length").toInteger();

  vec.resize(n);

  for (int i=0; i<n; i++) {
    QScriptValue pt = obj.property(i);

    if (pt.isValid()) {
      vec[i].n1() = pt.property("n1").toInteger();
      vec[i].n2() = pt.property("n2").toInteger();
      vec[i].n3() = pt.property("n3").toInteger();
      vec[i].x()  = pt.property("x").toNumber();
      vec[i].y()  = pt.property("y").toNumber();
      vec[i].r1() = pt.property("r1").toNumber();
      vec[i].r2() = pt.property("r2").toNumber();
      vec[i].az() = pt.property("az").toNumber();
    }
  }
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const QxrdPowderPoint &pt)
{
  stream << pt.n1() << pt.n2() << pt.n3() << pt.x() << pt.y() << pt.r1() << pt.r2() << pt.az();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, QxrdPowderPoint &pt)
{
  stream >> pt.n1() >> pt.n2() >> pt.n3() >> pt.x() >> pt.y() >> pt.r1() >> pt.r2() >> pt.az();

  return stream;
}

#endif
