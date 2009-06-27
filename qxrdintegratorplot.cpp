/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.5 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <QMetaMethod>

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_Zoomer(NULL),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.5 2009/06/27 22:50:32 jennings Exp $")
{
  m_Zoomer = new QwtPlotZoomer(canvas());
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);
}

void QxrdIntegratorPlot::doZoomIn()
{
  m_Zoomer -> zoom(1);
}

void QxrdIntegratorPlot::doZoomOut()
{
  m_Zoomer -> zoom(-1);
}

void QxrdIntegratorPlot::doZoomAll()
{
  m_Zoomer -> zoom(0);
}

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

