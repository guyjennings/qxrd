#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qxrdmutexlocker.h"
#include "levmar.h"
#include <QMessageBox>
#include "qxrdapplication.h"
#include <QtConcurrentMap>
#include "qxrddebug.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QxrdCenterFinder::QxrdCenterFinder(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
  : QxrdDetectorGeometry(),
    m_ObjectNamer(this, "centering"),
    m_CenterX(saver, this, "centerX", 0, "X Center"),
    m_CenterY(saver, this, "centerY", 0, "Y Center"),
    m_CenterStep(saver, this, "centerStep", 1, "Center Step"),
    m_DetectorXPixelSize(saver, this, "detectorXPixelSize", 200, "Detector X Pixels (um)"),
    m_DetectorYPixelSize(saver, this, "detectorYPixelSize", 200, "Detector Y Pixels (um)"),
    m_DetectorDistance(saver, this, "detectorDistance", 1000, "Sample-Detector Distance (mm)"),
    m_DetectorDistanceStep(saver, this, "detectorDistanceStep", 100, "Sample-Detector Distance Step (mm)"),
    m_Energy(saver, this, "energy", 20000, "Beam Energy (eV)"),
    m_ImplementTilt(saver, this,"implementTilt", false, "Implement Detector Tilt?"),
    m_DetectorTilt(saver, this, "detectorTilt", 0, "Tilt Angle (deg)"),
    m_DetectorTiltStep(saver, this, "detectorTiltStep", 0.1, "Tilt Angle Step(deg)"),
    m_TiltPlaneRotation(saver, this, "tiltPlaneRotation", 90, "Tilt Plane Rotation (deg)"),
    m_TiltPlaneRotationStep(saver, this, "tiltPlaneRotationStep", 10, "Tilt Plane Rotation Step (deg)"),
    m_EnableGeometricCorrections(saver, this, "enableGeometricCorrections", false, "Enable Geometric Corrections (tilt and distance) in Integration"),
    m_EnablePolarizationCorrections(saver, this, "enablePolarizationCorrections", false, "Enable Polarization Corrections in Integration"),
    m_Polarization(saver, this, "polarization", 1.0, "Beam Polarization Factor"),
    m_EnableAbsorptionCorrections(saver, this, "enableAbsorptionCorrections", false, "Enable Absorption Correction in Integration"),
    m_AttenuationLength(saver, this, "attenuationLength", 0, "Attenuation Length (mm)"),
    m_MarkedPoints(saver, this, "markedPoints", QcepPolygon(), "Marker Points"),
    m_RingRadius(saver, this, "ringRadius", 0.0, "Estimated Powder Ring Radius"),
    m_AdjustMarkedPoints(saver, this, "adjustMarkedPoints", true, "Auto-adjust position of entered points"),
    m_AdjustmentRadius(saver, this, "adjustmentRadius", 3.0, "Size of search region for marker auto adjustment"),
    m_EnableUserGeometry(saver, this, "enableUserGeometry", 0, "Apply user-defined geometry function in integration"),
    m_UserGeometryScript(saver, this, "userGeometryScript", defaultUserGeometryScript(), "Script to define user defined geometry functions"),
    m_UserGeometryFunction(saver, this, "userGeometryFunction", "userGeometry", "Name of user defined geometry function"),
    m_EnableUserAbsorption(saver, this, "enableUserAbsorption", 0, "Apply user-defined geometry function in integration"),
    m_UserAbsorptionScript(saver, this, "userAbsorptionScript", defaultUserAbsorptionScript(), "Script to define user defined absorption functions"),
    m_UserAbsorptionFunction(saver, this, "userAbsorptionFunction", "userAbsorb1", "Name of user defined absorption function"),
    m_PeakRadius(saver, this, "peakRadius", 2.0, "Radius of fitted peak"),
    m_PeakFitRadius(saver, this, "peakFitRadius", 10, "Half size of fitted area for peak fitting"),
    m_PeakHeight(saver, this, "peakHeight", 100.0, "Height of fitted peak"),
    m_PeakCenterX(saver, this, "peakCenterX", 0, "X Center of fitted peak"),
    m_PeakCenterY(saver, this, "peakCenterY", 0, "Y Center of fitted peak"),
    m_PeakBackground(saver, this, "peakBackground", 0, "Background Height of fitted peak"),
    m_PeakBackgroundX(saver, this, "peakBackgroundX", 0, "X Slope of Background"),
    m_PeakBackgroundY(saver, this, "peakBackgroundY", 0, "Y Slope of Background"),
    m_PeakAzimuth(saver, this, "peakAzimuth", 0, "Azimuthal angle of fitted ring sample"),
    m_PeakPixelRadius(saver, this, "peakPixelRadius", 0, "Radius of fitted ring sample"),
    m_PeakFitDebug(saver, this, "peakFitDebug", false, "Debug Print for peak fitting"),
    m_PeakFitIterations(saver, this, "peakFitIterations", 200, "Max Iterations during fitting"),
    m_RingAngles(saver, this, "ringAngles", QcepDoubleVector(), "Diffraction ring angles"),
    m_RingAngleTolerance(saver, this, "ringAngleToleance", 0.1, "Diffraction ring angle tolerance"),
    m_PowderFitOptions(saver, this, "powderFitOptions", 0, "Powder fitting options"),
    m_PowderPoint(saver, this, "powderPoint", QxrdPowderPoint(1,2,3,4), "Powder Point"),
    m_PowderPointVector(saver, this, "powderPointVector", QxrdPowderPointVector(), "Powder Point Vector"),
    m_RingIndex(saver, this, "ringIndex", 0, "Fitted Powder Ring Index"),
    m_SubtractRingAverages(saver, this, "subtractRingAverages", false, "Plot deviations of each ring from average"),
    m_FittedWidthMin(saver, this, "fittedWidthMin", 0.5, "Minimum acceptable fitted width (pixels)"),
    m_FittedWidthMax(saver, this, "fittedWidthMax", 3.0, "Maximum acceptable fitted width (pixels)"),
    m_FittedHeightMinRatio(saver, this, "fittedHeightMinRatio", 0.25, "Minimum acceptable peak height ratio"),
    m_FittedPositionMaxDistance(saver, this, "fittedPositionMaxDistance", 2.0, "Maximum acceptable fitted position shift (pixels)"),
    m_Experiment(expt)
{
  qRegisterMetaType<QPointF>("QPointF");

  m_PowderPointVector.appendValue(QxrdPowderPoint(1,2,3,4));
  m_PowderPointVector.appendValue(QxrdPowderPoint(5,6,7,8));
  m_PowderPointVector.appendValue(QxrdPowderPoint(9,10,11,12));

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

  connect(prop_EnableUserGeometry(), SIGNAL(valueChanged(int,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserGeometryScript(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserGeometryFunction(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));

  connect(prop_EnableUserAbsorption(), SIGNAL(valueChanged(int,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserAbsorptionScript(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserAbsorptionFunction(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
}

QxrdExperimentWPtr QxrdCenterFinder::experiment() const
{
  return m_Experiment;
}

QxrdDoubleImageDataPtr QxrdCenterFinder::data() const
{
  return m_Data;
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

void QxrdCenterFinder::onCenterChanged(QPointF pt)
{
  set_CenterX(pt.x());
  set_CenterY(pt.y());
}

double QxrdCenterFinder::getTTH(QPointF pt) const
{
  return getTTH(pt.x(), pt.y());
}

double QxrdCenterFinder::getTTH(double x, double y) const
{
  if (get_ImplementTilt()) {
    double beta = get_DetectorTilt()*M_PI/180.0;
    double rot  = get_TiltPlaneRotation()*M_PI/180.0;

    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return getTwoTheta(get_CenterX(), get_CenterY(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       1.0, 0.0, 1.0, 0.0);
  }
}

QPointF QxrdCenterFinder::getXY(double tth, double chi)
{
  double x,y;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {

    QxrdDetectorGeometry::getXY(get_CenterX(), get_CenterY(), get_DetectorDistance(),
          get_Energy(),
          convertTwoThetaToQ(tth, convertEnergyToWavelength(get_Energy())), chi,
          get_DetectorXPixelSize(), get_DetectorYPixelSize(),
          rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
          &x, &y);
  } else {
    QxrdDetectorGeometry::getXY(get_CenterX(), get_CenterY(), get_DetectorDistance(),
          get_Energy(),
          convertTwoThetaToQ(tth, convertEnergyToWavelength(get_Energy())), chi,
          get_DetectorXPixelSize(), get_DetectorYPixelSize(),
          rot, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
          &x, &y);
  }

  return QPointF(x,y);
}

double QxrdCenterFinder::getQ(QPointF pt) const
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

double QxrdCenterFinder::getChi(QPointF pt) const
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

double QxrdCenterFinder::getDist(QPointF pt) const
{
  return getDist(pt.x(), pt.y());
}

double QxrdCenterFinder::getDist(double x, double y) const
{
  double tth = getTTH(x,y);

  return get_DetectorDistance()/cos(tth*M_PI/180.0);
}

double QxrdCenterFinder::getR(QPointF pt) const
{
  return getR(pt.x(), pt.y());
}

double QxrdCenterFinder::getR(double x, double y) const
{
  double tth = getTTH(x, y);
  double r = get_DetectorDistance()*tan(tth*M_PI/180.0);

  return r;
}

void QxrdCenterFinder::onPointSelected(QPointF pt)
{
  if (get_AdjustMarkedPoints()) {
    pt = adjustPoint(pt);
  }

  m_MarkedPoints.appendValue(pt);
}

void QxrdCenterFinder::evaluateFit(double *parm, double *x, int /*np*/, int nx)
{
  QcepPolygon pts = get_MarkedPoints();
  double cx = parm[0];
  double cy = parm[1];
  double r  = parm[2];

  for (int i=0; i<nx; i++) {
    QPointF pt = pts.value(i);
    double rcalc = sqrt(pow(pt.x() - cx, 2) + pow(pt.y() - cy, 2));
    x[i] = rcalc - r;
  }
}

static void fitCenter(double *p, double *hx, int m,int n, void *adata)
{
  QxrdCenterFinder *cf = (QxrdCenterFinder*) adata;

  if (cf) {
    cf->evaluateFit(p, hx, m, n);
  }
}

void QxrdCenterFinder::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdCenterFinder::fitPowderCircle()
{
  if (get_MarkedPoints().count() <= 3) {
    QString message = "You must mark at least three points on a powder ring before you can fit the center";

    if (g_Application->get_GuiWanted()) {
      QMessageBox::information(NULL, "Fitting Failed", message);
    } else {
      printMessage(message);
    }

    return;
  }

  double parms[3];
  QcepDoubleVector vals(get_MarkedPoints().count());

  parms[0] = get_CenterX();
  parms[1] = get_CenterY();
  parms[2] = get_RingRadius();

  double info[LM_INFO_SZ];

  int niter = dlevmar_dif(fitCenter, parms, NULL, 3, vals.count(), 100, NULL, info, NULL, NULL, this);

  int update = false;
  QString message;

  if (niter >= 0) {
    message.append(tr("Fitting Succeeded after %1 iterations\n").arg(niter));

    message.append(tr("New Center = [%1,%2], New Radius = %3\n").arg(parms[0]).arg(parms[1]).arg(parms[2]));

  } else {
    message.append(tr("dlevmar_dif failed: reason = %1").arg(info[6]));
  }

  printMessage(message);

  if (g_Application->get_GuiWanted()) {
    if (niter >= 0) {
      message.append(tr("Do you want to update the beam centering parameters?"));

      if (QMessageBox::question(NULL, "Update Fitted Center?", message, QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok) == QMessageBox::Ok) {
        update = true;
      }
    } else {
      QMessageBox::information(NULL, "Fitting Failed", message);
    }
  } else if (niter >= 0){
    update = true;
  }

  if (update) {
    set_CenterX(parms[0]);
    set_CenterY(parms[1]);
    set_RingRadius(parms[2]);
  }
}

QPointF QxrdCenterFinder::powderPoint(int i)
{
  return get_MarkedPoints().value(i);
}

int QxrdCenterFinder::nearestPowderPointIndex(double x, double y)
{
  int nearest = -1;
  double nearestDist;
  QcepPolygon pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    QPointF pt = pts.value(i);
    double dist = sqrt(pow(x-pt.x(), 2) + pow(y-pt.y(), 2));

    if (nearest == -1 || dist < nearestDist) {
      nearest     = i;
      nearestDist = dist;
    }
  }

  return nearest;
}

QPointF QxrdCenterFinder::nearestPowderPoint(double x, double y)
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

void QxrdCenterFinder::appendPowderPoint(double x, double y)
{
  m_MarkedPoints.appendValue(QPointF(x,y));
}

void QxrdCenterFinder::deletePowderPoints()
{
  m_MarkedPoints.clear();

  set_RingIndex(0);
}

QPointF QxrdCenterFinder::adjustPoint(QPointF pt)
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

  if (sum != 0) {
    return QPointF(sumx/sum, sumy/sum);
  } else {
    return pt;
  }
}

QPointF QxrdCenterFinder::adjustPoint(int i)
{
  return adjustPoint(powderPoint(i));
}

void QxrdCenterFinder::adjustPointNear(double x, double y)
{
  int nearest = nearestPowderPointIndex(x, y);

  if (nearest >= 0) {
    QcepPolygon pts = get_MarkedPoints();

    QPointF val = adjustPoint(pts[nearest]);

    pts[nearest] = val;

    set_MarkedPoints(pts);
  }
}

void QxrdCenterFinder::evaluatePeakFit(double *parm, double *xv, int /*np*/, int nx)
{
  double cx = parm[0];
  double cy = parm[1];
  double r  = parm[2];
  double ht = parm[3];
  double bg = parm[4];
  double bx = parm[5];
  double by = parm[6];

  double cx0 = get_PeakCenterX();
  double cy0 = get_PeakCenterY();
  double rr0 = get_PeakFitRadius();

  int n = get_PeakFitRadius()*2+1;
  int x0 = cx0 - rr0;
  int y0 = cy0 - rr0;
  int nn = n*n;
  int i=0;

  for (int y=y0; y<y0+n; y++) {
    for (int x=x0; x<x0+n; x++) {
      double d = imageValue(x,y);

      double dx = x-cx;
      double dy = y-cy;
      double pk = bg + dx*bx + dy*by + ht*exp(-((dx*dx+dy*dy)/(2.0*r*r)));

      xv[i++] = pk - d;
    }
  }
}

static void fitPeak(double *p, double *hx, int m,int n, void *adata)
{
  QxrdCenterFinder *cf = (QxrdCenterFinder*) adata;

  if (cf) {
    cf->evaluatePeakFit(p, hx, m, n);
  }
}

bool QxrdCenterFinder::fitPeakNear(double x, double y)
{
  printMessage(tr("centering.fitPeakNear(%1,%2)").arg(x).arg(y));

  set_PeakCenterX(x);
  set_PeakCenterY(y);

  double dr = get_PeakFitRadius();

  double bkgd = ( imageValue(x+dr, y) +
                  imageValue(x, y+dr) +
                  imageValue(x-dr, y) +
                  imageValue(x, y-dr) )/4.0;
  double pkht = imageValue(x,y) - bkgd;

  double parms[7];

  parms[0] = x;
  parms[1] = y;
  parms[2] = 2.0;
  parms[3] = pkht;
  parms[4] = bkgd;
  parms[5] = 0;
  parms[6] = 0;

  double info[LM_INFO_SZ];

  int n = get_PeakFitRadius()*2+1;

  int niter = dlevmar_dif(fitPeak, parms, NULL, 7, n*n, get_PeakFitIterations(), NULL, info, NULL, NULL, this);

  if (niter >= 0) {
    QString msg = tr("fit peak nr [%1,%2]\t").arg(x).arg(y);
    msg += tr("OK %1 iter:\t").arg(niter);
    msg += tr("Pos: [%1,%2]\t").arg(parms[0]).arg(parms[1]);
    msg += tr("Wid: %1\t").arg(fabs(parms[2]));
    msg += tr("Ht: %1\t").arg(parms[3]);
    msg += tr("Bkd: %1\t").arg(parms[4]);
    msg += tr("Bkx: %1\t").arg(parms[5]);
    msg += tr("Bky: %1").arg(parms[6]);

    printMessage(msg);

    set_PeakCenterX(parms[0]);
    set_PeakCenterY(parms[1]);
    set_PeakRadius(parms[2]);
    set_PeakHeight(parms[3]);
    set_PeakBackground(parms[4]);
    set_PeakBackgroundX(parms[5]);
    set_PeakBackgroundY(parms[6]);

    return true;
  } else {
    printMessage(tr("Fitting Failed"));

    return false;
  }
}

void QxrdCenterFinder::evaluateRingFit(double *parm, double *xv, int /*np*/, int nx)
{
  double pr = parm[0];
  double wd = parm[1];
  double ht = parm[2];
  double bg = parm[3];
  double bx = parm[4];
  double by = parm[5];

  double az  = get_PeakAzimuth();
  double cx0 = get_PeakCenterX();
  double cy0 = get_PeakCenterY();
  double rr0 = get_PeakFitRadius();
  double crx = get_CenterX();
  double cry = get_CenterY();

  int n = get_PeakFitRadius()*2+1;
  int x0 = cx0 - rr0;
  int y0 = cy0 - rr0;
  int nn = n*n;
  int i=0;

  double cx  = crx + pr*cos(az);
  double cy  = cry + pr*sin(az);

  double dx0 = cx - crx;
  double dy0 = cy - cry;
  double r0  = sqrt(dx0*dx0+dy0*dy0);

  for (int y=y0; y<y0+n; y++) {
    for (int x=x0; x<x0+n; x++) {
      double d = imageValue(x,y);

      double dx = x-cx;
      double dy = y-cy;
      double dx1 = x - crx;
      double dy1 = y - cry;
      double r1 = sqrt(dx1*dx1+dy1*dy1);
      double dr = r1-r0;
      double pk = bg + dx*bx + dy*by + ht*exp(-((dr*dr)/(2.0*wd*wd)));

      xv[i++] = pk - d;
    }
  }
}

static void fitRing(double *p, double *hx, int m,int n, void *adata)
{
  QxrdCenterFinder *cf = (QxrdCenterFinder*) adata;

  if (cf) {
    cf->evaluateRingFit(p, hx, m, n);
  }
}

bool QxrdCenterFinder::fitRingNear(double x0, double y0)
{
  printMessage(tr("centering.fitRingNear(%1,%2)").arg(x0).arg(y0));

  QxrdRingFitResult fit(this, 0, x0, y0, 0, 0);
  fit.fitRingPoint();

  double xc = get_CenterX();
  double yc = get_CenterY();
  double dx = x0 - xc;
  double dy = y0 - yc;
  double r  = sqrt(dx*dx+dy*dy);
  double az = atan2(dy,dx);
  double dr = get_PeakFitRadius();

  set_PeakAzimuth(az);
  set_PeakPixelRadius(r);
  set_PeakCenterX(x0);
  set_PeakCenterY(y0);

  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

  double pkht = imageValue(x0,y0) - bkgd;

  double parms[6];

  parms[0] = r;
  parms[1] = 2.0;
  parms[2] = pkht;
  parms[3] = bkgd;
  parms[4] = 0;
  parms[5] = 0;

  double info[LM_INFO_SZ];

  int n = get_PeakFitRadius()*2+1;

  int niter = dlevmar_dif(fitRing, parms, NULL, 6, n*n, get_PeakFitIterations(), NULL, info, NULL, NULL, this);

  if (niter >= 0) {
    QString msg = tr("fit ring nr [%1,%2]\t").arg(x0).arg(y0);
    msg += tr("OK %1 iter:\t").arg(niter);
    msg += tr("Rad: %1\t").arg(parms[0]);
    msg += tr("Wid: %1\t").arg(fabs(parms[1]));
    msg += tr("Ht: %1\t").arg(parms[2]);
    msg += tr("Bkd: %1\t").arg(parms[3]);
    msg += tr("Bkx: %1\t").arg(parms[4]);
    msg += tr("Bky: %1").arg(parms[5]);

    printMessage(msg);

    double nr = parms[0];

    set_PeakPixelRadius(nr);
    set_PeakCenterX(xc + nr*cos(az));
    set_PeakCenterY(yc + nr*sin(az));
    set_PeakRadius(parms[1]);
    set_PeakHeight(parms[2]);
    set_PeakBackground(parms[3]);
    set_PeakBackgroundX(parms[4]);
    set_PeakBackgroundY(parms[5]);

    return true;
  } else {
    printMessage(tr("Fitting Failed"));

    return false;
  }
}

bool QxrdCenterFinder::traceRingNear(double x0, double y0, double step)
{
  printMessage(tr("centering.traceRingNear(%1,%2,%3)").arg(x0).arg(y0).arg(step));

  double x=x0, y=y0;
  double xc  = get_CenterX();
  double yc  = get_CenterY();
  double dx  = x0-xc;
  double dy  = y0-yc;
  double az  = atan2(dy, dx);
  double az0 = az;
  double r   = sqrt(dx*dx + dy*dy);
  double ast = step/r;
  double dr  = get_PeakFitRadius();

  QxrdRingFitResult fit(this, 0, x0, y0, 0, 0);
  fit.fitRingPoint();

  printMessage(tr("Initial fit: az: %1, x: %2, y: %3, nx: %4, ny: %5, wd: %6, ht: %7, rz: %8")
               .arg(az).arg(x).arg(y).arg(fit.fittedX()).arg(fit.fittedY())
               .arg(fit.fittedWidth()).arg(fit.fittedHeight())
               .arg(fit.reasonString()));

  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

  double pkht = imageValue(x0,y0) - bkgd;

  int    width = 0, height = 0;

  if (m_Data) {
    width = m_Data->get_Width()+1;
    height = m_Data->get_Height()+1;
  }

  while (true) {
    if (x >= 0 && y >= 0 && x <= width && y <= width) {
      QxrdRingFitResult fit(this, 0, x, y, pkht, bkgd);
      fit.fitRingPoint();

      if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
        printMessage(tr("Fit Ring Near: az: %1, x: %2, y: %3, nx: %4, ny: %5, wd: %6, ht: %7, rz: %8")
                     .arg(az).arg(x).arg(y).arg(fit.fittedX()).arg(fit.fittedY())
                     .arg(fit.fittedWidth()).arg(fit.fittedHeight())
                     .arg(fit.reasonString()));
      }

      x = fit.fittedX();
      y = fit.fittedY();

      if (fit.reason()==QxrdRingFitResult::Successful) {
        m_RingPowderPoints.append(QxrdPowderPoint(get_RingIndex(), 1, x, y));
        r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
      } else {
        m_RingPowderPoints.append(QxrdPowderPoint(get_RingIndex(), 0, x, y));
      }
    }

    az += ast;

    if (step > 0) {
      if ((az-az0) >= 2*M_PI) break;
    } else {
      if ((az-az0) <= -2*M_PI) break;
    }

    x = xc + r*cos(az);
    y = yc + r*sin(az);
  }

  int npts = 0;
  int nok  = 0;

  for(int i=0; i<m_RingPowderPoints.count(); i++) {
    QxrdPowderPoint pt = m_RingPowderPoints[i];

    if (pt.n1() == get_RingIndex()) {
      npts += 1;
      if (pt.n2()) {
        nok += 1;
        appendPowderPoint(pt.x(), pt.y());
      }
    }
  }

  if (npts) {
    printMessage(tr("%1/%2 fitted points").arg(nok).arg(npts));

    if (nok) {
      prop_RingIndex()->incValue(1);
    }
  }

  return nok;
}

QxrdRingFitResult::QxrdRingFitResult(QxrdCenterFinder *cf, int index, double x0, double y0, double pkht, double bkgd) :
  m_CenterFinder(cf),
  m_Index(index),
  m_X0(x0),
  m_Y0(y0),
  m_Pkht(pkht),
  m_Bkgd(bkgd),
  m_Reason(NoResult),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

QxrdRingFitResult::QxrdRingFitResult() :
  m_CenterFinder(NULL),
  m_Index(0),
  m_X0(0.0),
  m_Y0(0.0),
  m_Pkht(0.0),
  m_Bkgd(0.0),
  m_Reason(NoResult),
  m_FittedX(0.0),
  m_FittedY(0.0),
  m_FittedWidth(0.0),
  m_FittedHeight(0.0),
  m_FittedBkgd(0.0),
  m_FittedBkgdX(0.0),
  m_FittedBkgdY(0.0)
{
}

//QxrdRingFitResult::QxrdRingFitResult(const QxrdRingFitResult &cpy) :
//  m_CenterFinder(cpy.cf()),
//  m_TTH(cpy.tth()),
//  m_Chi(cpy.chi()),
//  m_Pkht(cpy.pkht()),
//  m_Bkgd(cpy.bkgd())
//{
//}

static void fitRingParallel(double *p, double *hx, int m, int n, void *adata)
{
  QxrdRingFitResult *rf = (QxrdRingFitResult*) adata;

  if (rf) {
    rf->evaluateRingFit(p,hx,m,n);
  }
}

void QxrdRingFitResult::evaluateRingFit(double *parm, double *xv, int np, int nx)
{
  if (m_CenterFinder) {
//    m_CenterFinder->printMessage("Fitting");

    if (np!=6) {
      m_CenterFinder->printMessage("Wrong number of parameters");
    } else {
      double pr = parm[0];
      double wd = parm[1];
      double ht = parm[2];
      double bg = parm[3];
      double bx = parm[4];
      double by = parm[5];

      double dr = m_CenterFinder->get_PeakFitRadius();
      double xc = m_CenterFinder->get_CenterX();
      double yc = m_CenterFinder->get_CenterY();
      double cx0 = m_X0;
      double cy0 = m_Y0;
      double az = atan2(cy0-yc, cx0-xc);

      int n = dr*2+1;
      int x0 = cx0 - dr;
      int y0 = cy0 - dr;
      int nn = n*n;
      int i=0;

      double cx  = xc + pr*cos(az);
      double cy  = yc + pr*sin(az);

      double dx0 = cx - xc;
      double dy0 = cy - yc;
      double r0  = sqrt(dx0*dx0+dy0*dy0);

      for (int y=y0; y<y0+n; y++) {
        for (int x=x0; x<x0+n; x++) {
          double d = m_CenterFinder->imageValue(x,y);

          double dx = x-cx;
          double dy = y-cy;
          double dx1 = x - xc;
          double dy1 = y - yc;
          double r1 = sqrt(dx1*dx1+dy1*dy1);
          double dr = r1-r0;
          double pk = bg + dx*bx + dy*by + ht*exp(-((dr*dr)/(2.0*wd*wd)));

          xv[i++] = pk - d;
        }
      }
    }
  }
}

void QxrdRingFitResult::fitRingPoint()
{
  if (m_CenterFinder) {

    double x = m_X0, y = m_Y0;

//    if (qcepDebug(DEBUG_FITTING) || m_CenterFinder->get_PeakFitDebug()) {
//      m_CenterFinder -> printMessage(QObject::tr("Fitting i: %1, x0: %2, y0: %3")
//                                     .arg(index()).arg(m_X0).arg(m_Y0));
//    }

    int    width = 0, height = 0;

    QxrdDoubleImageDataPtr data(m_CenterFinder->data());

    if (data) {
      width  = data -> get_Width()+1;
      height = data -> get_Height()+1;

      if (x<0 || x>width || y<0 || y>height) {
        m_Reason = OutsideData;
      } else {
        double xc = m_CenterFinder->get_CenterX();
        double yc = m_CenterFinder->get_CenterY();
        double dx = x - xc;
        double dy = y - yc;
        double r  = sqrt(dx*dx + dy*dy);
        double chi = atan2(dy, dx);
        double dr  = m_CenterFinder->get_PeakFitRadius();
        double bkgd = ( m_CenterFinder->imageValue(xc+(r+dr)*cos(chi), yc+(r+dr)*sin(chi))
                       +m_CenterFinder->imageValue(xc+(r-dr)*cos(chi), yc+(r-dr)*sin(chi)))/2.0;
        double pkht = m_CenterFinder->imageValue(x,y) - bkgd;

        double parms[6];

        parms[0] = r;
        parms[1] = 2.0;
        parms[2] = pkht;
        parms[3] = bkgd;
        parms[4] = 0;
        parms[5] = 0;

        double info[LM_INFO_SZ];

        int n = m_CenterFinder->get_PeakFitRadius()*2+1;

        int niter = dlevmar_dif(fitRingParallel,
                                parms, NULL, 6, n*n,
                                m_CenterFinder->get_PeakFitIterations(),
                                NULL, info, NULL, NULL, this);

        if (niter > 0) {
          m_Reason       = Successful;
          m_FittedX      = xc + parms[0]*cos(chi);
          m_FittedY      = yc + parms[0]*sin(chi);
          m_FittedWidth  = parms[1];
          m_FittedHeight = parms[2];
          m_FittedBkgd   = parms[3];
          m_FittedBkgdX  = parms[4];
          m_FittedBkgdY  = parms[5];

          double dx = m_FittedX - m_X0;
          double dy = m_FittedY - m_Y0;
          double dr = sqrt(dx*dx + dy*dy);

          if ((fabs(m_FittedWidth)<m_CenterFinder->get_FittedWidthMin()) ||
              (fabs(m_FittedWidth)>m_CenterFinder->get_FittedWidthMax())) {
            m_Reason = BadWidth;
          } else if ((m_FittedHeight/m_Pkht)<m_CenterFinder->get_FittedHeightMinRatio()) {
            m_Reason = BadHeight;
          } else if (dr>m_CenterFinder->get_FittedPositionMaxDistance()) {
            m_Reason = BadPosition;
          }
        }
      }
    }
  }
}

QString QxrdRingFitResult::reasonString() const
{
  QString res = "Unknown";

  switch (m_Reason) {
  case NoResult:
    res = "No Result";
    break;

  case OutsideData:
    res = "Outside range of data";
    break;

  case Successful:
    res = "Successful";
    break;

  case BadWidth:
    res = "Bad Width";
    break;

  case BadPosition:
    res = "Bad Position";
    break;

  case BadHeight:
    res = "Bad Height";
    break;
  }

  return res;
}

bool QxrdCenterFinder::traceRingNearParallel(double x0, double y0, double step)
{
  printMessage(tr("centering.traceRingNearParallel(%1,%2,%3)")
               .arg(x0).arg(y0).arg(step));

  double xc  = get_CenterX();
  double yc  = get_CenterY();
  double dx  = x0-xc;
  double dy  = y0-yc;
  double dr  = get_PeakFitRadius();
  double r   = sqrt(dx*dx+dy*dy);
  double az  = atan2(dy, dx);
  double ast = step/r;

  QxrdRingFitResult fit(this, 0, x0, y0, 0, 0);

  fit.fitRingPoint();

  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

  double pkht = imageValue(x0,y0) - bkgd;

  int nsteps = (int) ((2.0*M_PI)/ast)+1;

  QVector<QxrdRingFitResult> fits;

  double tth = getTTH(x0, y0);
  for (int i=0; i<nsteps; i++) {
    double chi = ast*i*180.0/M_PI;
    QPointF xy  = getXY(tth, chi);

    fits.append(QxrdRingFitResult(this, i, xy.x(), xy.y(), pkht, bkgd));

//    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
//      printMessage(tr("Fitting i: %1, x0: %2, y0: %3").arg(i).arg(fits[i].x0()).arg(fits[i].y0()));
//    }
  }

  if (qcepDebug(DEBUG_NOPARALLEL)) {
    for (int i=0; i<nsteps; i++) {
      fits[i].fitRingPoint();
    }
  } else {
    QFuture<void> fitDone = QtConcurrent::map(fits, &QxrdRingFitResult::fitRingPoint);

    fitDone.waitForFinished();
  }

  int sums[6];

  for (int i=0; i<6; i++) {
    sums[i]=0;
  }

  for (int i=0; i<nsteps; i++) {
    QxrdRingFitResult &r = fits[i];

//    QPointF xy = getXY(r.tth(), r.chi());

    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
      printMessage(tr("Fitted %1 : x %2, y %3, w %4, ht %5, bk %6, bkx %7, bky %8, rzn %9")
                   .arg(i).arg(r.fittedX()).arg(r.fittedY())
                   .arg(r.fittedWidth()).arg(r.fittedHeight())
                   .arg(r.fittedBkgd()).arg(r.fittedBkgdX()).arg(r.fittedBkgdY())
                   .arg(r.reasonString()));
//      printMessage(tr("tth %1, chi %2")
//                   .arg(getTTH(r.fittedX(), r.fittedY()))
//                   .arg(getChi(r.fittedX(), r.fittedY())));
    }

    int rz = r.reason();
    if (rz>=0 && rz<6) {
      sums[rz]++;
    }

    if (r.reason() == QxrdRingFitResult::Successful) {
      appendPowderPoint(r.fittedX(), r.fittedY());
    }
  }

  printMessage(tr("Fitted %1/%2 : NR %3, OR %4, BdW %5, BdP %6, BdH %7")
      .arg(sums[QxrdRingFitResult::Successful])
      .arg(nsteps)
      .arg(sums[QxrdRingFitResult::NoResult])
      .arg(sums[QxrdRingFitResult::OutsideData])
      .arg(sums[QxrdRingFitResult::BadWidth])
      .arg(sums[QxrdRingFitResult::BadPosition])
      .arg(sums[QxrdRingFitResult::BadHeight])
      );

  return true;
}

void QxrdCenterFinder::adjustAllPoints()
{
  QcepPolygon pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    pts[i] = adjustPoint(pts[i]);
  }

  set_MarkedPoints(pts);
}

double QxrdCenterFinder::getPowderPointX(int i)
{
  QPointF res = get_MarkedPoints().value(i);

  return res.x();
}

double QxrdCenterFinder::getPowderPointY(int i)
{
  QPointF res = get_MarkedPoints().value(i);

  return res.y();
}

void QxrdCenterFinder::setPowderPoint(int i, double x, double y)
{
  QcepPolygon pts = get_MarkedPoints();

  if (i>=0 && i<pts.count()) {
    pts[i] = QPointF(x,y);
  } else {
    pts.append(QPointF(x,y));
  }

  set_MarkedPoints(pts);
}

QScriptValue QxrdCenterFinder::getPowderPoint(int i)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdScriptEnginePtr eng(exp->scriptEngine());

    if (eng) {
      if (i>=0 && i<get_MarkedPoints().count()) {
        QScriptValue val = eng->newObject();

        val.setProperty("x", getPowderPointX(i));
        val.setProperty("y", getPowderPointY(i));

        return val;
      }
    }
  }

  return QScriptValue();
}

QScriptValue QxrdCenterFinder::getPowderPoints()
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdScriptEnginePtr eng(exp->scriptEngine());

    if (eng) {
      QcepPolygon pts = get_MarkedPoints();

      QScriptValue val = eng->newArray();

      for (int i=0; i<pts.count(); i++) {
        QScriptValue item = eng->newObject();

        item.setProperty("x", pts[i].x());
        item.setProperty("y", pts[i].y());

        val.setProperty(tr("%1").arg(i), item);
      }

      return val;
    }
  }

  return QScriptValue();
}

int          QxrdCenterFinder::countPowderPoints()
{
  return get_MarkedPoints().count();
}

void         QxrdCenterFinder::setPowderPoint(int i, QScriptValue val)
{
  double x = val.property("x").toNumber();
  double y = val.property("y").toNumber();

  setPowderPoint(i, x, y);
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

QString QxrdCenterFinder::defaultUserGeometryScript()
{
  QFile def(":/qxrdexampleusergeometry.js");

  if (def.open(QFile::ReadOnly)) {
    return def.readAll();
  } else {
    return "Couldn't open resource file";
  }
}

QString QxrdCenterFinder::defaultUserAbsorptionScript()
{
  QFile def(":/qxrdexampleuserabsorption.js");

  if (def.open(QFile::ReadOnly)) {
    return def.readAll();
  } else {
    return "Couldn't open resource file";
  }
}

void QxrdCenterFinder::undoRefinePowderFitParameters()
{
}

void QxrdCenterFinder::evaluatePowderFit(double *parm, double *x, int np, int nx)
{
}

static void fitPowderFit(double *p, double *x, int np, int nx, void *data)
{
}

void QxrdCenterFinder::refinePowderFitParameters()
{
  int options = get_PowderFitOptions();
  int nMarked = get_MarkedPoints().count();
  int nRings  = get_RingAngles().count();

  QVector<double> parms(6+nRings);
  int np=0;

  parms[0] = get_CenterX();
  parms[1] = get_CenterY();
  parms[2] = get_DetectorTilt();
  parms[3] = get_TiltPlaneRotation();

  if (options == 0) { // Keep energy and angles fixed, fit distance
    parms[4] = get_DetectorDistance();
    np = 5;
  } else if (options == 1) { // Keep distance and angles fixed, fit energy
    parms[4] = get_Energy();
    np = 5;
  } else if (options == 2) { // Keep distance and energy fixed, fit angles
    for (int i=0; i<nRings; i++) {
      parms[4+i] = get_RingAngles()[i];
    }
    np = 4+nRings;
  }

  double info[LM_INFO_SZ];

  int niter = dlevmar_dif(fitPowderFit, parms.data(), NULL, np, nMarked, 200, NULL, info, NULL, NULL, this);

  int update = false;
  QString message;
  if (niter >= 0) {
    message.append(tr("Fitting succeeded after %1 iterations\n").arg(niter));
    message.append(tr("New Center = [%1,%2]").arg(parms[0]).arg(parms[1]));
    message.append(tr("Tilt Angle %1, Tilt Plane Rotation %2\n").arg(parms[2]).arg(parms[3]));

    if (options == 0) {
      message.append(tr("Detector Distance %1\n").arg(parms[4]));
    } else if (options == 1) {
      message.append(tr("Energy %1\n").arg(parms[4]));
    } else if (options == 3) {
      for (int i=0; i<nRings; i++) {
        message.append(tr("Ring %1: Angle %2").arg(i).arg(parms[4+i]));
      }
    }
  } else {
    message.append(tr("dlevmar_dif failed: reason = %1, %2").arg(info[6]).arg(levmarFailureReason(info[6])));
  }

  printMessage(message);

//  if (g_Application->get_GuiWanted()) {
//    if (niter >= 0) {
//      message.append("Do you want to update the calibration factors?");

//      if (QMessageBox::question(NULL, "Update calibration?", message, QMessageBox::Ok | QMessageBox::No, QMessageBox::No) == QMessageBox::Ok) {
//        update = true;
//      }
//    } else {
//      QMessageBox::information(NULL, "Fitting Failed", message);
//    }
//  }

  if (update) {
    set_CenterX(parms[0]);
    set_CenterY(parms[1]);
    set_DetectorTilt(parms[2]);
    set_TiltPlaneRotation(parms[3]);

    if (options == 0) {
      set_DetectorDistance(parms[4]);
    } else if (options == 1) {
      set_Energy(parms[4]);
    } else if (options == 2) {
      QcepDoubleVector angles;

      for (int i=0; i<nRings; i++) {
        angles.append(parms[4+i]);
      }

      set_RingAngles(angles);
    }
  }
}

QString QxrdCenterFinder::levmarFailureReason(int n)
{
  switch (n) {
  case 1:
    return "Stopped by Small Gradient";
    break;

  case 2:
    return "Stopped by Small Dp";
    break;

  case 3:
    return "Stopped by iteration limit";
    break;

  case 4:
    return "Stopped by singular matrix";
    break;

  case 5:
    return "No further error reduction possible";
    break;

  case 6:
    return "Stopped by small ||e||^2";
    break;

  case 7:
    return "Stopped by invalid (i.e. NaN or Inf) function values";
    break;

  default:
    return "Unknown reason for failure";
  }
}

int QxrdCenterFinder::countPowderRings() const
{
  int max = 0;

  int n = m_RingPowderPoints.count();

  for (int i=0; i<n; i++) {
    QxrdPowderPoint pt = m_RingPowderPoints.value(i);

    if (pt.n2() && pt.n1()>max) {
      max = pt.n1();
    }
  }

  return max;
}

int QxrdCenterFinder::countPowderRingPoints() const
{
  return m_RingPowderPoints.count();
}

QxrdPowderPoint QxrdCenterFinder::powderRingPoint(int i) const
{
  return m_RingPowderPoints.value(i);
}
