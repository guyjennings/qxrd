#include "qxrdcentermarker.h"
#include "qxrdplotwidget.h"
#include "qxrdcenterfinder.h"

QxrdCenterMarker::QxrdCenterMarker(QString                    name,
                                   QxrdPlotWidget            *plot,
                                   QxrdPlotWidgetSettingsWPtr set,
                                   QxrdCenterFinderWPtr       cf) :
  QxrdPlotOverlay(name, plot, set),
  m_CenterFinder(cf),
  m_CenterMarker(NULL)
{
  m_CenterMarker = new QwtPlotMarker();
  m_CenterMarker -> setLineStyle(QwtPlotMarker::Cross);

  if (m_PlotWidget) {
    QcepPlot *plot = m_PlotWidget->plot();

    if (plot) {
      m_CenterMarker -> attach(plot);
    }
  }

  QxrdCenterFinderPtr c(m_CenterFinder);

  if (c) {
    connect(c->prop_Center(), &QcepDoublePointProperty::valueChanged,
            this,             &QxrdCenterMarker::centerChanged);

    centerChanged(c->get_Center());
  }
}

//TODO: make it work
void QxrdCenterMarker::centerChanged(QPointF c)
{
  if (m_CenterMarker) {
    m_CenterMarker -> setValue(c);

    if (m_PlotWidget) {
      QcepPlot *plot = m_PlotWidget->plot();

      if (plot) {
        plot -> replot();
      }
    }
  }
}

void QxrdCenterMarker::setPen(QPen pen)
{
  if (m_CenterMarker) {
    m_CenterMarker -> setLinePen(pen);

    if (m_PlotWidget) {
      QcepPlot *plot = m_PlotWidget->plot();

      if (plot) {
        plot -> replot();
      }
    }
  }
}
