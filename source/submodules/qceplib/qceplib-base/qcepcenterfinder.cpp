#include "qcepallocator.h"
#include "qcepcenterfinder.h"
#include "qcepcenterfinderpicker.h"
#include "qcepapplication.h"
#include <qwt_plot_marker.h>
#include "qcepmutexlocker.h"
#include "levmar.h"
#include <QMessageBox>
#include <QtConcurrentMap>
#include "qcepdebug.h"
#include "qcepfitterpeakpoint.h"
#include "qcepfitterringpoint.h"
#include "qcepfitterringcircle.h"
#include "qcepfitterringellipse.h"
#include <QVector>
#include "triangulate.h"
#include "qcepplanefitter.h"
#include "qcepdatasetmodel-ptr.h"
#include "qcepdatasetmodel.h"
#include "qcepcalibrantlibrary.h"
#include "qcepfittedrings.h"
#include "qcepfittedrings-ptr.h"
#include "qcepcalibrantdspacings.h"

# ifdef LINSOLVERS_RETAIN_MEMORY
#  ifdef _MSC_VER
#  pragma message("LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!")
#  else
#  warning LINSOLVERS_RETAIN_MEMORY is not safe in a multithreaded environment and should be turned off!
#  endif /* _MSC_VER */
# endif /* LINSOLVERS_RETAIN_MEMORY */

QcepCenterFinder::QcepCenterFinder(QString name)
  : inherited(name),
    m_Center(this, "center", QPointF(0,0), "Center"),
    m_CenterStep(this, "centerStep", 1, "Center Step"),
    m_DetectorXPixelSize(this, "detectorXPixelSize", 200, "Detector X Pixels (um)"),
    m_DetectorYPixelSize(this, "detectorYPixelSize", 200, "Detector Y Pixels (um)"),
    m_DetectorDistance(this, "detectorDistance", 1000, "Sample-Detector Distance (mm)"),
    m_DetectorDistanceStep(this, "detectorDistanceStep", 100, "Sample-Detector Distance Step (mm)"),
    m_Energy(this, "energy", 20000, "Beam Energy (eV)"),
    m_ImplementTilt(this,"implementTilt", false, "Implement Detector Tilt?"),
    m_DetectorTilt(this, "detectorTilt", 0, "Tilt Angle (deg)"),
    m_DetectorTiltStep(this, "detectorTiltStep", 0.1, "Tilt Angle Step(deg)"),
    m_TiltPlaneRotation(this, "tiltPlaneRotation", 90, "Tilt Plane Rotation (deg)"),
    m_TiltPlaneRotationStep(this, "tiltPlaneRotationStep", 10, "Tilt Plane Rotation Step (deg)"),
    m_MarkedPoints(this, "markedPoints", QcepPowderPointVector(), "Marker Points"),
    m_FittedRings(this, "fittedRings", QcepPowderPointVector(), "Fitted Powder Rings"),
    m_RingRadius(this, "ringRadius", 0.0, "Estimated Powder Ring Radius"),
    m_RingRadiusA(this, "ringRadiusA", 0.0, "Estimated Powder Ellipse Major Axis Radius"),
    m_RingRadiusB(this, "ringRadiusB", 0.0, "Estimated Powder Ellipse Minor Axis Radius"),
    m_RingRotation(this, "ringRotation", 0.0, "Estimated Powder Ellipse Major Axis Rotation"),
    m_PeakFitRadius(this, "peakFitRadius", 10, "Half size of fitted area for peak fitting"),
    m_PeakHeight(this, "peakHeight", 100.0, "Height of fitted peak"),
    m_PeakCenterX(this, "peakCenterX", 0, "X Center of fitted peak"),
    m_PeakCenterY(this, "peakCenterY", 0, "Y Center of fitted peak"),
    m_PeakWidth(this, "peakWidth", 2.0, "Width of fitted peak"),
    m_PeakBackground(this, "peakBackground", 0, "Background Height of fitted peak"),
    m_PeakBackgroundX(this, "peakBackgroundX", 0, "X Slope of Background"),
    m_PeakBackgroundY(this, "peakBackgroundY", 0, "Y Slope of Background"),
    m_PeakFitDebug(this, "peakFitDebug", false, "Debug Print for peak fitting"),
    m_PeakFitIterations(this, "peakFitIterations", 200, "Max Iterations during fitting"),
    m_RingAngles(this, "ringAngles", QcepDoubleVector(), "Diffraction ring angles"),
    m_RingAngleTolerance(this, "ringAngleTolerance", 0.1, "Diffraction ring angle tolerance"),
    m_PowderFitOptions(this, "powderFitOptions", 0, "Powder fitting options"),
    m_RingIndex(this, "ringIndex", 0, "Fitted Powder Ring Index"),
    m_SubtractRingAverages(this, "subtractRingAverages", false, "Plot deviations of each ring from average"),
    m_RingAverageDisplacement(this, "ringAverageDisplacement", 0.0, "Extra displacement between curves in ring radius plot"),
    m_FittedWidthMin(this, "fittedWidthMin", 0.5, "Minimum acceptable fitted width (pixels)"),
    m_FittedWidthMax(this, "fittedWidthMax", 3.0, "Maximum acceptable fitted width (pixels)"),
    m_FittedHeightMinRatio(this, "fittedHeightMinRatio", 0.25, "Minimum acceptable peak height ratio"),
    m_FittedPositionMaxDistance(this, "fittedPositionMaxDistance", 2.0, "Maximum acceptable fitted position shift (pixels)"),
    m_FitPowderPointPosition(this, "fitPowderPointPosition", true, "Fit to nearby peak when adding powder points individually")
{
#ifndef QT_NO_DEBUG
  printf("Constructing center finder\n");
#endif

  qRegisterMetaType<QPointF>("QPointF");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);

  connect(prop_Center(), &QcepDoublePointProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorXPixelSize(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorYPixelSize(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorDistance(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_Energy(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_ImplementTilt(), &QcepBoolProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_DetectorTilt(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
  connect(prop_TiltPlaneRotation(), &QcepDoubleProperty::valueChanged, this, &QcepCenterFinder::parameterChanged, Qt::DirectConnection);
}

void QcepCenterFinder::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);
}

QcepCenterFinder::~QcepCenterFinder()
{
#ifndef QT_NO_DEBUG
  printf("Deleting center finder\n");
#endif
}

QcepCenterFinderWPtr QcepCenterFinder::findCenterFinder(QcepObjectWPtr p)
{
  QcepCenterFinderWPtr res =
      qSharedPointerDynamicCast<QcepCenterFinder>(p);

  if (res == NULL) {
    QcepObjectPtr objp(p);

    if (objp) {
      res = findCenterFinder(objp->parentPtr());
    }
  }

  return res;
}

QcepDoubleImageDataPtr QcepCenterFinder::data() const
{
  return m_Data;
}

QcepDoubleImageDataPtr QcepCenterFinder::newData()
{
  int wd, ht;

  if (m_Data) {
    wd = m_Data->get_Width();
    ht = m_Data->get_Height();
  } else {
    wd = 2048;
    ht = 2048;
  }

  QcepDoubleImageDataPtr res =
      QcepAllocator::newDoubleImage(sharedFromThis(),
                                    "newData",
                                    wd,
                                    ht,
                                    QcepAllocator::NullIfNotAvailable);

  return res;
}

void QcepCenterFinder::writeSettings(QSettings *settings)
{
  inherited::writeSettings(settings);
}

void QcepCenterFinder::readSettings(QSettings *settings)
{
  inherited::readSettings(settings);
}

void QcepCenterFinder::onCenterChanged(QPointF pt)
{
  THREAD_CHECK;

  set_Center(pt);
}

double QcepCenterFinder::getTTH(QPointF pt) const
{
  return getTTH(pt.x(), pt.y());
}

double QcepCenterFinder::getTTH(double x, double y) const
{
  if (get_ImplementTilt()) {
    double beta = get_DetectorTilt()*M_PI/180.0;
    double rot  = get_TiltPlaneRotation()*M_PI/180.0;

    return getTwoTheta(get_Center().x(), get_Center().y(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       cos(beta), sin(beta), cos(rot), sin(rot));
  } else {
    return getTwoTheta(get_Center().x(), get_Center().y(), get_DetectorDistance(),
                       x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
                       1.0, 0.0, 1.0, 0.0);
  }
}

QPointF QcepCenterFinder::getXY(double tth, double chi)
{
  double x,y;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {

    inherited::getXY(get_Center().x(), get_Center().y(), get_DetectorDistance(),
          get_Energy(),
          convertTwoThetaToQ(tth, convertEnergyToWavelength(get_Energy())), chi,
          get_DetectorXPixelSize(), get_DetectorYPixelSize(),
          rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
          &x, &y);
  } else {
    inherited::getXY(get_Center().x(), get_Center().y(), get_DetectorDistance(),
          get_Energy(),
          convertTwoThetaToQ(tth, convertEnergyToWavelength(get_Energy())), chi,
          get_DetectorXPixelSize(), get_DetectorYPixelSize(),
          rot, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
          &x, &y);
  }

  return QPointF(x,y);
}

double QcepCenterFinder::getQ(QPointF pt) const
{
  return getQ(pt.x(), pt.y());
}

double QcepCenterFinder::getQ(double x, double y) const
{
  double q,chi;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    getQChi(get_Center().x(), get_Center().y(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
            &q, &chi);
  } else {
    getQChi(get_Center().x(), get_Center().y(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
            &q, &chi);
  }

  return q;
}

double QcepCenterFinder::getChi(QPointF pt) const
{
  return getChi(pt.x(), pt.y());
}

double QcepCenterFinder::getChi(double x, double y) const
{
  double q,chi;
  double beta = get_DetectorTilt()*M_PI/180.0;
  double rot  = get_TiltPlaneRotation()*M_PI/180.0;

  if (get_ImplementTilt()) {
    getQChi(get_Center().x(), get_Center().y(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            rot, cos(beta), sin(beta), 1.0, 0.0, cos(rot), sin(rot),
            &q, &chi);
  } else {
    getQChi(get_Center().x(), get_Center().y(), get_DetectorDistance(),
            get_Energy(),
            x, y, get_DetectorXPixelSize(), get_DetectorYPixelSize(),
            0.0, 1.0, 0.0, 1.0, 0.0, 1.0, 0.0,
            &q, &chi);
  }

  return chi;
}

double QcepCenterFinder::getDist(QPointF pt) const
{
  return getDist(pt.x(), pt.y());
}

double QcepCenterFinder::getDist(double x, double y) const
{
  double tth = getTTH(x,y);

  return get_DetectorDistance()/cos(tth*M_PI/180.0);
}

double QcepCenterFinder::getR(QPointF pt) const
{
  return getR(pt.x(), pt.y());
}

double QcepCenterFinder::getR(double x, double y) const
{
  double tth = getTTH(x, y);
  double r = get_DetectorDistance()*tan(tth*M_PI/180.0);

  return r;
}

void QcepCenterFinder::onPointSelected(QPointF pt)
{
  if (get_FitPowderPointPosition()) {
    fitPeakNear(pt.x(), pt.y());
  } else {
    appendPowderPoint(pt.x(), pt.y());
  }
}

void QcepCenterFinder::fitPowderCircle(int n)
{
  QcepFitterRingCircle fitter(this, n, get_Center().x(), get_Center().y());

  int niter = fitter.fit();

  int update = false;
  QString message;

  if (fitter.reason() == QcepFitter::Successful) {
    message.append(tr("Circle Fitting Succeeded after %1 iterations\n").arg(niter));
    message.append(tr("Old Center = [%1,%2]\n").arg(get_Center().x()).arg(get_Center().y()));
    message.append(tr("New Center = [%1,%2], New Radius = %3\n").arg(fitter.fittedX()).arg(fitter.fittedY()).arg(fitter.fittedR()));
    double dx = fitter.fittedX() - get_Center().x();
    double dy = fitter.fittedY() - get_Center().y();
    message.append(tr("Moved by [%1,%2] = %3\n").arg(dx).arg(dy).arg(sqrt(dx*dx + dy*dy)));
  } else {
    message.append(tr("Circle Fitting Failed: Reason = %1\n").arg(fitter.reasonString()));
  }

  printMessage(message);

  if (g_Application && g_Application->get_GuiWanted()) {
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
    set_Center(QPointF(fitter.fittedX(), fitter.fittedY()));
    set_RingRadius(fitter.fittedR());
  }
}

void QcepCenterFinder::fitPowderEllipse(int n)
{
  QcepFitterRingEllipse fitter(this, n, get_Center().x(), get_Center().y());

  int niter = fitter.fit();

  int update = false;
  QString message;

  if (fitter.reason() == QcepFitter::Successful) {
    message.append(tr("Ellipse Fitting Succeeded after %1 iterations\n").arg(niter));
    message.append(tr("Old Center = [%1,%2]\n").arg(get_Center().x()).arg(get_Center().y()));
    message.append(tr("New Center = [%1,%2], New Radii = %3,%4\n")
                   .arg(fitter.fittedX()).arg(fitter.fittedY()).arg(fitter.fittedA()).arg(fitter.fittedB()));
    message.append(tr("Major Axis rotation = %1 deg\n").arg(fitter.fittedRot()*180.0/M_PI));

    double dx = fitter.fittedX() - get_Center().x();
    double dy = fitter.fittedY() - get_Center().y();
    message.append(tr("Moved by [%1,%2] = %3\n").arg(dx).arg(dy).arg(sqrt(dx*dx + dy*dy)));
  } else {
    message.append(tr("Ellipse Fitting Failed: Reason = %1\n").arg(fitter.reasonString()));
  }

  printMessage(message);

  if (g_Application && g_Application->get_GuiWanted()) {
    if (fitter.reason() == QcepFitter::Successful) {
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
    set_Center(QPointF(fitter.fittedX(), fitter.fittedY()));
    set_RingRadiusA(fitter.fittedA());
    set_RingRadiusB(fitter.fittedB());
    set_RingRotation(fitter.fittedRot());
  }
}

void QcepCenterFinder::fitPowderEllipses()
{
  int nrings = countPowderRings();

  QVector<QcepFitterRingEllipse> fits;

  for (int i=0; i<nrings; i++) {
    fits.append(QcepFitterRingEllipse(this, i, get_Center().x(), get_Center().y()));
  }

  if (qcepDebug(DEBUG_NOPARALLEL)) {
    for (int i=0; i<nrings; i++) {
      fits[i].fit();
    }
  } else {
    QFuture<void> fitDone = QtConcurrent::map(fits, &QcepFitterRingEllipse::fit);

    fitDone.waitForFinished();
  }

  QcepFittedRingsPtr pts(new QcepFittedRings("rings"));

  if (pts) {
    pts->initialize(sharedFromThis());

    for (int i=0; i<nrings; i++) {
      QcepFitterRingEllipse &r = fits[i];

      if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
        printMessage(tr("Fitted Ring %1: x: %2, y: %3, a: %4, b: %5, rot: %6, rzn: %7")
                     .arg(i).arg(r.fittedX()).arg(r.fittedY())
                     .arg(r.fittedA()).arg(r.fittedB()).arg(r.fittedRot())
                     .arg(r.reasonString()));
      }

      if (r.reason() == QcepFitter::Successful) {
        pts->append(i,
                    r.fittedX(),
                    r.fittedY(),
                    r.fittedA(),
                    r.fittedB(), r.fittedRot());
      }
    }

    setFittedRings(pts);
  }
}

void QcepCenterFinder::setFittedRings(QcepFittedRingsPtr rings)
{
  if (rings) {
    int nr = rings->rowCount();

    for (int i=0; i<nr; i++) {
      printMessage(tr("Ring:%1 x:%2 y:%3: a:%4 b:%5 rot:%6")
                   .arg(rings->n(i))
                   .arg(rings->x(i))
                   .arg(rings->y(i))
                   .arg(rings->a(i))
                   .arg(rings->b(i))
                   .arg(rings->rot(i)));
    }

    QcepDatasetModelPtr data(m_Dataset);

    if (data) {
      data->remove("rings");
      data->append("rings", rings);
    }
  }
}

void QcepCenterFinder::adjustEnergy(int n)
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

void QcepCenterFinder::adjustDistance(int n)
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

QcepPowderPoint QcepCenterFinder::powderPoint(int i)
{
  return get_MarkedPoints().value(i);
}

int QcepCenterFinder::nearestPowderPointIndex(double x, double y)
{
  int nearest = -1;
  double nearestDist;
  QcepPowderPointVector pts = get_MarkedPoints();

  for (int i=0; i<pts.count(); i++) {
    QcepPowderPoint pt = pts.value(i);
    double dist = sqrt(pow(x-pt.x(), 2) + pow(y-pt.y(), 2));

    if (nearest == -1 || dist < nearestDist) {
      nearest     = i;
      nearestDist = dist;
    }
  }

  return nearest;
}

QcepPowderPoint QcepCenterFinder::nearestPowderPoint(double x, double y)
{
  return get_MarkedPoints().value(nearestPowderPointIndex(x,y));
}

void QcepCenterFinder::deletePowderPointNear(double x, double y)
{
  int nearest = nearestPowderPointIndex(x, y);

  if (nearest >= 0) {
    QcepPowderPointVector pts = get_MarkedPoints();

    pts.remove(nearest);

    set_MarkedPoints(pts);
  }
}

void QcepCenterFinder::appendPowderPoint(double x, double y)
{
  m_MarkedPoints.appendValue(QcepPowderPoint(get_RingIndex(), 0, 0, x,y, 0,0,0));
}

void QcepCenterFinder::appendPowderPoint(int n1, int n2, int n3, double x, double y, double r1, double r2, double az)
{
  m_MarkedPoints.appendValue(QcepPowderPoint(n1,n2,n3, x, y, r1, r2, az));
}

void QcepCenterFinder::deletePowderRing(int n)
{
  QcepPowderPointVector pts = get_MarkedPoints();

  QcepPowderPointVector res;

  foreach (QcepPowderPoint pt, pts) {
    if (pt.n1() < n) {
      res.append(pt);
    } else if (pt.n1() > n) {
      res.append(QcepPowderPoint(pt.n1()-1, pt.n2(), pt.n3(), pt.x(), pt.y(), pt.r1(), pt.r2(), pt.az()));
    }
  }

  set_MarkedPoints(res);
}

void QcepCenterFinder::disablePowderRing(int n)
{
  QcepPowderPointVector pts = get_MarkedPoints();

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

void QcepCenterFinder::enablePowderRing(int n)
{
  QcepPowderPointVector pts = get_MarkedPoints();

  int np=0;

  for (int i=0; i<pts.count(); i++) {
    if (pts[i].n1() == n) {
      pts[i].n2() = np++;
    }
  }

  printMessage(tr("Enabled %1 points in ring %2").arg(np).arg(n));

  set_MarkedPoints(pts);
}

void QcepCenterFinder::deletePowderPoints()
{
  m_MarkedPoints.clear();

  set_RingIndex(0);
}

void QcepCenterFinder::normalizePowderRings()
{
  QcepPowderPointVector pts = get_MarkedPoints();

  double maxR = 0;

  foreach(QcepPowderPoint pt, pts) {
    double r = getR(pt.x(), pt.y());

    if (r > maxR) {
      maxR = r;
    }
  }

  if (maxR > 0 && maxR < 10000) {
    int nbins = (int) maxR + 5;
    QVector<int> npts(nbins);

    npts.fill(0);

    foreach(QcepPowderPoint pt, pts) {
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
      QcepPowderPoint &pt = pts[i];

      double r = getR(pt.x(), pt.y());
      int idx = npts[(int) r];

      pt.n1() = idx;
    }

    set_MarkedPoints(pts);
    set_RingIndex(ringIndex);
  }
}

bool QcepCenterFinder::fitPeakNear(double x, double y)
{
  printMessage(tr("centering.fitPeakNear(%1,%2)").arg(x).arg(y));

  QcepFitterPeakPoint fit(this, 0, x, y, 0, 0);
  fit.fit();

  if (fit.reason() == QcepFitter::Successful) {
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

bool QcepCenterFinder::fitRingNear(double x0, double y0)
{
  printMessage(tr("centering.fitRingNear(%1,%2)").arg(x0).arg(y0));

  QcepFitterRingPoint fit(this, 0, x0, y0, 0, 0);
  fit.fit();

  if (fit.reason() == QcepFitter::Successful) {
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

bool QcepCenterFinder::traceRingNear(double x0, double y0, double step)
{
  printMessage(tr("centering.traceRingNear(%1,%2,%3)").arg(x0).arg(y0).arg(step));

  double x=x0, y=y0;
  double xc  = get_Center().x();
  double yc  = get_Center().y();
  double dx  = x0-xc;
  double dy  = y0-yc;
  double az  = atan2(dy, dx);
  double az0 = az;
  double r   = sqrt(dx*dx + dy*dy);
  double ast = step/r;
  double dr  = get_PeakFitRadius();

  QcepFitterRingPoint fit(this, 0, x0, y0, 0, 0);
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

  QVector<QcepFitterRingPoint> fits;

  while (true) {
    if (x >= 0 && y >= 0 && x <= width && y <= height) {
      QcepFitterRingPoint fit(this, 0, x, y, pkht, bkgd);
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

      if (fit.reason()==QcepFitter::Successful) {
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

  QcepPowderPointVector pts = get_MarkedPoints();

  QVector<int> nreason(QcepFitter::LastReason);

  for(int i=0; i<fits.count(); i++) {
    QcepFitterRingPoint &fit = fits[i];

    if (fit.reason() == QcepFitter::Successful) {
      nok += 1;
      pts.append(QcepPowderPoint(get_RingIndex(), 0, 0, fit.fittedX(), fit.fittedY(), fit.fittedR(), fit.fittedR(), fit.fittedAz()));
    }

    nreason[fit.reason()] += 1;
  }

  QcepDatasetModelPtr data(m_Dataset);

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
      scan->column(2)->setFormatter(&QcepFitter::reasonString);

      for (int i=0; i<fits.count(); i++) {
        QcepFitterRingPoint &fit = fits[i];
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

  if (npts) {
    QString msg(tr("centering.traceRingNear : %1/%2 fitted points").arg(nok).arg(npts));

    printMessage(msg);
    statusMessage(msg);

    for (int i=0; i<QcepFitter::LastReason; i++) {
      if (nreason[i] > 0) {
        QString msg(tr("  %1 : %2").arg(nreason[i]).arg(QcepFitter::reasonString((QcepFitter::FitResult) i)));
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

bool QcepCenterFinder::missingRingNear(double x, double y)
{
  appendPowderPoint(get_RingIndex(), -1, 0, x, y, getR(x,y), getR(x,y), getChi(x,y));
  prop_RingIndex()->incValue(1);

  return true;
}

//bool QcepCenterFinder::traceRingNearParallel(double x0, double y0, double step)
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

//  QcepFitterRingPoint fit(this, 0, x0, y0, 0, 0);

//  fit.fit();

//  double bkgd = ( imageValue(xc+(r+dr)*cos(az), yc+(r+dr)*sin(az))
//                 +imageValue(xc+(r-dr)*cos(az), yc+(r-dr)*sin(az)))/2.0;

//  double pkht = imageValue(x0,y0) - bkgd;

//  int nsteps = (int) ((2.0*M_PI)/ast)+1;

//  QVector<QcepFitterRingPoint> fits;

//  double tth = getTTH(x0, y0);
//  for (int i=0; i<nsteps; i++) {
//    double chi = ast*i*180.0/M_PI;
//    QPointF xy  = getXY(tth, chi);

//    fits.append(QcepFitterRingPoint(this, i, xy.x(), xy.y(), pkht, bkgd));

////    if (qcepDebug(DEBUG_FITTING) || get_PeakFitDebug()) {
////      printMessage(tr("Fitting i: %1, x0: %2, y0: %3").arg(i).arg(fits[i].x0()).arg(fits[i].y0()));
////    }
//  }

//  if (qcepDebug(DEBUG_NOPARALLEL)) {
//    for (int i=0; i<nsteps; i++) {
//      fits[i].fit();
//    }
//  } else {
//    QFuture<void> fitDone = QtConcurrent::map(fits, &QcepFitterRingPoint::fit);

//    fitDone.waitForFinished();
//  }

//  int sums[6];

//  for (int i=0; i<6; i++) {
//    sums[i]=0;
//  }

//  QcepPowderPointVector pts = get_MarkedPoints();

//  for (int i=0; i<nsteps; i++) {
//    QcepFitterRingPoint &r = fits[i];

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

//    if (r.reason() == QcepFitter::Successful) {
//      pts.append(QcepPowderPoint(get_RingIndex(), 0, r.fittedX(), r.fittedY(), r.fittedR(), r.fittedR(), r.fittedAz()));
//    }
//  }

//  QString msg(tr("centering.traceRingNearParallel : Fitted %1/%2 : NR %3, OR %4, BdW %5, BdP %6, BdH %7")
//      .arg(sums[QcepFitter::Successful])
//      .arg(nsteps)
//      .arg(sums[QcepFitter::NoResult])
//      .arg(sums[QcepFitter::OutsideData])
//      .arg(sums[QcepFitter::BadWidth])
//      .arg(sums[QcepFitter::BadPosition])
//      .arg(sums[QcepFitter::BadHeight])
//      );

//  printMessage(msg);
//  statusMessage(msg);

//  if (sums[QcepFitter::Successful]) {
//    prop_RingIndex()->incValue(1);
//    set_MarkedPoints(pts);
//  }

//  return true;
//}

int QcepCenterFinder::getPowderPointN1(int i)
{
  QcepPowderPoint res = get_MarkedPoints().value(i);

  return res.n1();
}

int QcepCenterFinder::getPowderPointN2(int i)
{
  QcepPowderPoint res = get_MarkedPoints().value(i);

  return res.n2();
}

double QcepCenterFinder::getPowderPointX(int i)
{
  QcepPowderPoint res = get_MarkedPoints().value(i);

  return res.x();
}

double QcepCenterFinder::getPowderPointY(int i)
{
  QcepPowderPoint res = get_MarkedPoints().value(i);

  return res.y();
}

void QcepCenterFinder::setPowderPoint(int i, int n1, int n2, int n3, double x, double y, double r1, double r2, double az)
{
  QcepPowderPointVector pts = get_MarkedPoints();

  if (i>=0 && i<pts.count()) {
    pts[i] = QcepPowderPoint(n1,n2,n3,x,y,r1,r2,az);
  } else {
    pts.append(QcepPowderPoint(n1,n2,n3,x,y,r1,r2,az));
  }

  set_MarkedPoints(pts);
}

//QScriptValue QcepCenterFinder::getPowderPoint(int i)
//{
//  QcepScriptEnginePtr   eng(m_ScriptEngine);
//  QcepPowderPointVector pts = get_MarkedPoints();

//  if (eng) {
//    if (i>=0 && i<get_MarkedPoints().count()) {
//      QScriptValue val = eng->newObject();

//      QcepPowderPoint &pt = pts[i];
//      val.setProperty("n1", pt.n1());
//      val.setProperty("n2", pt.n2());
//      val.setProperty("x",  pt.x());
//      val.setProperty("y",  pt.y());
//      val.setProperty("r1", pt.r1());
//      val.setProperty("r2", pt.r2());
//      val.setProperty("az", pt.az());

//      return val;
//    }
//  }

//  return QScriptValue();
//}

//QScriptValue QcepCenterFinder::getPowderPoints()
//{
//  QcepScriptEnginePtr eng(m_ScriptEngine);

//  if (eng) {
//    QcepPowderPointVector pts = get_MarkedPoints();

//    QScriptValue val = eng->newArray();

//    for (int i=0; i<pts.count(); i++) {
//      QScriptValue item = eng->newObject();

//      QcepPowderPoint &pt = pts[i];
//      item.setProperty("n1", pt.n1());
//      item.setProperty("n2", pt.n2());
//      item.setProperty("x", pt.x());
//      item.setProperty("y", pt.y());
//      item.setProperty("r1", pt.r1());
//      item.setProperty("r2", pt.r2());
//      item.setProperty("az", pt.az());

//      val.setProperty(tr("%1").arg(i), item);
//    }

//    return val;
//  }

//  return QScriptValue();
//}

//void         QcepCenterFinder::setPowderPoint(int i, QScriptValue val)
//{
//  int   n1 = val.property("n1").toInteger();
//  int   n2 = val.property("n2").toInteger();
//  int   n3 = val.property("n3").toInteger();
//  double x = val.property("x").toNumber();
//  double y = val.property("y").toNumber();
//  double r1 = val.property("r1").toNumber();
//  double r2 = val.property("r2").toNumber();
//  double az = val.property("az").toNumber();

//  setPowderPoint(i, n1, n2, n3, x, y, r1, r2, az);
//}


void QcepCenterFinder::setData(QcepDoubleImageDataPtr data)
{
  m_Data = data;
}

double QcepCenterFinder::imageValue(double x, double y)
{
  if (m_Data) {
    return m_Data->value(x,y);
  } else {
    return 0;
  }
}

QString QcepCenterFinder::levmarFailureReason(int n)
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

int QcepCenterFinder::countPowderRings() const
{
  int max = -1;

  QcepPowderPointVector pts = get_MarkedPoints();

  int n = pts.count();

  for (int i=0; i<n; i++) {
    QcepPowderPoint pt = pts.value(i);

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

int QcepCenterFinder::countPowderRingPoints() const
{
  return get_MarkedPoints().count();
}

int QcepCenterFinder::countPowderRingPoints(int r) const
{
  QcepPowderPointVector pts = get_MarkedPoints();

  int sum = 0;

  foreach (QcepPowderPoint pt, pts) {
    if (pt.n1() == r || r < 0) {
      sum += 1;
    }
  }

  return sum;
}

QcepPowderPoint QcepCenterFinder::powderRingPoint(int i) const
{
  return get_MarkedPoints().value(i);
}

QcepPowderPoint QcepCenterFinder::powderRingPoint(int r, int i) const
{
  QcepPowderPointVector pts = get_MarkedPoints();

  int sum = 0;

  foreach (QcepPowderPoint pt, pts) {
    if (pt.n1() == r || r < 0) {
      if (sum == i) {
        return pt;
      } else {
        sum += 1;
      }
    }
  }

  return QcepPowderPoint();
}

double QcepCenterFinder::powderRingAverageR(int r) const
{
  QcepPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QcepPowderPoint pt, pts) {
    if (pt.n1() == r) {
      npts += 1;
      sum  += getR(pt.x(), pt.y());
    }
  }

  return sum/npts;
}

double QcepCenterFinder::powderRingAverageQ(int r) const
{
  QcepPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QcepPowderPoint pt, pts) {
    if (pt.n1() == r) {
      npts += 1;
      sum  += getQ(pt.x(), pt.y());
    }
  }

  return sum/npts;
}

double QcepCenterFinder::powderRingAverageTTH(int r) const
{
  QcepPowderPointVector pts = get_MarkedPoints();

  double sum = 0, npts = 0;

  foreach (QcepPowderPoint pt, pts) {
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

//void QcepCenterFinder::updateCalibrantDSpacings()
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

//  QcepPowderPointVector pts;

//  for (int i=1; i<mmax*mmax; i++) {
//    QuadInt e = ex[i];
//    if (e.n() > 0) {
//      double d = a/sqrt(i);
//      double tth = 2.0*asin(lambda/(2.0*d))*180.0/M_PI;

//      if (tth <= 180) {
//        pts.append(QcepPowderPoint(e.h(), e.k(), e.l(), d, tth, 0, 0, 0));

//        if (qcepDebug(DEBUG_CALIBRANT)) {
//          printMessage(tr("%1(%2): [%3,%4,%5], d:%6, tth:%7").arg(i).arg(e.n()).arg(e.h()).arg(e.k()).arg(e.l()).arg(d).arg(tth));
//        }
//      }
//    }
//  }

//  set_CalibrantDSpacings(pts);
//}

double QcepCenterFinder::calibrantDSpacing(int n)
{
  double res = qQNaN();

  QcepCalibrantDSpacingsPtr dsp(m_CalibrantDSpacings);

  if (dsp) {
    res = dsp->calibrantDSpacing(n);
  }

  return res;
}

double QcepCenterFinder::calibrantTTH(int n)
{
  double res = qQNaN();

  QcepCalibrantDSpacingsPtr dsp(m_CalibrantDSpacings);

  if (dsp) {
    res = dsp->calibrantTTH(n);
  }

  return res;
}

//TODO: needed?
//static int XYZCompare(const void *v1, const void *v2)
//{
//   XYZ *p1,*p2;
//   p1 = (XYZ*) v1;
//   p2 = (XYZ*) v2;
//   if (p1->x < p2->x)
//      return(-1);
//   else if (p1->x > p2->x)
//      return(1);
//   else
//      return(0);
//}

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

void QcepCenterFinder::calculateCalibration()
{
  printMessage("centering.calculateCalibration()");

  QcepDatasetModelPtr    dset(m_Dataset);
  QcepDoubleImageDataPtr res = newData();

  if (dset) {
    res -> fill(nan(""));

    int wd = res->get_Width();
    int ht = res->get_Height();

    QcepPlaneFitter f00, f01, f10, f11;

    int np = countPowderRingPoints();

    QVector<XYZ> verts;

    for (int i=0; i<np; i++) {
      QcepPowderPoint pt = powderPoint(i);

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

    dset->append("calibration", res);
  }
}

QScriptValue QcepCenterFinder::toScriptValue(QScriptEngine *engine, const QcepCenterFinderPtr &proc)
{
  return engine->newQObject(proc.data());
}

void QcepCenterFinder::fromScriptValue(const QScriptValue &obj, QcepCenterFinderPtr &proc)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepCenterFinder *f = qobject_cast<QcepCenterFinder*>(qobj);

    if (f) {
      proc = QcepCenterFinderPtr(f);
    }
  }
}

void QcepCenterFinder::moveCenter(int dx, int dy)
{
  double s = get_CenterStep();

  QPointF c(get_Center() + QPointF(s*dx, s*dy));

  set_Center(c);
}

void QcepCenterFinder::centerMoveUpLeft()
{
  moveCenter(-1,1);
}

void QcepCenterFinder::centerMoveUp()
{
  moveCenter(0,1);
}

void QcepCenterFinder::centerMoveUpRight()
{
  moveCenter(1,1);
}

void QcepCenterFinder::centerMoveRight()
{
  moveCenter(1,0);
}

void QcepCenterFinder::centerMoveDownRight()
{
  moveCenter(1,-1);
}

void QcepCenterFinder::centerMoveDown()
{
  moveCenter(0,-1);
}

void QcepCenterFinder::centerMoveDownLeft()
{
  moveCenter(-1,-1);
}

void QcepCenterFinder::centerMoveLeft()
{
  moveCenter(-1,0);
}


