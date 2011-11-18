#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qxrdmutexlocker.h"

QxrdCenterFinder::QxrdCenterFinder
    (QObject *parent)
  : QxrdDetectorGeometry(parent),
    m_CenterX(this, "centerX", 0),
    m_CenterY(this, "centerY", 0),
    m_CenterStep(this, "centerStep", 1),
    m_DetectorXPixelSize(this, "detectorXPixelSize", 200),
    m_DetectorYPixelSize(this, "detectorYPixelSize", 200),
    m_DetectorDistance(this, "detectorDistance", 1000),
    m_Energy(this, "energy", 20000),
    m_ImplementTilt(this,"implementTilt", false),
    m_DetectorTilt(this, "detectorTilt", 0),
    m_TiltPlaneRotation(this, "tiltPlaneRotation", 90)
{
  qRegisterMetaType<QwtDoublePoint>("QwtDoublePoint");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);

  connect(prop_CenterX(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_CenterY(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorXPixelSize(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorYPixelSize(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorDistance(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_ImplementTilt(), SIGNAL(valueChanged(bool,int)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorTilt(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_TiltPlaneRotation(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
}

void QxrdCenterFinder::writeSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::readSettings(QSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::onCenterChanged(QwtDoublePoint pt)
{
  set_CenterX(pt.x());
  set_CenterY(pt.y());
}

double QxrdCenterFinder::getTTH(QwtDoublePoint pt) const
{
  return getTTH(pt.x(), pt.y());
}

double QxrdCenterFinder::getTTH(double x, double y) const
{
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       1.0, 0.0, 1.0, 0.0);
  }
}

double QxrdCenterFinder::getQ(QwtDoublePoint pt) const
{
  return getQ(pt.x(), pt.y());
}

double QxrdCenterFinder::getQ(double x, double y) const
{
  double q,chi;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    getQChi(get_CenterX(), get_CenterY(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
            &q, &chi);
  } else {
    getQChi(get_CenterX(), get_CenterY(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
            &q, &chi);
  }

  return q;
}

double QxrdCenterFinder::getR(QwtDoublePoint pt) const
{
  return getR(pt.x(), pt.y());
}

double QxrdCenterFinder::getR(double x, double y) const
{
  double tth = getTTH(x, y);
  double r = get_DetectorDistance()*tan(tth*M_PI/180.0);

  return r;
}
