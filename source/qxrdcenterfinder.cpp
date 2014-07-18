#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qxrdmutexlocker.h"
#include "levmar.h"
#include <QMessageBox>
#include "qxrdapplication.h"

QxrdCenterFinder::QxrdCenterFinder(QxrdSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
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
    m_PeakFitDebug(saver, this, "peakFitDebug", 0, "Debug Print for peak fitting"),
    m_RingAngles(saver, this, "ringAngles", QcepDoubleVector(), "Diffraction ring angles"),
    m_RingAngleTolerance(saver, this, "ringAngleToleance", 0.1, "Diffraction ring angle tolerance"),
    m_PowderFitOptions(saver, this, "powderFitOptions", 0, "Powder fitting options"),
    m_PowderPoint(saver, this, "powderPoint", QxrdPowderPoint(1,2,3,4), "Powder Point"),
    m_PowderPointVector(saver, this, "powderPointVector", QxrdPowderPointVector(), "Powder Point Vector"),
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

static int firstFit=0;

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

      if (firstFit) {
        printMessage(tr("%1\t%2\t%3\t%4\t%5").arg(x).arg(y).arg(pk).arg(d).arg(pk-d));
      }
    }

    if (firstFit) {
      printMessage("\n");
    }
  }

  if (firstFit) {
    firstFit--;
  }
}

static void fitPeak(double *p, double *hx, int m,int n, void *adata)
{
  QxrdCenterFinder *cf = (QxrdCenterFinder*) adata;

  if (cf) {
    cf->evaluatePeakFit(p, hx, m, n);
  }
}

//bool QxrdCenterFinder::fitPeakNear()
//{
//  return fitPeakNear(get_PeakCenterX(), get_PeakCenterY());
//}

bool QxrdCenterFinder::fitPeakNear(double x, double y, int nitermax)
{
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

  firstFit = get_PeakFitDebug();

  int niter = dlevmar_dif(fitPeak, parms, NULL, 7, n*n, nitermax, NULL, info, NULL, NULL, this);

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
  double r  = parm[1];
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
      double pk = bg + dx*bx + dy*by + ht*exp(-((dr*dr)/(2.0*r*r)));

      xv[i++] = pk - d;

      if (firstFit) {
        printMessage(tr("%1\t%2\t%3\t%4\t%5").arg(x).arg(y).arg(pk).arg(d).arg(pk-d));
      }
    }

    if (firstFit) {
      printMessage("\n");
    }
  }

  if (firstFit) {
    firstFit--;
  }
}

static void fitRing(double *p, double *hx, int m,int n, void *adata)
{
  QxrdCenterFinder *cf = (QxrdCenterFinder*) adata;

  if (cf) {
    cf->evaluateRingFit(p, hx, m, n);
  }
}

bool QxrdCenterFinder::fitRingNear(double x0, double y0, int nitermax)
{
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

  firstFit = get_PeakFitDebug();

  int niter = dlevmar_dif(fitRing, parms, NULL, 6, n*n, nitermax, NULL, info, NULL, NULL, this);

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

bool QxrdCenterFinder::traceRingNear(double x0, double y0, double step, int nitermax)
{
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

  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

  double pkht = imageValue(x0,y0) - bkgd;

  while (true) {
    if (fitRingNear(x,y, nitermax)) {

      x = get_PeakCenterX();
      y = get_PeakCenterY();

      if ((get_PeakHeight() > (pkht*0.25)) &&
          (fabs(get_PeakRadius()) < 10.0)){
        appendPowderPoint(x, y);
        r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
      }
    }

    az += ast;

    if (step > 0) {
      if ((az-az0) >= 2*M_PI) return true;
    } else {
      if ((az-az0) <= -2*M_PI) return true;
    }

    x = xc + r*cos(az);
    y = yc + r*sin(az);
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
