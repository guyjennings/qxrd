#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <QMetaMethod>
#include <qwt_legend.h>
#include <qwt_legend_label.h>
#include <stdio.h>
//#include <QTime>
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdplotmeasurer.h"
#include "qxrdintegrateddata.h"
#include "qwt_plot_piecewise_curve.h"

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_ObjectNamer(this, "integratorGraph"),
    m_DataProcessor(),
    m_Integrator(),
    m_PlotIndex(0),
    m_XUnitsLabel("")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");


}

void QxrdIntegratorPlot::init(QxrdPlotSettingsWPtr settings)
{
  QxrdPlot::init(settings);

  if (m_Legend) {
    insertLegend(m_Legend, QwtPlot::BottomLegend);

    connect(m_Legend, SIGNAL(clicked(const QVariant &,int)),      this, SLOT(onLegendClicked(const QVariant&, int)));
    connect(m_Legend, SIGNAL(checked(const QVariant &,bool,int)), this, SLOT(onLegendChecked(const QVariant&, bool, int)));
  }
}

void QxrdIntegratorPlot::setDataProcessor(QxrdDataProcessorWPtr proc)
{
  m_DataProcessor = proc;

  QxrdDataProcessorPtr dp(m_DataProcessor);

  if (dp) {
    m_Integrator = dp -> integrator();

    connect(m_Measurer, SIGNAL(selected(QVector<QPointF>)),
            dp.data(), SLOT(printMeasuredPolygon(QVector<QPointF>)));
  }
}

void QxrdIntegratorPlot::onNewIntegrationAvailable(QxrdIntegratedDataPtr data)
{

//  printf("New integration available, %d, %d points\n", x.size(), y.size());

  if (m_PlotIndex < 40) {
    QTime tic;
    tic.start();

    QString units = data->get_XUnitsLabel();

    if (m_PlotIndex == 0) {
      m_XUnitsLabel = units;
    } else if (units != m_XUnitsLabel) {
      m_XUnitsLabel = "";
    }

    setAxisTitle(QwtPlot::xBottom, m_XUnitsLabel);

    QxrdDoubleImageDataPtr img(data->get_Image());

    QString title = (img ? img -> get_Title() : "");
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

    updateZoomer();

    QxrdDataProcessorPtr proc(m_DataProcessor);

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

