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
#include "qxrdfitterpeakpoint.h"
#include "qxrdfitterringpoint.h"
#include "qxrdfitterringcircle.h"
#include "qxrdfitterringellipse.h"
#include <QVector>

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
    m_MarkedPoints(saver, this, "markedPoints", QxrdPowderPointVector(), "Marker Points"),
    m_FittedRings(saver, this, "fittedRings", QxrdPowderPointVector(), "Fitted Powder Rings"),
    m_RingRadius(saver, this, "ringRadius", 0.0, "Estimated Powder Ring Radius"),
    m_RingRadiusA(saver, this, "ringRadiusA", 0.0, "Estimated Powder Ellipse Major Axis Radius"),
    m_RingRadiusB(saver, this, "ringRadiusB", 0.0, "Estimated Powder Ellipse Minor Axis Radius"),
    m_RingRotation(saver, this, "ringRotation", 0.0, "Estimated Powder Ellipse Major Axis Rotation"),
    m_PeakFitRadius(saver, this, "peakFitRadius", 10, "Half size of fitted area for peak fitting"),
    m_PeakHeight(saver, this, "peakHeight", 100.0, "Height of fitted peak"),
    m_PeakCenterX(saver, this, "peakCenterX", 0, "X Center of fitted peak"),
    m_PeakCenterY(saver, this, "peakCenterY", 0, "Y Center of fitted peak"),
    m_PeakWidth(saver, this, "peakWidth", 2.0, "Width of fitted peak"),
    m_PeakBackground(saver, this, "peakBackground", 0, "Background Height of fitted peak"),
    m_PeakBackgroundX(saver, this, "peakBackgroundX", 0, "X Slope of Background"),
    m_PeakBackgroundY(saver, this, "peakBackgroundY", 0, "Y Slope of Background"),
    m_PeakFitDebug(saver, this, "peakFitDebug", false, "Debug Print for peak fitting"),
    m_PeakFitIterations(saver, this, "peakFitIterations", 200, "Max Iterations during fitting"),
    m_RingAngles(saver, this, "ringAngles", QcepDoubleVector(), "Diffraction ring angles"),
    m_RingAngleTolerance(saver, this, "ringAngleToleance", 0.1, "Diffraction ring angle tolerance"),
    m_PowderFitOptions(saver, this, "powderFitOptions", 0, "Powder fitting options"),
    m_RingIndex(saver, this, "ringIndex", 0, "Fitted Powder Ring Index"),
    m_SubtractRingAverages(saver, this, "subtractRingAverages", false, "Plot deviations of each ring from average"),
    m_FittedWidthMin(saver, this, "fittedWidthMin", 0.5, "Minimum acceptable fitted width (pixels)"),
    m_FittedWidthMax(saver, this, "fittedWidthMax", 3.0, "Maximum acceptable fitted width (pixels)"),
    m_FittedHeightMinRatio(saver, this, "fittedHeightMinRatio", 0.25, "Minimum acceptable peak height ratio"),
    m_FittedPositionMaxDistance(saver, this, "fittedPositionMaxDistance", 2.0, "Maximum acceptable fitted position shift (pixels)"),
    m_Experiment(expt)
{
  qRegisterMetaType<QPointF>("QPointF");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);

  connect(prop_CenterX(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_CenterY(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_DetectorXPixelSize(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_DetectorYPixelSize(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_DetectorDistance(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_ImplementTilt(), SIGNAL(valueChanged(bool,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_DetectorTilt(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
  connect(prop_TiltPlaneRotation(), SIGNAL(valueChanged(double,int)), this, SIGNAL(parameterChanged()), Qt::DirectConnection);
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
  fitPeakNear(pt.x(), pt.y());
}

void QxrdCenterFinder::printMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdCenterFinder::statusMessage(QString msg, QDateTime ts)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg);
  }
}

void QxrdCenterFinder::fitPowderCircle(int n)
{
  QxrdFitterRingCircle fitter(this, n, get_CenterX(), get_CenterY());

  int niter = fitter.fit();

  int update = false;
  QString message;

  if (fitter.reason() == QxrdFitter::Successful) {
    message.append(tr("Circle Fitting Succeeded after %1 iterations\n").arg(niter));
    message.append(tr("Old Center = [%1,%2]\n").arg(get_CenterX()).arg(get_CenterY()));
    message.append(tr("New Center = [%1,%2], New Radius = %3\n").arg(fitter.fittedX()).arg(fitter.fittedY()).arg(fitter.fittedR()));
    double dx = fitter.fittedX() - get_CenterX();
    double dy = fitter.fittedY() - get_CenterY();
    message.append(tr("Moved by [%1,%2] = %3\n").arg(dx).arg(dy).arg(sqrt(dx*dx + dy*dy)));
  } else {
    message.append(tr("Circle Fitting Failed: Reason = %1\n").arg(fitter.reasonString()));
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
    set_CenterX(fitter.fittedX());
    set_CenterY(fitter.fittedY());
    set_RingRadius(fitter.fittedR());
  }
}

void QxrdCenterFinder::fitPowderEllipse(int n)
{
  QxrdFitterRingEllipse fitter(this, n, get_CenterX(), get_CenterY());

  int niter = fitter.fit();

  int update = false;
  QString message;

  if (fitter.reason() == QxrdFitter::Successful) {
    message.append(tr("Ellipse Fitting Succeeded after %1 iterations\n").arg(niter));
    message.append(tr("Old Center = [%1,%2]\n").arg(get_CenterX()).arg(get_CenterY()));
    message.append(tr("New Center = [%1,%2], New Radii = %3,%4\n")
                   .arg(fitter.fittedX()).arg(fitter.fittedY()).arg(fitter.fittedA()).arg(fitter.fittedB()));
    message.append(tr("Major Axis rotation = %1 deg\n").arg(fitter.fittedRot()*180.0/M_PI));

    double dx = fitter.fittedX() - get_CenterX();
    double dy = fitter.fittedY() - get_CenterY();
    message.append(tr("Moved by [%1,%2] = %3\n").arg(dx).arg(dy).arg(sqrt(dx*dx + dy*dy)));
  } else {
    message.append(tr("Ellipse Fitting Failed: Reason = %1\n").arg(fitter.reasonString()));
  }

  printMessage(message);

  if (g_Application->get_GuiWanted()) {
    if (fitter.reason() == QxrdFitter::Successful) {
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
    set_CenterX(fitter.fittedX());
    set_CenterY(fitter.fittedY());
    set_RingRadiusA(fitter.fittedA());
    set_RingRadiusB(fitter.fittedB());
    set_RingRotation(fitter.fittedRot());
  }
}

void QxrdCenterFinder::fitPowderEllipses()
{
  int nrings = countPowderRings();

  QVector<QxrdFitterRingEllipse> fits;

  for (int i=0; i<nrings; i++) {
    fits.append(QxrdFitterRingEllipse(this, i, get_CenterX(), get_CenterY()));
  }

  if (qcepDebug(DEBUG_NOPARALLEL)) {
    for (int i=0; i<nrings; i++) {
      fits[i].fit();
    }
  } else {
    QFuture<void> fitDone = QtConcurrent::map(fits, &QxrdFitterRingEllipse::fit);

    fitDone.waitForFinished();
  }

  QxrdPowderPointVector pts;

  for (int i=0; i<nrings; i++) {
    QxrdFitterRingEllipse &r = fits[i];

    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
      printMessage(tr("Fitted Ring %1: x: %2, y: %3, a: %4, b: %5, rot: %6, rzn: %7")
                   .arg(i).arg(r.fittedX()).arg(r.fittedY())
                   .arg(r.fittedA()).arg(r.fittedB()).arg(r.fittedRot())
                   .arg(r.reasonString()));
    }

    if (r.reason() == QxrdFitter::Successful) {
      pts.append(QxrdPowderPoint(i, 0, r.fittedX(), r.fittedY(), r.fittedA(), r.fittedB(), r.fittedRot()));
    }
  }

  set_FittedRings(pts);
}

QxrdPowderPoint QxrdCenterFinder::powderPoint(int i)
{
  return get_MarkedPoints().value(i);
}

int QxrdCenterFinder::nearestPowderPointIndex(double x, double y)
{
  int nearest = -1;
  double nearestDist;
  QxrdPowderPointVector pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    QxrdPowderPoint pt = pts.value(i);
    double dist = sqrt(pow(x-pt.x(), 2) + pow(y-pt.y(), 2));

    if (nearest == -1 || dist < nearestDist) {
      nearest     = i;
      nearestDist = dist;
    }
  }

  return nearest;
}

QxrdPowderPoint QxrdCenterFinder::nearestPowderPoint(double x, double y)
{
  return get_MarkedPoints().value(nearestPowderPointIndex(x,y));
}

void QxrdCenterFinder::deletePowderPointNear(double x, double y)
{
  int nearest = nearestPowderPointIndex(x, y);

  if (nearest >= 0) {
    QxrdPowderPointVector pts = get_MarkedPoints();

    pts.remove(nearest);

    set_MarkedPoints(pts);
  }
}

void QxrdCenterFinder::appendPowderPoint(double x, double y)
{
  m_MarkedPoints.appendValue(QxrdPowderPoint(get_RingIndex(), 0, x,y, 0,0,0));
}

void QxrdCenterFinder::deletePowderRing(int n)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  QxrdPowderPointVector res;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() < n) {
      res.append(pt);
    } else if (pt.n1() > n) {
      res.append(QxrdPowderPoint(pt.n1()-1, pt.n2(), pt.x(), pt.y(), pt.r1(), pt.r2(), pt.az()));
    }
  }

  set_MarkedPoints(res);
}

void QxrdCenterFinder::deletePowderPoints()
{
  m_MarkedPoints.clear();

  set_RingIndex(0);
}

void QxrdCenterFinder::normalizePowderRings()
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  double maxR = 0;

  foreach(QxrdPowderPoint pt, pts) {
    double r = getR(pt.x(), pt.y());

    if (r > maxR) {
      maxR = r;
    }
  }

  if (maxR > 0 && maxR < 10000) {
    int nbins = (int) maxR + 5;
    QVector<int> npts(nbins);

    npts.fill(0);

    foreach(QxrdPowderPoint pt, pts) {
      double r = getR(pt.x(), pt.y());
      npts[(int) r] ++;
    }

    int ringIndex = 0;

    for (int i=0; i<nbins-1; i++) {
      if (npts[i] > 0) {
        if (npts[i+1] == 0) {
          npts[i] = ringIndex++;
        } else {
          npts[i] = ringIndex;
        }
      }
    }

    for (int i=0; i<nbins; i++) {
      if (npts[i] > 0) {
        printMessage(tr("i: %1, r: %2").arg(i).arg(npts[i]));
      }
    }

    int n = pts.count();

    for (int i=0; i<n; i++) {
      QxrdPowderPoint &pt = pts[i];

      double r = getR(pt.x(), pt.y());
      int idx = npts[(int) r];

      pt.n1() = idx;
    }

    set_MarkedPoints(pts);
    set_RingIndex(ringIndex);
  }
}

bool QxrdCenterFinder::fitPeakNear(double x, double y)
{
  printMessage(tr("centering.fitPeakNear(%1,%2)").arg(x).arg(y));

  QxrdFitterPeakPoint fit(this, 0, x, y, 0, 0);
  fit.fit();

  if (fit.reason() == QxrdFitter::Successful) {
    set_PeakCenterX(fit.fittedX());
    set_PeakCenterY(fit.fittedY());
    set_PeakWidth(fit.fittedWidth());
    set_PeakHeight(fit.fittedHeight());
    set_PeakBackground(fit.fittedBkgd());
    set_PeakBackgroundX(fit.fittedBkgdX());
    set_PeakBackgroundY(fit.fittedBkgdY());

    appendPowderPoint(fit.fittedX(), fit.fittedY());

    return true;
  } else {
    printMessage(tr("Fitting Failed"));

    return false;
  }
}

bool QxrdCenterFinder::fitRingNear(double x0, double y0)
{
  printMessage(tr("centering.fitRingNear(%1,%2)").arg(x0).arg(y0));

  QxrdFitterRingPoint fit(this, 0, x0, y0, 0, 0);
  fit.fit();

  if (fit.reason() == QxrdFitter::Successful) {
    set_PeakCenterX(fit.fittedX());
    set_PeakCenterY(fit.fittedY());
    set_PeakWidth(fit.fittedWidth());
    set_PeakHeight(fit.fittedHeight());
    set_PeakBackground(fit.fittedBkgd());
    set_PeakBackgroundX(fit.fittedBkgdX());
    set_PeakBackgroundY(fit.fittedBkgdY());

    appendPowderPoint(fit.fittedX(), fit.fittedY());

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

  QxrdFitterRingPoint fit(this, 0, x0, y0, 0, 0);
  fit.fit();

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

  QVector<QxrdFitterRingPoint> fits;

  while (true) {
    if (x >= 0 && y >= 0 && x <= width && y <= width) {
      QxrdFitterRingPoint fit(this, 0, x, y, pkht, bkgd);
      fit.fit();
      fits.append(fit);

      if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
        printMessage(tr("Fit Ring Near: az: %1, x: %2, y: %3, nx: %4, ny: %5, wd: %6, ht: %7, rz: %8")
                     .arg(az).arg(x).arg(y).arg(fit.fittedX()).arg(fit.fittedY())
                     .arg(fit.fittedWidth()).arg(fit.fittedHeight())
                     .arg(fit.reasonString()));
      }

      x = fit.fittedX();
      y = fit.fittedY();

      if (fit.reason()==QxrdFitter::Successful) {
        r = sqrt((x-xc)*(x-xc) + (y-yc)*(y-yc));
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

  int npts = fits.count();
  int nok  = 0;

  QxrdPowderPointVector pts = get_MarkedPoints();

  for(int i=0; i<fits.count(); i++) {
    QxrdFitterRingPoint &fit = fits[i];

    if (fit.reason() == QxrdFitter::Successful) {
      nok += 1;
      pts.append(QxrdPowderPoint(get_RingIndex(), 0, fit.fittedX(), fit.fittedY(), fit.fittedR(), fit.fittedR(), fit.fittedAz()));
    }
  }

  if (npts) {
    QString msg(tr("centering.traceRingNear : %1/%2 fitted points").arg(nok).arg(npts));

    printMessage(msg);
    statusMessage(msg);

    if (nok) {
      prop_RingIndex()->incValue(1);
      set_MarkedPoints(pts);
    }
  }

  return nok;
}

//bool QxrdCenterFinder::traceRingNearParallel(double x0, double y0, double step)
//{
//  printMessage(tr("centering.traceRingNearParallel(%1,%2,%3)")
//               .arg(x0).arg(y0).arg(step));

//  double xc  = get_CenterX();
//  double yc  = get_CenterY();
//  double dx  = x0-xc;
//  double dy  = y0-yc;
//  double dr  = get_PeakFitRadius();
//  double r   = sqrt(dx*dx+dy*dy);
//  double az  = atan2(dy, dx);
//  double ast = step/r;

//  QxrdFitterRingPoint fit(this, 0, x0, y0, 0, 0);

//  fit.fit();

//  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
//                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

//  double pkht = imageValue(x0,y0) - bkgd;

//  int nsteps = (int) ((2.0*M_PI)/ast)+1;

//  QVector<QxrdFitterRingPoint> fits;

//  double tth = getTTH(x0, y0);
//  for (int i=0; i<nsteps; i++) {
//    double chi = ast*i*180.0/M_PI;
//    QPointF xy  = getXY(tth, chi);

//    fits.append(QxrdFitterRingPoint(this, i, xy.x(), xy.y(), pkht, bkgd));

////    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
////      printMessage(tr("Fitting i: %1, x0: %2, y0: %3").arg(i).arg(fits[i].x0()).arg(fits[i].y0()));
////    }
//  }

//  if (qcepDebug(DEBUG_NOPARALLEL)) {
//    for (int i=0; i<nsteps; i++) {
//      fits[i].fit();
//    }
//  } else {
//    QFuture<void> fitDone = QtConcurrent::map(fits, &QxrdFitterRingPoint::fit);

//    fitDone.waitForFinished();
//  }

//  int sums[6];

//  for (int i=0; i<6; i++) {
//    sums[i]=0;
//  }

//  QxrdPowderPointVector pts = get_MarkedPoints();

//  for (int i=0; i<nsteps; i++) {
//    QxrdFitterRingPoint &r = fits[i];

//    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
//      printMessage(tr("Fitted %1 : x %2, y %3, w %4, ht %5, bk %6, bkx %7, bky %8, rzn %9")
//                   .arg(i).arg(r.fittedX()).arg(r.fittedY())
//                   .arg(r.fittedWidth()).arg(r.fittedHeight())
//                   .arg(r.fittedBkgd()).arg(r.fittedBkgdX()).arg(r.fittedBkgdY())
//                   .arg(r.reasonString()));
////      printMessage(tr("tth %1, chi %2")
////                   .arg(getTTH(r.fittedX(), r.fittedY()))
////                   .arg(getChi(r.fittedX(), r.fittedY())));
//    }

//    int rz = r.reason();
//    if (rz>=0 && rz<6) {
//      sums[rz]++;
//    }

//    if (r.reason() == QxrdFitter::Successful) {
//      pts.append(QxrdPowderPoint(get_RingIndex(), 0, r.fittedX(), r.fittedY(), r.fittedR(), r.fittedR(), r.fittedAz()));
//    }
//  }

//  QString msg(tr("centering.traceRingNearParallel : Fitted %1/%2 : NR %3, OR %4, BdW %5, BdP %6, BdH %7")
//      .arg(sums[QxrdFitter::Successful])
//      .arg(nsteps)
//      .arg(sums[QxrdFitter::NoResult])
//      .arg(sums[QxrdFitter::OutsideData])
//      .arg(sums[QxrdFitter::BadWidth])
//      .arg(sums[QxrdFitter::BadPosition])
//      .arg(sums[QxrdFitter::BadHeight])
//      );

//  printMessage(msg);
//  statusMessage(msg);

//  if (sums[QxrdFitter::Successful]) {
//    prop_RingIndex()->incValue(1);
//    set_MarkedPoints(pts);
//  }

//  return true;
//}

int QxrdCenterFinder::getPowderPointN1(int i)
{
  QxrdPowderPoint res = get_MarkedPoints().value(i);

  return res.n1();
}

int QxrdCenterFinder::getPowderPointN2(int i)
{
  QxrdPowderPoint res = get_MarkedPoints().value(i);

  return res.n2();
}

double QxrdCenterFinder::getPowderPointX(int i)
{
  QxrdPowderPoint res = get_MarkedPoints().value(i);

  return res.x();
}

double QxrdCenterFinder::getPowderPointY(int i)
{
  QxrdPowderPoint res = get_MarkedPoints().value(i);

  return res.y();
}

void QxrdCenterFinder::setPowderPoint(int i, int n1, int n2, double x, double y, double r1, double r2, double az)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  if (i>=0 && i<pts.count()) {
    pts[i] = QxrdPowderPoint(n1,n2,x,y,r1,r2,az);
  } else {
    pts.append(QxrdPowderPoint(n1,n2,x,y,r1,r2,az));
  }

  set_MarkedPoints(pts);
}

QScriptValue QxrdCenterFinder::getPowderPoint(int i)
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    QxrdScriptEnginePtr eng(exp->scriptEngine());
    QxrdPowderPointVector pts = get_MarkedPoints();

    if (eng) {
      if (i>=0 && i<get_MarkedPoints().count()) {
        QScriptValue val = eng->newObject();

        QxrdPowderPoint &pt = pts[i];
        val.setProperty("n1", pt.n1());
        val.setProperty("n2", pt.n2());
        val.setProperty("x",  pt.x());
        val.setProperty("y",  pt.y());
        val.setProperty("r1", pt.r1());
        val.setProperty("r2", pt.r2());
        val.setProperty("az", pt.az());

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
      QxrdPowderPointVector pts = get_MarkedPoints();

      QScriptValue val = eng->newArray();

      for (int i=0; i<pts.count(); i++) {
        QScriptValue item = eng->newObject();

        QxrdPowderPoint &pt = pts[i];
        item.setProperty("n1", pt.n1());
        item.setProperty("n2", pt.n2());
        item.setProperty("x", pt.x());
        item.setProperty("y", pt.y());
        item.setProperty("r1", pt.r1());
        item.setProperty("r2", pt.r2());
        item.setProperty("az", pt.az());

        val.setProperty(tr("%1").arg(i), item);
      }

      return val;
    }
  }

  return QScriptValue();
}

void         QxrdCenterFinder::setPowderPoint(int i, QScriptValue val)
{
  int   n1 = val.property("n1").toInteger();
  int   n2 = val.property("n2").toInteger();
  double x = val.property("x").toNumber();
  double y = val.property("y").toNumber();
  double r1 = val.property("r1").toNumber();
  double r2 = val.property("r2").toNumber();
  double az = val.property("az").toNumber();

  setPowderPoint(i, n1, n2, x, y, r1, r2, az);
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
  int max = -1;

  QxrdPowderPointVector pts = get_MarkedPoints();

  int n = pts.count();

  for (int i=0; i<n; i++) {
    QxrdPowderPoint pt = pts.value(i);

    if (pt.n1()>max) {
      max = pt.n1();
    }
  }

  if (max>1000) {
    return 1000;
  } else {
    return max+1;
  }
}

int QxrdCenterFinder::countPowderRingPoints() const
{
  return get_MarkedPoints().count();
}

int QxrdCenterFinder::countPowderRingPoints(int r) const
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  int sum = 0;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() == r || r < 0) {
      sum += 1;
    }
  }

  return sum;
}

QxrdPowderPoint QxrdCenterFinder::powderRingPoint(int i) const
{
  return get_MarkedPoints().value(i);
}

QxrdPowderPoint QxrdCenterFinder::powderRingPoint(int r, int i) const
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  int sum = 0;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() == r || r < 0) {
      if (sum == i) {
        return pt;
      } else {
        sum += 1;
      }
    }
  }

  return QxrdPowderPoint();
}

double QxrdCenterFinder::powderRingAverageR(int r) const
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() == r) {
      npts += 1;
      sum  += getR(pt.x(), pt.y());
    }
  }

  return sum/npts;
}

double QxrdCenterFinder::powderRingAverageQ(int r) const
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() == r) {
      npts += 1;
      sum  += getQ(pt.x(), pt.y());
    }
  }

  return sum/npts;
}

double QxrdCenterFinder::powderRingAverageTTH(int r) const
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() == r) {
      npts += 1;
      sum  += getTTH(pt.x(), pt.y());
    }
  }

  return sum/npts;
}
