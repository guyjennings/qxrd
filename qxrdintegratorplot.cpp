#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <QMetaMethod>

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_Zoomer(NULL)
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
