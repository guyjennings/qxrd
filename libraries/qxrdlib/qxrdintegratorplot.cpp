#include "qxrdintegratorplot.h"

#include <stdio.h>

#include <QMetaMethod>
//#include <QTime>

#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <qwt_legend.h>
#include <qwt_legend_label.h>

#include "qcepplotmeasurer.h"
#include "qcepintegrateddata.h"

#include "qxrdprocessor.h"
#include "qxrdintegrator.h"
#include "qwt_plot_piecewise_curve.h"

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QcepPlot(parent),
    m_Processor(),
    m_Integrator(),
    m_PlotIndex(0),
    m_XUnitsLabel("")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");


}

void QxrdIntegratorPlot::init(QcepPlotSettingsWPtr settings)
{
  QcepPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);
  }
}

void QxrdIntegratorPlot::setProcessor(QxrdProcessorWPtr proc)
{
  m_Processor = proc;

  QxrdProcessorPtr dp(m_Processor);

  if (dp) {
    m_Integrator = dp -> integrator();

//    connect(m_Measurer, (void (QcepPlotMeasurer::*)( const QVector<QPointF> &)) &QwtPlotPicker::selected,
//            dp.data(), &QxrdProcessor::printMeasuredPolygon);
  }
}

void QxrdIntegratorPlot::onNewIntegrationAvailable(QcepIntegratedDataPtr data)
{

//  printf("New integration available, %d, %d points\n", x.size(), y.size());

  if (data && m_PlotIndex < 40) {
    QTime tic;
    tic.start();

    QString units = data->get_XUnitsLabel();

    if (m_PlotIndex == 0) {
      m_XUnitsLabel = units;
    } else if (units != m_XUnitsLabel) {
      m_XUnitsLabel = "";
    }

    setAxisTitle(QwtPlot::xBottom, m_XUnitsLabel);

    QString title = data -> get_Name();
    QString tooltip;

    if (data->get_Oversample() > 1) {
      tooltip = tr("%1\nPlotted vs %2\n%3 x oversampled")
          .arg(title)
          .arg(units)
          .arg(data->get_Oversample());
    } else {
      tooltip = tr("%1\nPlotted vs %2\nNo oversampling")
          .arg(title)
          .arg(units);
    }

    QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(this, title/*tr("Plot %1").arg(m_PlotIndex)*/);
    pc -> setSamples(data->x(), data->y(), data->size());
    setPlotCurveStyle(m_PlotIndex, pc);
    pc -> attach(this);
    pc -> setLegendAttribute(QwtPlotCurve::LegendShowSymbol, true);
    pc -> setLegendAttribute(QwtPlotCurve::LegendShowLine, true);

    updateZoomer();

    QxrdProcessorPtr proc(m_Processor);

    if (proc) {
      proc -> updateEstimatedTime(proc -> prop_DisplayIntegratedDataTime(), tic.restart());
    }

    QWidget *legend = m_Legend->legendWidget(itemToInfo(pc));

    if (legend) {
      legend->setToolTip(tooltip);
    }
  }

  m_PlotIndex++;
//
//  printf("Plotting took %d msec\n", tic.restart());
}

void QxrdIntegratorPlot::clearGraph()
{
  m_PlotIndex = 0;

  detachItems(QwtPlotItem::Rtti_PlotCurve);
  detachItems(QwtPlotItem::Rtti_PlotMarker);

  replot();
}

void QxrdIntegratorPlot::clearSelectedCurves()
{
  QList<QwtPlotCurve*> toDelete;

  foreach(QwtPlotItem* item, itemList()) {
    QwtPlotCurve *pc = dynamic_cast<QwtPlotCurve*>(item);
    if (pc) {
      QWidget *wid = m_Legend->legendWidget(itemToInfo(pc));

      if (wid) {
        QwtLegendLabel *itm = qobject_cast<QwtLegendLabel*>(wid);

        if (itm) {
          if (itm->isChecked()) {
            toDelete.append(pc);
          }
        }
      }
    }
  }

  foreach(QwtPlotCurve *curve, toDelete) {
    if (curve) {
      curve->detach();

      delete curve;
    }
  }

  replot();
}

