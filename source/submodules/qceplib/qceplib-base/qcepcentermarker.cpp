#include "qcepcentermarker.h"
#include "qcepplotwidget.h"
#include "qcepcenterfinder.h"

QcepCenterMarker::QcepCenterMarker(QString                    name,
                                   QcepPlotWidget            *plot,
                                   QcepPlotWidgetSettingsWPtr set,
                                   QcepCenterFinderWPtr       cf) :
  QcepPlotOverlay(name, plot, set),
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

  QcepCenterFinderPtr c(m_CenterFinder);

  if (c) {
    connect(c->prop_Center(), &QcepDoublePointProperty::valueChanged,
            this,             &QcepCenterMarker::centerChanged);

    centerChanged(c->get_Center());
  }
}

//TODO: make it work
void QcepCenterMarker::centerChanged(QPointF c)
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

void QcepCenterMarker::setPen(QPen pen)
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
