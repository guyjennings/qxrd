#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qxrdmutexlocker.h"

QxrdCenterFinder::QxrdCenterFinder
    (QxrdSettingsSaverWPtr saver)
  : QxrdDetectorGeometry(),
    m_CenterX(saver, this, "centerX", 0, "X Center"),
    m_CenterY(saver, this, "centerY", 0, "Y Center"),
    m_CenterStep(saver, this, "centerStep", 1, "Center Step"),
    m_DetectorXPixelSize(saver, this, "detectorXPixelSize", 200, "Detector X Pixels (um)"),
    m_DetectorYPixelSize(saver, this, "detectorYPixelSize", 200, "Detector Y Pixels (um)"),
    m_DetectorDistance(saver, this, "detectorDistance", 1000, "Sample-Detector Distance (mm)"),
    m_Energy(saver, this, "energy", 20000, "Beam Energy (eV)"),
    m_ImplementTilt(saver, this,"implementTilt", false, "Implement Detector Tilt?"),
    m_DetectorTilt(saver, this, "detectorTilt", 0, "Tilt Angle (deg)"),
    m_TiltPlaneRotation(saver, this, "tiltPlaneRotation", 90, "Tilt Plane Rotation (deg)")
{
  setObjectName("centering");

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

void QxrdCenterFinder::writeSettings(QSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::readSettings(QSettings *settings, QString section)
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
