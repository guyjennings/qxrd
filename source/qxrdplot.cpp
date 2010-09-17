/******************************************************************
*
*  $Id: qxrdplot.cpp,v 1.3 2010/09/17 16:23:06 jennings Exp $
*
*******************************************************************/

#include "qxrdplot.h"

#include <qwt_plot_curve.h>
#include <qwt_plot_picker.h>
#include <qwt_plot_zoomer.h>
#include <qwt_plot_panner.h>
#include <qwt_plot_magnifier.h>
#include <qwt_symbol.h>
#include <qwt_legend.h>
#include "qxrdplotmeasurer.h"
#include "qxrdplotzoomer.h"
#include <stdio.h>
#include <qwt_scale_engine.h>

QxrdPlot::QxrdPlot(QWidget *parent)
  : QwtPlot(parent),
    m_Legend(NULL),
    m_Zoomer(NULL),
    m_Panner(NULL),
    m_Magnifier(NULL),
    m_Measurer(NULL),
    SOURCE_IDENT("$Id: qxrdplot.cpp,v 1.3 2010/09/17 16:23:06 jennings Exp $")
{
  setCanvasBackground(QColor(Qt::white));

  m_Zoomer = QxrdPlotZoomerPtr(new QxrdPlotZoomer(QwtPlotCanvasPtr(canvas()), QxrdPlotPtr(this)));
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::AlwaysOn);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
                              Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
                              Qt::LeftButton, Qt::ControlModifier);

  m_Zoomer -> setEnabled(true);

  m_Legend = QwtLegendPtr(new QwtLegend(this));
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  m_Panner = QwtPlotPannerPtr(new QwtPlotPanner(canvas()));
  m_Panner -> setEnabled(true);
  m_Panner -> setMouseButton(Qt::MidButton);
  m_Panner -> setAxisEnabled(QwtPlot::yRight, false);

  m_Magnifier = QwtPlotMagnifierPtr(new QwtPlotMagnifier(canvas()));
  m_Magnifier -> setEnabled(true);
  m_Magnifier -> setMouseButton(Qt::NoButton);
  m_Magnifier -> setAxisEnabled(QwtPlot::yRight, false);

  m_Measurer = QxrdPlotMeasurerPtr(new QxrdPlotMeasurer(QwtPlotCanvasPtr(canvas()), QxrdPlotPtr(this)));
  m_Measurer -> setEnabled(false);

  setAxisLabelRotation(QwtPlot::yLeft, -90);
  setAxisLabelAlignment(QwtPlot::yLeft, Qt::AlignVCenter);

  autoScale();
}

QxrdPlot::~QxrdPlot()
{
}

void QxrdPlot::setPlotCurveStyle(int index, QwtPlotCurvePtr curve)
{
  const int nColors = 10;
  const int nSymbols = 4;
  const int nStyles = 3;

  int colorIndex = index % nColors;
  int symbolIndex = (index / nColors) % nSymbols;
  int styleIndex = (index / (nColors * nSymbols)) % nStyles;

  QPen pen;
  QwtSymbol symb;
  QBrush brush;

  switch (colorIndex) {
  case 0:
    pen = QPen(Qt::black);
    break;
  case 1:
    pen = QPen(Qt::red);
    break;
  case 2:
    pen = QPen(Qt::green);
    break;
  case 3:
    pen = QPen(Qt::blue);
    break;
  case 4:
    pen = QPen(Qt::cyan);
    break;
  case 5:
    pen = QPen(Qt::magenta);
    break;
  case 6:
    pen = QPen(Qt::darkRed);
    break;
  case 7:
    pen = QPen(Qt::darkGreen);
    break;
  case 8:
    pen = QPen(Qt::darkBlue);
    break;
  case 9:
    pen = QPen(Qt::darkYellow);
    break;
  }

  switch (styleIndex) {
  case 0:
    pen.setStyle(Qt::SolidLine);
    break;
  case 1:
    pen.setStyle(Qt::DashLine);
    break;
  case 2:
    pen.setStyle(Qt::DotLine);
    break;
  case 3:
    pen.setStyle(Qt::DashDotLine);
    break;
  }

  symb.setPen(pen);
  symb.setBrush(QBrush(pen.color()));
  symb.setSize(7,7);

  switch (symbolIndex) {
  case 0:
    symb.setStyle(QwtSymbol::Ellipse);
    break;
  case 1:
    symb.setStyle(QwtSymbol::Rect);
    break;
  case 2:
    symb.setStyle(QwtSymbol::XCross);
    break;
  case 3:
    symb.setStyle(QwtSymbol::Cross);
    break;
  }

  curve -> setPen(pen);
  curve -> setSymbol(symb);
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

void QxrdPlot::enableZooming()
{
  m_Zoomer       -> setEnabled(true);
  m_Measurer     -> setEnabled(false);
}

void QxrdPlot::enableMeasuring()
{
  m_Zoomer   -> setEnabled(false);
  m_Measurer -> setEnabled(true);
}

void QxrdPlot::onLegendClicked(QwtPlotItem *item)
{
  emit printMessage(tr("QxrdPlot::onLegendClicked(%1)").arg(item->title().text()));
}

void QxrdPlot::onLegendChecked(QwtPlotItem *item, bool checked)
{
  emit printMessage(tr("QxrdPlot::onLegendChecked(%1,%2)").arg(item->title().text()).arg(checked));
}

void QxrdPlot::setLogAxis(int axis, int isLog)
{
  if (isLog) {
    setAxisScaleEngine(axis, new QwtLog10ScaleEngine);
  } else {
    setAxisScaleEngine(axis, new QwtLinearScaleEngine);
  }

  replot();
}

QwtText QxrdPlot::trackerText(const QwtDoublePoint &pos) const
{
  return tr("%1, %2").arg(pos.x()).arg(pos.y());
}

/******************************************************************
*
*  $Log: qxrdplot.cpp,v $
*  Revision 1.3  2010/09/17 16:23:06  jennings
*  Added log axis command for scripting purposes
*
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:41  jennings
*  Moving files into source subdirectory
*
*  Revision 1.9.2.4  2010/06/18 16:29:13  jennings
*  Added an (empty) destructor to QxrdPlot - wanted to check that plots were being deleted properly
*
*  Revision 1.9.2.3  2010/05/25 22:06:53  jennings
*  Added extra plot curve colors - now 10
*
*  Revision 1.9.2.2  2010/05/06 18:52:42  jennings
*  Make plot legends owned by plot
*
*  Revision 1.9.2.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.9  2009/11/12 20:17:43  jennings
*  Version 0.3.5, fix problems with first-time zooming of graphs
*
*  Revision 1.8  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.7  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.6  2009/08/11 20:53:42  jennings
*  Added automatic plot style options to plot curves
*
*  Revision 1.5  2009/08/09 15:40:32  jennings
*  Added measurer tool to all graphs
*
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

