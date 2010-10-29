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
    m_ImplementTilt(this,"implementTilt", false),
    m_DetectorTilt(this, "detectorTilt", 0),
    m_TiltPlaneRotation(this, "tiltPlaneRotation", 90)
{
  qRegisterMetaType<QwtDoublePoint>("QwtDoublePoint");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);

  connect(prop_CenterX(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_CenterY(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorXPixelSize(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorYPixelSize(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorDistance(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_ImplementTilt(), SIGNAL(changedValue(bool)), this, SIGNAL(parameterChanged()));
  connect(prop_DetectorTilt(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
  connect(prop_TiltPlaneRotation(), SIGNAL(changedValue(double)), this, SIGNAL(parameterChanged()));
}

void QxrdCenterFinder::writeSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::readSettings(QxrdSettings &settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::onCenterChanged(QwtDoublePoint pt)
{
  set_CenterX(pt.x());
  set_CenterY(pt.y());
}

double QxrdCenterFinder::getTTH(QwtDoublePoint pt)
{
  return getTTH(pt.x(), pt.y());
}

double QxrdCenterFinder::getTTH(double x, double y)
{
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(), x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(), cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(), x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(), 1.0, 0.0, 1.0, 0.0);
  }
}
