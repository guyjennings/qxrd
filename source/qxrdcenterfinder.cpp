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
    m_Experiment(expt)
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

  connect(prop_EnableUserGeometry(), SIGNAL(valueChanged(int,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserGeometryScript(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserGeometryFunction(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));

  connect(prop_EnableUserAbsorption(), SIGNAL(valueChanged(int,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserAbsorptionScript(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
  connect(prop_UserAbsorptionFunction(), SIGNAL(valueChanged(QString,int)), this, SIGNAL(parameterChanged()));
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

void QxrdCenterFinder::evaluateFit(double *parm, double *x, int np, int nx)
{
  QcepPolygon pts = get_MarkedPoints();
  double cx = parm[0];
  double cy = parm[1];
  double r  = parm[2];

  for (int i=0; i<nx; i++) {
    QwtDoublePoint pt = pts.value(i);
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

  if (sum != 0) {
    return QwtDoublePoint(sumx/sum, sumy/sum);
  } else {
    return pt;
  }
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

double QxrdCenterFinder::getPowderPointX(int i)
{
  QwtDoublePoint res = get_MarkedPoints().at(i);

  return res.x();
}

double QxrdCenterFinder::getPowderPointY(int i)
{
  QwtDoublePoint res = get_MarkedPoints().at(i);

  return res.y();
}

void QxrdCenterFinder::setPowderPoint(int i, double x, double y)
{
  QcepPolygon pts = get_MarkedPoints();

  if (i>=0 && i<pts.count()) {
    pts[i] = QwtDoublePoint(x,y);
  } else {
    pts.append(QwtDoublePoint(x,y));
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
