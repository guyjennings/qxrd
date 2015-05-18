#include "qxrdcalibrant.h"
#include "qxrdcalibrantlibrary.h"
#include "qxrdsettingssaver.h"
#include "qxrdexperiment.h"
#include <QScriptEngine>
#include <math.h>
#include "qxrddebug.h"

QxrdCalibrant::QxrdCalibrant(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr exp, QxrdCalibrantLibraryWPtr lib)
  : QcepObject("calibrant", NULL),
    m_Description(saver, this, "description", "Description", "Calibrant Description"),
    m_Flags(saver, this, "flags", 0, "Calibrant flags"),
    m_Symmetry(saver, this, "symmetry", 0, "Calibrant Symmetry"),
    m_A(saver, this, "a", 1, "calibrant a-axis"),
    m_B(saver, this, "b", 1, "calibrant b-axis"),
    m_C(saver, this, "c", 1, "calibrant c-axis"),
    m_Alpha(saver, this, "alpha", 90, "calibrant alpha angle (deg)"),
    m_Beta(saver, this, "beta", 90, "calibrant beta angle (deg)"),
    m_Gamma(saver, this, "gamma", 90, "calibrant gamma angle (deg)"),
    m_Experiment(exp),
    m_CalibrantLibrary(lib)
{

}

QxrdCalibrant::~QxrdCalibrant()
{

}

bool QxrdCalibrant::isLocked()
{
  return (get_Flags() && 1) == 0;
}

QScriptValue QxrdCalibrant::toScriptValue(QScriptEngine *engine, const QxrdCalibrantWPtr &cal)
{
  return engine->newQObject(cal.data());
}

void QxrdCalibrant::fromScriptValue(const QScriptValue &obj, QxrdCalibrantWPtr &cal)
{
//  QxrdExperimentPtr exp(m_Experiment);

//  if (exp) {
//    exp->printMessage("QxrdCalibrant::fromScriptValue not yet written")
//  }
}

QxrdCalibrantDSpacing QxrdCalibrant::dSpacing(int h, int k, int l)
{
  return QxrdCalibrantDSpacing(h,k,l, sqrt(h*h + k*k + l*l), 0);
}

class QxrdCalibrantQuadInt {
public:
  QxrdCalibrantQuadInt(int n=0, int h=-1, int k=-1, int l=-1) : m_N(n), m_H(h), m_K(k), m_L(l) {}

  int & n() { return m_N; }
  int & h() { return m_H; }
  int & k() { return m_K; }
  int & l() { return m_L; }

private:
  int m_N;
  int m_H;
  int m_K;
  int m_L;
};

QxrdCalibrantDSpacingVector QxrdCalibrant::dSpacings(double energy)
{
  int s = get_Symmetry();

  switch (s) {

  case SimpleCubic:
  case BodyCenteredCubic:
  case FaceCenteredCubic:
  case DiamondCubic:
    return dSpacingsCubic(energy);

  case Hexagonal:
  case RHexagonal:
    return dSpacingsHexagonal(energy);

  default:
    return QxrdCalibrantDSpacingVector();
  }
}

QxrdCalibrantDSpacingVector QxrdCalibrant::dSpacingsCubic(double energy)
{
  int    s = get_Symmetry();
  double a = get_A();
  double lambda = (energy>100 ? 12398.4187/energy : energy);

  int mmax = 2.0*a/lambda + 1;

//  if (qcepDebug(DEBUG_CALIBRANT)) {
//    printMessage(tr("mmax = %1").arg(mmax));
//  }

  QVector<QxrdCalibrantQuadInt> ex(mmax*mmax);

  for (int h=1; h<=mmax; h++) {
    for (int k=0; k<=h; k++) {
      for (int l=0; l<=k; l++) {
        int r = h*h+k*k+l*l;

        if (r < mmax*mmax) {
          bool ok=false;

          switch (s) {
          case SimpleCubic:  // Simple cubic - all OK
            ok = true;
            break;

          case BodyCenteredCubic:  // BCC h+k+l even
            ok = ((h + k + l) % 2) == 0;
            break;

          case FaceCenteredCubic:  // FCC h,k,l all even or all odd
            {
              int n = h%2 + k%2 + l%2;
              ok = (n==0) || (n==3);
            }
            break;

          case DiamondCubic:
            {
              int nn = h%2 + k%2 + l%2;

              if (nn==0 || nn==3) { // If nn==0 - all even, nn==3 - all odd
                int n = (h + k + l);

                if (n%2 == 0) {
                  ok = (n%4 == 0);
                } else {
                  ok = true;
                }
              }
            }
            break;
          }

          if (ok) {
            if (ex[r].n() == 0) {
              ex[r] = QxrdCalibrantQuadInt(1, h,k,l);
            } else {
              ex[r].n()++;
            }
          }
        }
      }
    }
  }

  QxrdCalibrantDSpacingVector pts;

  for (int i=1; i<mmax*mmax; i++) {
    QxrdCalibrantQuadInt e = ex[i];
    if (e.n() > 0) {
      double d = a/sqrt(i);
      double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

      if (tth <= 180) {
        pts.append(QxrdCalibrantDSpacing(e.h(), e.k(), e.l(), d, tth));

        if (qcepDebug(DEBUG_CALIBRANT)) {
          printMessage(tr("%1(%2): [%3,%4,%5], d:%6, tth:%7").arg(i).arg(e.n()).arg(e.h()).arg(e.k()).arg(e.l()).arg(d).arg(tth));
        }
      }
    }
  }

  return pts;
}

bool lessThan(const QxrdCalibrantDSpacing &d1, const QxrdCalibrantDSpacing&d2)
{
  return d1.tth() < d2.tth();
}

QxrdCalibrantDSpacingVector QxrdCalibrant::dSpacingsHexagonal(double energy)
{
  int    s = get_Symmetry();
  double a = get_A();
  double c = get_C();

  double lambda = (energy>100 ? 12398.4187/energy : energy);

  int hkmax = 2.0*a/lambda + 1;
  int lmax  = 2.0*c/lambda + 1;

//  if (qcepDebug(DEBUG_CALIBRANT)) {
//    printMessage(tr("mmax = %1").arg(mmax));
//  }

  QxrdCalibrantDSpacingVector pts;

  for (int h=0; h<=hkmax; h++) {
    for (int k=0; k<=hkmax; k++) {
      for (int l=0; l<=lmax; l++) {
        int ok = false;

        switch (s) {
        case RHexagonal:
          ok = ((-h+k+l)%3 != 0);
          break;
        case Hexagonal:
          ok = true;
        }

        if (ok) {
          double d = 1.0/sqrt((4.0/3.0)*(h*h + h*k + k*k)/(a*a) + (l*l)/(c*c));
          double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

          if (tth < 180) {
            pts.insertUnique(h,k,l,d,tth);
          }
        }
      }
    }
  }

  qSort(pts.begin(), pts.end(), lessThan);

  return pts;
}

QxrdCalibrantDSpacing::QxrdCalibrantDSpacing(int h, int k, int l, double d, double tth) :
  m_H(h),
  m_K(k),
  m_L(l),
  m_D(d),
  m_TTH(tth)
{
}

QxrdCalibrantDSpacing::QxrdCalibrantDSpacing(const QxrdCalibrantDSpacing &spc) :
  m_H(spc.h()),
  m_K(spc.k()),
  m_L(spc.l()),
  m_D(spc.d()),
  m_TTH(spc.tth())
{
}

QxrdCalibrantDSpacing::QxrdCalibrantDSpacing() :
  m_H(0),
  m_K(0),
  m_L(0),
  m_D(0),
  m_TTH(0)
{
}

bool QxrdCalibrantDSpacing::isValid() const
{
  return (m_H != 0 || m_K != 0 || m_L != 0) && (m_D > 0.0) && (m_TTH > 0.0);
}

bool QxrdCalibrantDSpacing::operator == ( const QxrdCalibrantDSpacing &spc) const
{
  return h()==spc.h() && k()==spc.k() && l()==spc.l() && d()==spc.d() && tth()==spc.tth();
}

bool QxrdCalibrantDSpacing::operator != ( const QxrdCalibrantDSpacing &spc) const
{
  return h()!=spc.h() || k()!=spc.k() || l()!=spc.l() || d()!=spc.d() || tth()!=spc.tth();
}

void QxrdCalibrantDSpacing::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginGroup(name);
  settings->setValue("h", h());
  settings->setValue("k", k());
  settings->setValue("l", l());
  settings->setValue("d", d());
  settings->setValue("tth", tth());
  settings->endGroup();
}

void QxrdCalibrantDSpacing::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdCalibrantDSpacing spc = val.value<QxrdCalibrantDSpacing>();

  spc.setSettingsValue(settings, name);
}


QString QxrdCalibrantDSpacing::toString() const
{
  return QObject::tr("{h:%1, k:%2, l:%3, d:%4, tth:%5}").arg(h()).arg(k()).arg(l()).arg(d()).arg(tth());
}

void QxrdCalibrantDSpacing::registerMetaTypes()
{
  qRegisterMetaType<QxrdCalibrantDSpacing>("QxrdCalibrantDSpacing");

  qRegisterMetaTypeStreamOperators<QxrdCalibrantDSpacing>("QxrdCalibrantDSpacing");

  QcepProperty::registerCustomSaver("QxrdCalibrantDSpacing", QxrdCalibrantDSpacing::customSaver);
}

QScriptValue QxrdCalibrantDSpacing::toScriptValue(QScriptEngine *engine, const QxrdCalibrantDSpacing &spc)
{
  QScriptValue obj = engine->newObject();

  obj.setProperty("h", spc.h());
  obj.setProperty("k", spc.k());
  obj.setProperty("l", spc.l());
  obj.setProperty("d", spc.d());
  obj.setProperty("tth", spc.tth());

  return obj;
}

void QxrdCalibrantDSpacing::fromScriptValue(const QScriptValue &obj, QxrdCalibrantDSpacing &spc)
{
  spc.h() = obj.property("h").toInteger();
  spc.k() = obj.property("k").toInteger();
  spc.l() = obj.property("l").toInteger();
  spc.d() = obj.property("d").toNumber();
  spc.tth() = obj.property("tth").toNumber();
}

#ifndef QT_NO_DATASTREAM

QDataStream &operator<<(QDataStream &stream, const QxrdCalibrantDSpacing &pt)
{
  stream << pt.h() << pt.k() << pt.l() << pt.d() << pt.tth();

  return stream;
}

QDataStream &operator>>(QDataStream &stream, QxrdCalibrantDSpacing &pt)
{
  stream >> pt.h() >> pt.k() >> pt.l() >> pt.d() >> pt.tth();

  return stream;
}

#endif


void QxrdCalibrantDSpacingVector::setSettingsValue(QSettings *settings, QString name)
{
  settings->beginWriteArray(name, count());

  for (int i=0; i<count(); i++) {
    settings->setArrayIndex(i);
    const QxrdCalibrantDSpacing &pt = at(i);
    settings->setValue("h", pt.h());
    settings->setValue("k", pt.k());
    settings->setValue("l", pt.l());
    settings->setValue("d", pt.d());
    settings->setValue("tth", pt.tth());
  }

  settings->endArray();
}

void QxrdCalibrantDSpacingVector::customSaver(const QVariant &val, QSettings *settings, QString name)
{
  QxrdCalibrantDSpacingVector vec = val.value<QxrdCalibrantDSpacingVector>();

  vec.setSettingsValue(settings, name);
}

QString QxrdCalibrantDSpacingVector::toString() const
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

void QxrdCalibrantDSpacingVector::registerMetaTypes()
{
  qRegisterMetaType< QxrdCalibrantDSpacingVector >("QxrdCalibrantDSpacingVector");

  qRegisterMetaTypeStreamOperators< QxrdCalibrantDSpacingVector >("QxrdCalibrantDSpacingVector");

  QcepProperty::registerCustomSaver("QxrdCalibrantDSpacingVector", QxrdCalibrantDSpacingVector::customSaver);
}

QScriptValue QxrdCalibrantDSpacingVector::toScriptValue(QScriptEngine *engine,
                                                          const QxrdCalibrantDSpacingVector &vec)
{
  int n = vec.count();

  QScriptValue res = engine->newArray(n);

  for (int i=0; i<n; i++) {
    const QxrdCalibrantDSpacing &pt = vec.at(i);
    QScriptValue obj = engine->newObject();

    obj.setProperty("h", pt.h());
    obj.setProperty("k", pt.k());
    obj.setProperty("l", pt.l());
    obj.setProperty("d", pt.d());
    obj.setProperty("tth", pt.tth());

    res.setProperty(i, obj);
  }

  return res;
}

void QxrdCalibrantDSpacingVector::fromScriptValue(const QScriptValue &obj,
                                                    QxrdCalibrantDSpacingVector &vec)
{
  int n=obj.property("length").toInteger();

  vec.resize(n);

  for (int i=0; i<n; i++) {
    QScriptValue pt = obj.property(i);

    if (pt.isValid()) {
      vec[i].h() = pt.property("h").toInteger();
      vec[i].k() = pt.property("k").toInteger();
      vec[i].l() = pt.property("l").toInteger();
      vec[i].d() = pt.property("d").toNumber();
      vec[i].tth() = pt.property("tth").toNumber();
    }
  }
}

void QxrdCalibrantDSpacingVector::insertUnique(int h, int k, int l, double d, double tth)
{
  int n = count();

  for (int i=0; i<n; i++) {
    if (d == value(i).d()) return;
  }

  append(QxrdCalibrantDSpacing(h,k,l,d,tth));
}
