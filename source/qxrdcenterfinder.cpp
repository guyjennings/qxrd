#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qxrdmutexlocker.h"

QxrdCenterFinder::QxrdCenterFinder(QxrdSettingsSaverWPtr saver)
  : QxrdDetectorGeometry(),
    m_ObjectNamer(this, "centering"),
    m_CenterX(saver, this, "centerX", 0, "X Center"),
    m_CenterY(saver, this, "centerY", 0, "Y Center"),
    m_CenterStep(saver, this, "centerStep", 1, "Center Step"),
    m_DetectorXPixelSize(saver, this, "detectorXPixelSize", 200, "Detector X Pixels (um)"),
    m_DetectorYPixelSize(saver, this, "detectorYPixelSize", 200, "Detector Y Pixels (um)"),
    m_DetectorDistance(saver, this, "detectorDistance", 1000, "Sample-Detector Distance (mm)"),
    m_Energy(saver, this, "energy", 20000, "Beam Energy (eV)"),
    m_ImplementTilt(saver, this,"implementTilt", false, "Implement Detector Tilt?"),
    m_DetectorTilt(saver, this, "detectorTilt", 0, "Tilt Angle (deg)"),
    m_TiltPlaneRotation(saver, this, "tiltPlaneRotation", 90, "Tilt Plane Rotation (deg)"),
    m_EnableGeometricCorrections(saver, this, "enableGeometricCorrections", false, "Enable Geometric Corrections (tilt and distance) in Integration"),
    m_EnablePolarizationCorrections(saver, this, "enablePolarizationCorrections", false, "Enable Polarization Corrections in Integration"),
    m_Polarization(saver, this, "polarization", 1.0, "Beam Polarization Factor"),
    m_EnableAbsorptionCorrections(saver, this, "enableAbsorptionCorrections", false, "Enable Absorption Correction in Integration"),
    m_AttenuationLength(saver, this, "attenuationLength", 0, "Attenuation Length (mm)"),
    m_MarkedPoints(saver, this, "markedPoints", QcepPolygon(), "Marker Points"),
    m_AdjustMarkedPoints(saver, this, "adjustMarkedPoints", true, "Auto-adjust position of entered points"),
    m_AdjustmentRadius(saver, this, "adjustmentRadius", 3.0, "Size of search region for marker auto adjustment")
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
  connect(prop_EnableGeometricCorrections(), SIGNAL(valueChanged(bool,int)), this, SIGNAL(parameterChanged()));
  connect(prop_EnablePolarizationCorrections(), SIGNAL(valueChanged(bool,int)), this, SIGNAL(parameterChanged()));
  connect(prop_Polarization(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
  connect(prop_EnableAbsorptionCorrections(), SIGNAL(valueChanged(bool,int)), this, SIGNAL(parameterChanged()));
  connect(prop_AttenuationLength(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()));
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

double QxrdCenterFinder::getChi(QwtDoublePoint pt) const
{
  return getChi(pt.x(), pt.y());
}

double QxrdCenterFinder::getChi(double x, double y) const
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

  return chi;
}

double QxrdCenterFinder::getDist(QwtDoublePoint pt) const
{
  return getDist(pt.x(), pt.y());
}

double QxrdCenterFinder::getDist(double x, double y) const
{
  double tth = getTTH(x,y);

  return get_DetectorDistance()/cos(tth*M_PI/180.0);
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

void QxrdCenterFinder::onPointSelected(QwtDoublePoint pt)
{
  if (get_AdjustMarkedPoints()) {
    pt = adjustPoint(pt);
  }

  m_MarkedPoints.appendValue(pt);
}

void QxrdCenterFinder::fitPowderCircle()
{
}

QwtDoublePoint QxrdCenterFinder::powderPoint(int i)
{
  return get_MarkedPoints().value(i);
}

int QxrdCenterFinder::nearestPowderPointIndex(double x, double y)
{
  int nearest = -1;
  double nearestDist;
  QcepPolygon pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    QwtDoublePoint pt = pts.value(i);
    double dist = sqrt(pow(x-pt.x(), 2) + pow(y-pt.y(), 2));

    if (nearest == -1 || dist < nearestDist) {
      nearest     = i;
      nearestDist = dist;
    }
  }

  return nearest;
}

QwtDoublePoint QxrdCenterFinder::nearestPowderPoint(double x, double y)
{
  return get_MarkedPoints().value(nearestPowderPointIndex(x,y));
}

void QxrdCenterFinder::deletePowderPointNear(double x, double y)
{
  int nearest = nearestPowderPointIndex(x, y);

  if (nearest >= 0) {
    QcepPolygon pts = get_MarkedPoints();

    pts.remove(nearest);

    set_MarkedPoints(pts);
  }
}

void QxrdCenterFinder::deletePowderPoints()
{
  m_MarkedPoints.clear();
}

QwtDoublePoint QxrdCenterFinder::adjustPoint(QwtDoublePoint pt)
{
  double x0=pt.x(), y0=pt.y();
  int rad = get_AdjustmentRadius();
  double sum=0, sumx=0, sumy=0;

  for (int iy=-rad; iy<=rad; iy++) {
    double y=y0+iy;
    for (int ix=-rad; ix<=rad; ix++) {
      double x=x0+ix;
      double val = imageValue(x,y);
      sum += val;
      sumx += val*x;
      sumy += val*y;
    }
  }

  return QwtDoublePoint(sumx/sum, sumy/sum);
}

QwtDoublePoint QxrdCenterFinder::adjustPoint(int i)
{
  return adjustPoint(powderPoint(i));
}

void QxrdCenterFinder::adjustPointNear(double x, double y)
{
  int nearest = nearestPowderPointIndex(x, y);

  if (nearest >= 0) {
    QcepPolygon pts = get_MarkedPoints();

    QwtDoublePoint val = adjustPoint(pts[nearest]);

    pts[nearest] = val;

    set_MarkedPoints(pts);
  }
}

void QxrdCenterFinder::adjustAllPoints()
{
  QcepPolygon pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    pts[i] = adjustPoint(pts[i]);
  }

  set_MarkedPoints(pts);
}

void QxrdCenterFinder::setData(QxrdDoubleImageDataPtr data)
{
  m_Data = data;
}

double QxrdCenterFinder::imageValue(double x, double y)
{
  if (m_Data) {
    return m_Data->value(x,y);
  } else {
    return 0;
  }
}
