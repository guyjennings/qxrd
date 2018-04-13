#include "qcepcalibrant.h"
#include "qcepcalibrantdspacing.h"
#include "qcepcalibrantdspacings.h"
#include <QScriptEngine>
#include "qcepdebug.h"

#include <qmath.h>

QcepCalibrant::QcepCalibrant(QString name)
  : QcepObject(name),
    m_IsUsed(this, "isUsed", 0, "Is Calibrant used?"),
    m_Index(this, "index", 0, "Calibrant Index"),
    m_Flags(this, "flags", 0, "Calibrant flags"),
    m_Description(this, "description", "Description", "Calibrant Description"),
    m_Symmetry(this, "symmetry", 0, "Calibrant Symmetry"),
    m_A(this, "a", 1.0, "calibrant a-axis"),
    m_B(this, "b", 1.0, "calibrant b-axis"),
    m_C(this, "c", 1.0, "calibrant c-axis"),
    m_Alpha(this, "alpha", 90.0, "calibrant alpha angle (deg)"),
    m_Beta(this, "beta", 90.0, "calibrant beta angle (deg)"),
    m_Gamma(this, "gamma", 90.0, "calibrant gamma angle (deg)")
{
}

QcepCalibrant::~QcepCalibrant()
{

}

bool QcepCalibrant::isLocked()
{
  return (get_Flags() & 1) == 0;
}

QScriptValue QcepCalibrant::toScriptValue(QScriptEngine *engine, const QcepCalibrantWPtr &cal)
{
  return engine->newQObject(cal.data());
}

void QcepCalibrant::fromScriptValue(const QScriptValue &obj, QcepCalibrantWPtr &cal)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepCalibrant *qcal = qobject_cast<QcepCalibrant*>(qobj);

    if (qcal) {
      cal = qSharedPointerDynamicCast<QcepCalibrant>(qcal->sharedFromThis());
    }
  }
}

QcepCalibrantDSpacing QcepCalibrant::dSpacing(int h, int k, int l)
{
  return QcepCalibrantDSpacing(get_Index(), h,k,l, 1, sqrt(h*h + k*k + l*l), 0);
}

class QcepCalibrantQuadInt {
public:
  QcepCalibrantQuadInt(int n=0, int h=-1, int k=-1, int l=-1) : m_N(n), m_H(h), m_K(k), m_L(l) {}

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

QcepCalibrantDSpacings QcepCalibrant::dSpacings(double energy)
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
    return QcepCalibrantDSpacings();
  }
}

QcepCalibrantDSpacings QcepCalibrant::dSpacingsCubic(double energy)
{
  int    s = get_Symmetry();
  double a = get_A();
  double lambda = (energy>100 ? 12398.4187/energy : energy);

  int mmax = 2.0*a/lambda + 1;

//  if (qcepDebug(DEBUG_CALIBRANT)) {
//    printMessage(tr("mmax = %1").arg(mmax));
//  }

  QVector<QcepCalibrantQuadInt> ex(mmax*mmax);

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
              ex[r] = QcepCalibrantQuadInt(1, h,k,l);
            } else {
              ex[r].n()++;
            }
          }
        }
      }
    }
  }

  QcepCalibrantDSpacings pts;

  for (int i=1; i<mmax*mmax; i++) {
    QcepCalibrantQuadInt e = ex[i];
    if (e.n() > 0) {
      double d = a/sqrt(i);
      double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

      if (tth <= 90) {
        pts.append(QcepCalibrantDSpacing(get_Index(), e.h(), e.k(), e.l(), e.n(), d, tth));

        if (qcepDebug(DEBUG_CALIBRANT)) {
          printMessage(tr("%1(%2): [%3,%4,%5], d:%6, tth:%7").arg(i).arg(e.n()).arg(e.h()).arg(e.k()).arg(e.l()).arg(d).arg(tth));
        }
      }
    }
  }

  return pts;
}

bool lessThan(const QcepCalibrantDSpacing &d1, const QcepCalibrantDSpacing&d2)
{
  return d1.tth() < d2.tth();
}

QcepCalibrantDSpacings QcepCalibrant::dSpacingsHexagonal(double energy)
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

  QcepCalibrantDSpacings pts;

  int lmin = 0;
  if (s == RHexagonal) {
    lmin = -lmax;
  }

  for (int l=lmin; l<=lmax+1; l++) {
    for (int k=0; k<=hkmax+1; k++) {

      int hmin=k;
      if (l<0) {
        hmin += 1;
      }

      for (int h=hmin; h<=hkmax+1; h++) {
        if (h || k || l) {
          int ok = false;

          switch (s) {
          case RHexagonal:
            ok = ((-h+k+l)%3 == 0);
            break;
          case Hexagonal:
            ok = true;
            break;
          }

          if (ok) {
            double d = 1.0/sqrt((4.0/3.0)*(h*h + h*k + k*k)/(a*a) + (l*l)/(c*c));
            double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

            if (tth < 90) {
              pts.insertUnique(get_Index(), h,k,l,d,tth);
            }
          }
        }
      }
    }
  }

  std::sort(pts.begin(), pts.end(), lessThan);

  return pts;
}

