/******************************************************************
*
*  $Id: qxrdcenterfinder.cpp,v 1.4 2010/10/01 22:28:45 jennings Exp $
*
*******************************************************************/

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
    m_TiltPlaneRotation(this, "tiltPlaneRotation", 90),
    SOURCE_IDENT("$Id: qxrdcenterfinder.cpp,v 1.4 2010/10/01 22:28:45 jennings Exp $")
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

/******************************************************************
*
*  $Log: qxrdcenterfinder.cpp,v $
*  Revision 1.4  2010/10/01 22:28:45  jennings
*  Added more tooltips, added pixel size editing widgets
*
*  Revision 1.3  2010/09/17 16:24:31  jennings
*  Made integrator algorithm honor the 'implementTilt' parameter
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.4  2010/09/10 18:54:10  jennings
*  Partial implement tilt in center finder dialog
*
*  Revision 1.1.2.3  2010/09/09 21:36:31  jennings
*  Made QxrdCenterFinder descend from QxrdDetectorGeometry
*
*  Revision 1.1.2.2  2010/09/08 19:40:57  jennings
*  Added tilt controls to center finder
*  Disabled fit refine command (for now)
*  Added percentile display mode
*
*  Revision 1.1.2.1  2010/07/22 18:39:37  jennings
*  Moving files into source subdirectory
*
*  Revision 1.13.2.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.13  2009/11/17 20:42:59  jennings
*  Added instrumented QxrdMutexLocker which tracks how long locks are held, and prints
*  info about any held for more than 100 msec
*
*  Revision 1.12  2009/07/25 17:03:40  jennings
*  More improvements to image plotting code
*
*  Revision 1.11  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.10  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.9  2009/07/10 22:54:23  jennings
*  Some rearrangement of data
*
*  Revision 1.8  2009/07/08 19:06:27  jennings
*  Made centering parameters into Q_PROPERTYs
*  Saved centering, integrator and data processor settings
*
*  Revision 1.7  2009/06/28 16:33:20  jennings
*  Eliminated compiler warnings
*
*  Revision 1.6  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

