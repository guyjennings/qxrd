/******************************************************************
*
*  $Id: qxrdcenterfinder.cpp,v 1.13 2009/11/17 20:42:59 jennings Exp $
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
  : QObject(parent),
    m_CenterX(this, "centerX", 0),
    m_CenterY(this, "centerY", 0),
    m_CenterStep(this, "centerStep", 1),
    SOURCE_IDENT("$Id: qxrdcenterfinder.cpp,v 1.13 2009/11/17 20:42:59 jennings Exp $")
{
  qRegisterMetaType<QwtDoublePoint>("QwtDoublePoint");

//  m_CenterX.setDebug(true);
//  m_CenterY.setDebug(true);
}

void QxrdCenterFinder::writeSettings(QxrdSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::readSettings(QxrdSettings *settings, QString section)
{
  QxrdMutexLocker lock(__FILE__, __LINE__, &m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::onCenterChanged(QwtDoublePoint pt)
{
  set_CenterX(pt.x());
  set_CenterY(pt.y());
}

/******************************************************************
*
*  $Log: qxrdcenterfinder.cpp,v $
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

