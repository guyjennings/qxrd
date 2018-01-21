#include "qxrdcentermarker.h"
#include "qxrdplotwidget.h"
#include "qxrdcenterfinder.h"

QxrdCenterMarker::QxrdCenterMarker(QString                    name,
                                   QxrdPlotWidgetWPtr         plot,
                                   QxrdPlotWidgetSettingsWPtr set,
                                   QxrdCenterFinderWPtr       cf) :
  QxrdPlotOverlay(name, plot, set),
  m_CenterFinder(cf),
  m_CenterMarker(NULL)
{
  QxrdPlotWidgetPtr pw(m_PlotWidget);
  m_CenterMarker = new QwtPlotMarker();
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);

  if (pw) {
    QcepPlot *plot = pw->plot();

    if (plot) {
      m_CenterMarker -> attach(plot);
    }
  }

  QxrdCenterFinderPtr c(m_CenterFinder);

  if (c) {
    connect(c->prop_Center(), &QcepDoublePointProperty::valueChanged,
            this,             &QxrdCenterMarker::centerChanged);
  }
}

//TODO: make it work
void QxrdCenterMarker::centerChanged(QPointF c)
{
  if (m_CenterMarker) {
    m_CenterMarker -> setValue(c);
  }
}

void QxrdCenterMarker::setPen(const QPen &pen)
{
  if (m_CenterMarker) {
    m_CenterMarker -> setLinePen(pen);
  }
}
