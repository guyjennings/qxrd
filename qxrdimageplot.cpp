#include "qxrdimageplot.h"

#include <qwt_plot_zoomer.h>
#include <qwt_legend.h>

QxrdImagePlot::QxrdImagePlot(QWidget *parent)
  : QwtPlot(parent),
    m_Zoomer(NULL),
    m_Legend(NULL)
{
  setCanvasBackground(QColor(Qt::white));

  m_Zoomer = new QwtPlotZoomer(QwtPlot::xBottom, QwtPlot::yLeft, canvas());
  m_Zoomer -> setSelectionFlags(QwtPicker::DragSelection | QwtPicker::CornerToCorner);
  m_Zoomer -> setTrackerMode(QwtPicker::ActiveOnly);
  m_Zoomer -> setRubberBand(QwtPicker::RectRubberBand);

  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect2,
			      Qt::LeftButton, Qt::ControlModifier | Qt::ShiftModifier);
  m_Zoomer -> setMousePattern(QwtEventPattern::MouseSelect3,
			      Qt::LeftButton, Qt::ControlModifier);

  m_Legend = new QwtLegend;
  m_Legend -> setFrameStyle(QFrame::Box|QFrame::Sunken);
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}

