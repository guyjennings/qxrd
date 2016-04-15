#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>
#include "qcepmutexlocker.h"
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
#include "triangulate.h"
#include "qxrdplanefitter.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qxrdcalibrantlibrary.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QxrdCenterFinder::QxrdCenterFinder(QcepSettingsSaverWPtr saver, QxrdExperimentWPtr expt)
  : QxrdDetectorGeometry("centering", NULL),
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
    m_RingAngleTolerance(saver, this, "ringAngleTolerance", 0.1, "Diffraction ring angle tolerance"),
    m_PowderFitOptions(saver, this, "powderFitOptions", 0, "Powder fitting options"),
    m_RingIndex(saver, this, "ringIndex", 0, "Fitted Powder Ring Index"),
    m_SubtractRingAverages(saver, this, "subtractRingAverages", false, "Plot deviations of each ring from average"),
    m_RingAverageDisplacement(saver, this, "ringAverageDisplacement", 0.0, "Extra displacement between curves in ring radius plot"),
    m_FittedWidthMin(saver, this, "fittedWidthMin", 0.5, "Minimum acceptable fitted width (pixels)"),
    m_FittedWidthMax(saver, this, "fittedWidthMax", 3.0, "Maximum acceptable fitted width (pixels)"),
    m_FittedHeightMinRatio(saver, this, "fittedHeightMinRatio", 0.25, "Minimum acceptable peak height ratio"),
    m_FittedPositionMaxDistance(saver, this, "fittedPositionMaxDistance", 2.0, "Maximum acceptable fitted position shift (pixels)"),
    m_FitPowderPointPosition(saver, this, "fitPowderPointPosition", true, "Fit to nearby peak when adding powder points individually"),
    m_Experiment(expt)
{
#ifndef QT_NO_DEBUG
  printf("Constructing center finder\n");
#endif

  qRegisterMetaType<QPointF>("QPointF");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);

  connect(prop_CenterX(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_CenterY(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorXPixelSize(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorYPixelSize(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorDistance(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_Energy(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_ImplementTilt(), &QcepBoolProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorTilt(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_TiltPlaneRotation(), &QcepDoubleProperty::valueChanged, this, &QxrdCenterFinder::parameterChanged, Qt::DirectConnection);
}

QxrdCenterFinder::~QxrdCenterFinder()
{
#ifndef QT_NO_DEBUG
  printf("Deleting center finder\n");
#endif
}

QxrdExperimentWPtr QxrdCenterFinder::experiment() const
{
  return m_Experiment;
}

QcepDoubleImageDataPtr QxrdCenterFinder::data() const
{
  return m_Data;
}

QcepDoubleImageDataPtr QxrdCenterFinder::newData()
{
  int wd, ht;

  if (m_Data) {
    wd = m_Data->get_Width();
    ht = m_Data->get_Height();
  } else {
    wd = 2048;
    ht = 2048;
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());

    if (proc) {
      QcepDoubleImageDataPtr res =
          QcepAllocator::newDoubleImage("newData", wd,ht, QcepAllocator::NullIfNotAvailable);

      return res;
    }
  }

  return QcepDoubleImageDataPtr();
}

void QxrdCenterFinder::writeSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QxrdDetectorGeometry::writeSettings(settings, section);
}

void QxrdCenterFinder::readSettings(QSettings *settings, QString section)
{
  QcepMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QxrdDetectorGeometry::readSettings(settings, section);
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
  if (get_FitPowderPointPosition()) {
    fitPeakNear(pt.x(), pt.y());
  } else {
    appendPowderPoint(pt.x(), pt.y());
  }
}

void QxrdCenterFinder::printMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->printMessage(msg, ts);
  }
}

void QxrdCenterFinder::statusMessage(QString msg, QDateTime ts) const
{
  QxrdExperimentPtr exp(m_Experiment);

  if (exp) {
    exp->statusMessage(msg, ts);
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

  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app && app->get_GuiWanted()) {
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

  QxrdApplication *app = qobject_cast<QxrdApplication*>(g_Application);

  if (app && app->get_GuiWanted()) {
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
      pts.append(QxrdPowderPoint(i, 0, 0, r.fittedX(), r.fittedY(), r.fittedA(), r.fittedB(), r.fittedRot()));
    }
  }

  set_FittedRings(pts);
}

void QxrdCenterFinder::adjustEnergy(int n)
{
  double tth    = powderRingAverageTTH(n);
  double th     = tth/2.0;
  double dlatt  = calibrantDSpacing(n);
  double caltth = calibrantTTH(n);

  double newLambda = 2.0*dlatt*sin(th*M_PI/180.0);
  double newEnergy = 12398.4187/newLambda;

  QString message;

  message.append(tr("Ring %1 average TTH = %2\n").arg(n).arg(tth));
  message.append(tr("Calibrant Peak %1 TTH = %2\n").arg(n).arg(caltth));
  message.append(tr("Adjust Energy from %1 to %2\n").arg(get_Energy()).arg(newEnergy));
  message.append(tr("to improve fit to ring %1").arg(n));

  if (QMessageBox::question(NULL, "Update Energy?",
                            message, QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok)
      == QMessageBox::Ok) {
    set_Energy(newEnergy);
  }
}

void QxrdCenterFinder::adjustDistance(int n)
{
  double r = powderRingAverageR(n);
  double tth = calibrantTTH(n);
  double d = r/tan(tth*M_PI/180.0);

  QString message;

  message.append(tr("Ring %1 average radius = %2\n").arg(n).arg(r));
  message.append(tr("Calibrant Peak %1 TTH = %2\n").arg(n).arg(tth));
  message.append(tr("Adjust sample - detector distance from %1 to %2\n")
                 .arg(get_DetectorDistance()).arg(d));
  message.append(tr("to improve fit to ring %1").arg(n));

  if (QMessageBox::question(NULL, "Update Detector Distance?",
                            message, QMessageBox::Ok | QMessageBox::No, QMessageBox::Ok)
      == QMessageBox::Ok) {
    set_DetectorDistance(d);
  }
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
  m_MarkedPoints.appendValue(QxrdPowderPoint(get_RingIndex(), 0, 0, x,y, 0,0,0));
}

void QxrdCenterFinder::appendPowderPoint(int n1, int n2, int n3, double x, double y, double r1, double r2, double az)
{
  m_MarkedPoints.appendValue(QxrdPowderPoint(n1,n2,n3, x, y, r1, r2, az));
}

void QxrdCenterFinder::deletePowderRing(int n)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  QxrdPowderPointVector res;

  foreach (QxrdPowderPoint pt, pts) {
    if (pt.n1() < n) {
      res.append(pt);
    } else if (pt.n1() > n) {
      res.append(QxrdPowderPoint(pt.n1()-1, pt.n2(), pt.n3(), pt.x(), pt.y(), pt.r1(), pt.r2(), pt.az()));
    }
  }

  set_MarkedPoints(res);
}

void QxrdCenterFinder::disablePowderRing(int n)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  int np = 0;

  for (int i=0; i<pts.count(); i++) {
    if (pts[i].n1() == n) {
      pts[i].n2() = -1;
      np++;
    }
  }

  printMessage(tr("Disabled %1 points in ring %2").arg(np).arg(n));

  set_MarkedPoints(pts);
}

void QxrdCenterFinder::enablePowderRing(int n)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  int np=0;

  for (int i=0; i<pts.count(); i++) {
    if (pts[i].n1() == n) {
      pts[i].n2() = np++;
    }
  }

  printMessage(tr("Enabled %1 points in ring %2").arg(np).arg(n));

  set_MarkedPoints(pts);
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

//    for (int i=0; i<nbins; i++) {
//      if (npts[i] > 0) {
//        printMessage(tr("i: %1, r: %2").arg(i).arg(npts[i]));
//      }
//    }

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
    if (x >= 0 && y >= 0 && x <= width && y <= height) {
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

  QVector<int> nreason(QxrdFitter::LastReason);

  for(int i=0; i<fits.count(); i++) {
    QxrdFitterRingPoint &fit = fits[i];

    if (fit.reason() == QxrdFitter::Successful) {
      nok += 1;
      pts.append(QxrdPowderPoint(get_RingIndex(), 0, 0, fit.fittedX(), fit.fittedY(), fit.fittedR(), fit.fittedR(), fit.fittedAz()));
    }

    nreason[fit.reason()] += 1;
  }

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QcepDatasetModelPtr data = expt->dataset();

    if (data) {
      QcepDataColumnScanPtr scan = data->columnScan(tr("fitted/ring%1").arg(get_RingIndex()));

      if (scan == NULL) {
        scan = data->newColumnScan(tr("fitted/ring%1").arg(get_RingIndex()));
      }

      if (scan) {
        scan->clear();

        scan->appendColumn("i");
        scan->appendColumn("index");
        scan->appendColumn("Reason");
        scan->appendColumn("X0");
        scan->appendColumn("Y0");
        scan->appendColumn("pkht");
        scan->appendColumn("bkgd");
        scan->appendColumn("XFit");
        scan->appendColumn("YFit");
        scan->appendColumn("WdFit");
        scan->appendColumn("HtFit");
        scan->appendColumn("BkgdFit");
        scan->appendColumn("BkgdXFit");
        scan->appendColumn("BkgdYFit");
        scan->appendColumn("RFit");
        scan->appendColumn("AzFit");
        scan->appendColumn("dx");
        scan->appendColumn("dy");
        scan->appendColumn("dr");

        scan->resizeRows(fits.count());
        scan->column(2)->setFormatter(&QxrdFitter::reasonString);

        for (int i=0; i<fits.count(); i++) {
          QxrdFitterRingPoint &fit = fits[i];
          int col=0;

          double dx = fit.fittedX() - fit.x0();
          double dy = fit.fittedY() - fit.y0();
          double dr = sqrt(dx*dx + dy*dy);

          scan->setValue(col++, i, i);
          scan->setValue(col++, i, fit.index());
          scan->setValue(col++, i, fit.reason());
          scan->setValue(col++, i, fit.x0());
          scan->setValue(col++, i, fit.y0());
          scan->setValue(col++, i, fit.pkht());
          scan->setValue(col++, i, fit.bkgd());
          scan->setValue(col++, i, fit.fittedX());
          scan->setValue(col++, i, fit.fittedY());
          scan->setValue(col++, i, fit.fittedWidth());
          scan->setValue(col++, i, fit.fittedHeight());
          scan->setValue(col++, i, fit.fittedBkgd());
          scan->setValue(col++, i, fit.fittedBkgdX());
          scan->setValue(col++, i, fit.fittedBkgdY());
          scan->setValue(col++, i, fit.fittedR());
          scan->setValue(col++, i, fit.fittedAz()*180.0/M_PI);
          scan->setValue(col++, i, dx);
          scan->setValue(col++, i, dy);
          scan->setValue(col++, i, dr);
        }
      }
    }
  }

  if (npts) {
    QString msg(tr("centering.traceRingNear : %1/%2 fitted points").arg(nok).arg(npts));

    printMessage(msg);
    statusMessage(msg);

    for (int i=0; i<QxrdFitter::LastReason; i++) {
      if (nreason[i] > 0) {
        QString msg(tr("  %1 : %2").arg(nreason[i]).arg(QxrdFitter::reasonString((QxrdFitter::FitResult) i)));
        printMessage(msg);
      }
    }

    if (nok) {
      prop_RingIndex()->incValue(1);
      set_MarkedPoints(pts);
    }
  }

  return nok;
}

bool QxrdCenterFinder::missingRingNear(double x, double y)
{
  appendPowderPoint(get_RingIndex(), -1, 0, x, y, getR(x,y), getR(x,y), getChi(x,y));
  prop_RingIndex()->incValue(1);

  return true;
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

void QxrdCenterFinder::setPowderPoint(int i, int n1, int n2, int n3, double x, double y, double r1, double r2, double az)
{
  QxrdPowderPointVector pts = get_MarkedPoints();

  if (i>=0 && i<pts.count()) {
    pts[i] = QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az);
  } else {
    pts.append(QxrdPowderPoint(n1,n2,n3,x,y,r1,r2,az));
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
  int   n3 = val.property("n3").toInteger();
  double x = val.property("x").toNumber();
  double y = val.property("y").toNumber();
  double r1 = val.property("r1").toNumber();
  double r2 = val.property("r2").toNumber();
  double az = val.property("az").toNumber();

  setPowderPoint(i, n1, n2, n3, x, y, r1, r2, az);
}


void QxrdCenterFinder::setData(QcepDoubleImageDataPtr data)
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

class QuadInt {
public:
  QuadInt(int n=0, int h=-1, int k=-1, int l=-1) : m_N(n), m_H(h), m_K(k), m_L(l) {}

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

//void QxrdCenterFinder::updateCalibrantDSpacings()
//{
//  double a = get_CalibrantLattice();
//  int s = get_CalibrantSymmetry();
//  double lambda = 12398.4187/get_Energy();

//  int mmax = 2.0*a/lambda + 1;

//  if (qcepDebug(DEBUG_CALIBRANT)) {
//    printMessage(tr("mmax = %1").arg(mmax));
//  }

//  QVector<QuadInt> ex(mmax*mmax);

//  for (int h=1; h<=mmax; h++) {
//    for (int k=0; k<=h; k++) {
//      for (int l=0; l<=k; l++) {
//        int r = h*h+k*k+l*l;

//        if (r < mmax*mmax) {
//          bool ok=false;

//          switch (s) {
//          case 0:  // Simple cubic - all OK
//            ok = true;
//            break;

//          case 1:  // BCC h+k+l even
//            ok = ((h + k + l) % 2) == 0;
//            break;

//          case 2:  // FCC h,k,l all even or all odd
//            int n = h%2 + k%2 + l%2;
//            ok = (n==0) || (n==3);
//            break;
//          }

//          if (ok) {
//            if (ex[r].n() == 0) {
//              ex[r] = QuadInt(1, h,k,l);
//            } else {
//              ex[r].n()++;
//            }
//          }
//        }
//      }
//    }
//  }

//  QxrdPowderPointVector pts;

//  for (int i=1; i<mmax*mmax; i++) {
//    QuadInt e = ex[i];
//    if (e.n() > 0) {
//      double d = a/sqrt(i);
//      double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

//      if (tth <= 180) {
//        pts.append(QxrdPowderPoint(e.h(), e.k(), e.l(), d, tth, 0, 0, 0));

//        if (qcepDebug(DEBUG_CALIBRANT)) {
//          printMessage(tr("%1(%2): [%3,%4,%5], d:%6, tth:%7").arg(i).arg(e.n()).arg(e.h()).arg(e.k()).arg(e.l()).arg(d).arg(tth));
//        }
//      }
//    }
//  }

//  set_CalibrantDSpacings(pts);
//}

double QxrdCenterFinder::calibrantDSpacing(int n)
{
  double res = qQNaN();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdCalibrantDSpacingsPtr dsp(expt->calibrantDSpacings());

    if (dsp) {
      res = dsp->calibrantDSpacing(n);
    }
  }

  return res;
}

double QxrdCenterFinder::calibrantTTH(int n)
{
  double res = qQNaN();

  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdCalibrantDSpacingsPtr dsp(expt->calibrantDSpacings());

    if (dsp) {
      res = dsp->calibrantTTH(n);
    }
  }

  return res;
}

static int XYZCompare(const void *v1, const void *v2)
{
   XYZ *p1,*p2;
   p1 = (XYZ*) v1;
   p2 = (XYZ*) v2;
   if (p1->x < p2->x)
      return(-1);
   else if (p1->x > p2->x)
      return(1);
   else
      return(0);
}

static int compareXYX(const XYZ& a, const XYZ& b)
{
  return a.x < b.x;
}

static int differentVertices(const XYZ& a, const XYZ& b)
{
  return (a.x != b.x) || (a.y != b.y);
}

static int nearby(double x1, double y1, double x2, double y2)
{
  double dx = x1 - x2;
  double dy = y1 - y2;

  return sqrt(dx*dx + dy*dy) < 100;
}

void QxrdCenterFinder::calculateCalibration()
{
  printMessage("centering.calculateCalibration()");
  QxrdExperimentPtr expt(m_Experiment);

  if (expt) {
    QxrdDataProcessorPtr proc(expt->dataProcessor());

    if (proc) {
      QcepDoubleImageDataPtr res = newData();
      res -> fill(nan(""));

      int wd = res->get_Width();
      int ht = res->get_Height();

      QxrdPlaneFitter f00, f01, f10, f11;

      int np = countPowderRingPoints();

      QVector<XYZ> verts;

      for (int i=0; i<np; i++) {
        QxrdPowderPoint pt = powderPoint(i);

        if (pt.isValid()) {
          double x = pt.x();
          double y = pt.y();
          double tth = getTTH(x, y);
          double tthNom = calibrantTTH(pt.n1());
          double disp = get_DetectorDistance()*(tan(tth*M_PI/180.0)/tan(tthNom*M_PI/180.0)-1.0);

          XYZ p;
          p.x = x;
          p.y = y;
          p.z = disp;
          verts.append(p);

          if (nearby(x,y,0,0)) {
            f00.addPoint(x,y,disp);
          }

          if (nearby(x,y,0,ht)) {
            f01.addPoint(x,y,disp);
          }

          if (nearby(x,y,wd,0)) {
            f10.addPoint(x,y,disp);
          }

          if (nearby(x,y,wd,ht)) {
            f11.addPoint(x,y,disp);
          }
        }
      }

      XYZ pc;

      pc.x = 0;  pc.y = 0;  pc.z = f00.value(0,0);   verts.append(pc); printMessage(tr("appended (%1,%2,%3)").arg(pc.x).arg(pc.y).arg(pc.z));
      pc.x = 0;  pc.y = ht; pc.z = f01.value(0,ht);  verts.append(pc); printMessage(tr("appended (%1,%2,%3)").arg(pc.x).arg(pc.y).arg(pc.z));
      pc.x = wd; pc.y = 0;  pc.z = f10.value(wd,0);  verts.append(pc); printMessage(tr("appended (%1,%2,%3)").arg(pc.x).arg(pc.y).arg(pc.z));
      pc.x = wd; pc.y = ht; pc.z = f11.value(wd,ht); verts.append(pc); printMessage(tr("appended (%1,%2,%3)").arg(pc.x).arg(pc.y).arg(pc.z));

      XYZ p = {0,0,0};

      std::sort(verts.begin(), verts.end(), compareXYX);

      int n = verts.count();

      QVector<XYZ> uniq;

      for (int i=0; i<n; i++) {
        if (i==0 || differentVertices(verts[i], verts[i-1])) {
          uniq.append(verts[i]);
        }
      }

      int drop = verts.count() - uniq.count();

      if (drop > 0) {
        printMessage(tr("Dropped %1 duplicated vertices").arg(drop));
      }

      n = uniq.count();

      uniq.append(p);
      uniq.append(p);
      uniq.append(p);

      QVector<ITRIANGLE> tris(n*3);

      int ntri = 0;

      Triangulate(n, uniq.data(), tris.data(), &ntri);

      printMessage(tr("Formed %1 triangles").arg(ntri));

      int ndup = 0, nMsg = 0;

      for (int i=0; i<ntri; i++) {
        int p1 = tris[i].p1;
        int p2 = tris[i].p2;
        int p3 = tris[i].p3;

        //        printMessage(tr("%1: (%2,%3) - (%4,%5) - (%6,%7)")
        //                     .arg(i)
        //                     .arg(uniq[p1].x).arg(uniq[p1].y)
        //                     .arg(uniq[p2].x).arg(uniq[p2].y)
        //                     .arg(uniq[p3].x).arg(uniq[p3].y)
        //                     );

        double x1 = uniq[p1].x;
        double x2 = uniq[p2].x;
        double x3 = uniq[p3].x;
        double y1 = uniq[p1].y;
        double y2 = uniq[p2].y;
        double y3 = uniq[p3].y;
        double z1 = uniq[p1].z;
        double z2 = uniq[p2].z;
        double z3 = uniq[p3].z;

        double minX = qMax((double) 0,floor(qMin(qMin(x1,x2),x3)));
        double maxX = qMin((double) wd,ceil(qMax(qMax(x1,x2),x3)));
        double minY = qMax((double) 0,floor(qMin(qMin(y1,y2),y3)));
        double maxY = qMin((double) ht,ceil(qMax(qMax(y1,y2),y3)));

        double detT = (y2 - y3)*(x1 - x3) + (x3 - x2)*(y1 - y3);

        int ndupt = 0;
        int nptst = 0;

        for (double y=minY; y<=maxY; y++) {
          for (double x=minX; x<=maxX; x++) {
            double l1 = ((y2 - y3)*(x - x3) + (x3 - x2)*(y - y3))/detT;
            double l2 = ((y3 - y1)*(x - x3) + (x1 - x3)*(y - y3))/detT;
            double l3 = 1 - l1 - l2;

            if (0 < l1 && l1 < 1.0 &&
                0 < l2 && l2 < 1.0 &&
                0 < l3 && l3 < 1.0) {
              double z = l1*z1 + l2*z2 + l3*z3;

              double val = res->getImageData(x,y);

              if (val == val) {
                ndup += 1;
                ndupt += 1;
                res -> setImageData(x,y, i);
              } else {
                res -> setImageData(x,y, z);
              }

              nptst += 1;
            }
          }
        }

        if (ndupt > 0 && nMsg++ < 50) {
          printMessage(tr("Triangle %1 : (%2,%3) - (%4,%5) - (%6,%7) : %8/%9 dups")
                       .arg(i)
                       .arg(uniq[p1].x).arg(uniq[p1].y)
                       .arg(uniq[p2].x).arg(uniq[p2].y)
                       .arg(uniq[p3].x).arg(uniq[p3].y)
                       .arg(ndupt).arg(nptst));
        }
      }

      int nunset = 0;

      for (int y=0; y<ht; y++) {
        for (int x=0; x<wd; x++) {
          double val = res->getImageData(x,y);

          if (val!=val) {
            nunset += 1;
          }
        }
      }

      for (int y=0; y<ht; y+=ht-1) {
        for (int x=0; x<wd; x+=wd-1) {
          double val = res->getImageData(x,y);

          if (val!=val) {
            printMessage(tr("[%1,%2] unset").arg(x).arg(y));
          }
        }
      }

      printMessage(tr("%1 duplicated points, %2 unset pixels").arg(ndup).arg(nunset));
      proc->newData(res, QcepMaskDataPtr());
    }
  }
}

QScriptValue QxrdCenterFinder::toScriptValue(QScriptEngine *engine, const QxrdCenterFinderPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QxrdCenterFinder::fromScriptValue(const QScriptValue &obj, QxrdCenterFinderPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdCenterFinder *f = qobject_cast<QxrdCenterFinder*>(qobj);

    if (f) {
      proc = QxrdCenterFinderPtr(f);
    }
  }
}

