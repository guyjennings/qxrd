#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <QMetaMethod>
#include <qwt_legend.h>
#include <stdio.h>
//#include <QTime>
#include "qxrddataprocessor.h"
#include "qxrdintegrator.h"
#include "qxrdplotmeasurer.h"
#include "qxrdintegrateddata.h"
#include "qwt_plot_piecewise_curve.h"

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_DataProcessor(NULL),
    m_Integrator(NULL),
    m_PlotIndex(0),
    m_XUnitsLabel("")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  connect(this, SIGNAL(legendClicked(QwtPlotItem*)), this, SLOT(onLegendClicked(QwtPlotItem*)));
  connect(this, SIGNAL(legendChecked(QwtPlotItem*,bool)), this, SLOT(onLegendChecked(QwtPlotItem*,bool)));
}

void QxrdIntegratorPlot::setDataProcessor(QxrdDataProcessor *proc)
{
  m_DataProcessor = proc;
  m_Integrator = m_DataProcessor -> integrator();

  connect(m_Measurer, SIGNAL(selected(QwtArray<QwtDoublePoint>)),
          m_DataProcessor, SLOT(printMeasuredPolygon(QwtArray<QwtDoublePoint>)));
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

    const QString title = data -> get_Image() -> get_Title();

    QwtPlotCurve *pc = new QwtPlotPiecewiseCurve(this, title/*tr("Plot %1").arg(m_PlotIndex)*/);
    pc -> setData(data->x(), data->y(), data->size());
    setPlotCurveStyle(m_PlotIndex, pc);
    pc -> attach(this);

    updateZoomer();

    m_DataProcessor -> updateEstimatedTime(m_DataProcessor -> prop_DisplayIntegratedDataTime(), tic.restart());
  }

  m_PlotIndex++;
//
//  printf("Plotting took %d msec\n", tic.restart());
}

void QxrdIntegratorPlot::clearGraph()
{
  m_PlotIndex = 0;

  clear();
  replot();
}
