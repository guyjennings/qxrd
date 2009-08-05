/******************************************************************
*
*  $Id: qxrdplot.cpp,v 1.4 2009/08/05 14:04:33 jennings Exp $
*
*******************************************************************/

#include "qxrdplot.h"

#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>

QxrdPlot::QxrdPlot(bool customTracker, bool customZoomer, QWidget *parent)
  : QwtPlot(parent),
    m_Tracker(NULL),
    m_Zoomer(NULL),
    m_Panner(NULL),
    m_Magnifier(NULL),
    SOURCE_IDENT("$Id: qxrdplot.cpp,v 1.4 2009/08/05 14:04:33 jennings Exp $")
{
  setCanvasBackground(QColor(Qt::white));

  if (!customTracker) {
    setCustomTracker(NULL);
  }

  if (!customZoomer) {
    setCustomZoomer(NULL);
  }

  m_Panner = new QwtPlotPanner(canvas());
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = new QwtPlotMagnifier(canvas());
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  setAxisLabelRotation(QwtPlot::yLeft, -90);
  setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);
}

void QxrdPlot::setCustomTracker(QwtPlotPicker *tracker)
{
  if (tracker == NULL) {
    m_Tracker = new QwtPlotPicker(canvas());
  } else {
    m_Tracker = tracker;
  }

  m_Tracker -> setSelectionFlags(QwtPicker::PointSelection);
  m_Tracker -> setEnabled(true);
}

void QxrdPlot::setCustomZoomer(QwtPlotZoomer *zoomer)
{
  if (zoomer == NULL) {
    m_Zoomer = new QwtPlotZoomer(canvas(), this);
  } else {
    m_Zoomer = zoomer;
  }

  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
//  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);
}

void QxrdPlot::autoScale()
{
  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

  m_Zoomer -> setZoomBase();

//
//  replot();
}

void QxrdPlot::zoomIn()
{
  m_Zoomer -> zoom(1);
}

void QxrdPlot::zoomOut()
{
  m_Zoomer -> zoom(-1);
}

/******************************************************************
*
*  $Log: qxrdplot.cpp,v $
*  Revision 1.4  2009/08/05 14:04:33  jennings
*  Turned off right hand color bar
*  Rotated left axis labels
*
*  Revision 1.3  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

