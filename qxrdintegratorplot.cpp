/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.13 2009/08/12 19:44:59 jennings Exp $
*
*******************************************************************/

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

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_DataProcessor(NULL),
    m_Integrator(NULL),
    m_PlotIndex(0),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.13 2009/08/12 19:44:59 jennings Exp $")
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

void QxrdIntegratorPlot::onNewIntegrationAvailable(QVector<double> x, QVector<double> y)
{
//  QTime tic;
//  tic.start();

//  printf("New integration available, %d, %d points\n", x.size(), y.size());

  QwtPlotCurve *pc = new QwtPlotCurve(tr("Plot %1").arg(m_PlotIndex));
  pc -> setData(x.data(), y.data(), x.size());
  setPlotCurveStyle(m_PlotIndex, pc);
  pc -> attach(this);
  replot();

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

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
*  Revision 1.13  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.12  2009/08/11 20:53:42  jennings
*  Added automatic plot style options to plot curves
*
*  Revision 1.11  2009/08/09 18:00:00  jennings
*  Added graph clearing button to integrator dialog
*
*  Revision 1.10  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.9  2009/08/05 16:44:08  jennings
*  More changes to oversampling code for integration
*
*  Revision 1.8  2009/08/04 22:03:31  jennings
*  Moved integration code into QxrdIntegrator, added oversampling option
*  Add each integration result to the az-avg plot panel
*
*  Revision 1.7  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.6  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

