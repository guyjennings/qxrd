/******************************************************************
*
*  $Id: qxrdcenterfinder.cpp,v 1.10 2009/07/21 22:55:48 jennings Exp $
*
*******************************************************************/

#include "qxrdcenterfinder.h"
#include "qxrdcenterfinderdialog.h"
#include "qxrdcenterfinderpicker.h"
#include "qxrdwindow.h"
#include <qwt_plot_marker.h>

QxrdCenterFinder::QxrdCenterFinder
    (QObject *parent)
  : QObject(parent),
    m_CenterX(this, "centerX", 0),
    m_CenterY(this, "centerY", 0),
    m_CenterStep(this, "centerStep", 1),
    SOURCE_IDENT("$Id: qxrdcenterfinder.cpp,v 1.10 2009/07/21 22:55:48 jennings Exp $")
{
}

void QxrdCenterFinder::writeSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::writeSettings(this, &staticMetaObject, section, settings);
}

void QxrdCenterFinder::readSettings(QxrdSettings *settings, QString section)
{
  QMutexLocker lock(&m_Mutex);

  QcepProperty::readSettings(this, &staticMetaObject, section, settings);
}

//void QxrdCenterFinder::setEnabled(bool imgenabled, bool /*cntrenabled*/)
//{
//  m_CenterFinderPicker -> setEnabled(imgenabled);
//
//  if (imgenabled) {
//    m_CenterMarker     -> attach(m_ImagePlot);
//  } else {
//    m_CenterMarker     -> detach();
//  }
//}
//
//void QxrdCenterFinder::setPen(const QPen &pen)
//{
//  m_Pen = pen;
//
//  if (m_CenterFinderPicker) {
//    m_CenterFinderPicker -> setTrackerPen(pen);
//    m_CenterFinderPicker -> setRubberBandPen(pen);
//  }
//
//  if (m_CenterMarker) {
//    m_CenterMarker -> setLinePen(pen);
//  }
//}
//
//void QxrdCenterFinder::onCenterXChanged(double cx)
//{
//  if (get_CenterX() != cx) {
//    set_CenterX(cx);
//    m_CenterMarker -> setXValue(cx);
//
//    m_ImagePlot -> replot();
//
//    emit centerChanged(get_CenterX(), get_CenterY());
//  }
//}
//
//void QxrdCenterFinder::onCenterYChanged(double cy)
//{
//  if (get_CenterY() != cy) {
//    set_CenterY(cy);
//    m_CenterMarker -> setYValue(cy);
//
//    m_ImagePlot -> replot();
//
//    emit centerChanged(get_CenterX(), get_CenterY());
//  }
//}
//
//void QxrdCenterFinder::onCenterChanged(QwtDoublePoint pt)
//{
//  if (get_CenterX() != pt.x() || get_CenterY() != pt.y()) {
//    set_CenterX(pt.x());
//    set_CenterY(pt.y());
//    m_CenterMarker -> setValue(pt);
//
//    m_ImagePlot -> replot();
//
//    emit centerChanged(get_CenterX(), get_CenterY());
//  }
//}
//
//void QxrdCenterFinder::onCenterChanged(double cx, double cy)
//{
////  QxrdImageData *data = m_Acquisition -> data();
////
////  m_CenterFinderPlot -> onCenterChanged(data, cx, cy);
//}
//
//void QxrdCenterFinder::onCenterStepChanged(double stp)
//{
//  set_CenterStep(stp);
//}

/******************************************************************
*
*  $Log: qxrdcenterfinder.cpp,v $
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

