#include "qcepcalibrantdspacing.h"

QcepCalibrantDSpacing::QcepCalibrantDSpacing(int calIndex, int h, int k, int l, int n, double d, double tth) :
  m_Index(calIndex),
  m_H(h),
  m_K(k),
  m_L(l),
  m_N(n),
  m_D(d),
  m_TTH(tth)
{
}

QcepCalibrantDSpacing::QcepCalibrantDSpacing(const QcepCalibrantDSpacing &spc) :
  m_Index(spc.index()),
  m_H(spc.h()),
  m_K(spc.k()),
  m_L(spc.l()),
  m_N(spc.n()),
  m_D(spc.d()),
  m_TTH(spc.tth())
{
}

QcepCalibrantDSpacing::QcepCalibrantDSpacing() :
  m_Index(-1),
  m_H(0),
  m_K(0),
  m_L(0),
  m_N(0),
  m_D(0),
  m_TTH(0)
{
}

bool QcepCalibrantDSpacing::isValid() const
{
  return (m_Index >= 0) && (m_H != 0 || m_K != 0 || m_L != 0) && (m_N >= 0) && (m_D > 0.0) && (m_TTH > 0.0);
}

bool QcepCalibrantDSpacing::operator == ( const QcepCalibrantDSpacing &spc) const
{
  return index()==spc.index() && h()==spc.h() && k()==spc.k() && l()==spc.l() && d()==spc.d() && tth()==spc.tth();
}

bool QcepCalibrantDSpacing::operator != ( const QcepCalibrantDSpacing &spc) const
{
  return index()!=spc.index() || h()!=spc.h() || k()!=spc.k() || l()!=spc.l() || d()!=spc.d() || tth()!=spc.tth();
}

void QcepCalibrantDSpacing::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);
  settings->setValue("index", index());
  settings->setValue("h", h());
  settings->setValue("k", k());
  settings->setValue("l", l());
  settings->setValue("n", n());
  settings->setValue("d", d());
  settings->setValue("tth", tth());
  settings->endGroup();
}

void QcepCalibrantDSpacing::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QcepCalibrantDSpacing spc = val.value<QcepCalibrantDSpacing>();

  spc.setSettingsValue(settings, name);
}


QString QcepCalibrantDSpacing::toString() const
{
  return QObject::tr("{index:%1, h:%2, k:%3, l:%4, n:%5, d:%6, tth:%7}")
      .arg(index()).arg(h()).arg(k()).arg(l()).arg(n()).arg(d()).arg(tth());
}

void QcepCalibrantDSpacing::registerMetaTypes()
{
  qRegisterMetaType<QcepCalibrantDSpacing>("QcepCalibrantDSpacing");

  qRegisterMetaTypeStreamOperators<QcepCalibrantDSpacing>("QcepCalibrantDSpacing");

  QcepProperty::registerCustomSaver("QcepCalibrantDSpacing", QcepCalibrantDSpacing::customSaver);
}

QScriptValue QcepCalibrantDSpacing::toScriptValue(QScriptEngine *engine, const QcepCalibrantDSpacing &spc)
{
  QScriptValue obj = engine->newObject();

  obj.setProperty("index", spc.index());
  obj.setProperty("h", spc.h());
  obj.setProperty("k", spc.k());
  obj.setProperty("l", spc.l());
  obj.setProperty("n", spc.n());
  obj.setProperty("d", spc.d());
  obj.setProperty("tth", spc.tth());

  return obj;
}

void QcepCalibrantDSpacing::fromScriptValue(const QScriptValue &obj, QcepCalibrantDSpacing &spc)
{
  spc.index() = obj.property("index").toInteger();
  spc.h() = obj.property("h").toInteger();
  spc.k() = obj.property("k").toInteger();
  spc.l() = obj.property("l").toInteger();
  spc.n() = obj.property("n").toInteger();
  spc.d() = obj.property("d").toNumber();
  spc.tth() = obj.property("tth").toNumber();
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const QcepCalibrantDSpacing &pt)
{
  stream << pt.index() << pt.h() << pt.k() << pt.l() << pt.n() << pt.d() << pt.tth();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, QcepCalibrantDSpacing &pt)
{
  stream >> pt.index() >> pt.h() >> pt.k() >> pt.l() >> pt.n() >> pt.d() >> pt.tth();

  return stream;
}

#endif

