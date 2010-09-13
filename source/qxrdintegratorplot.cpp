/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $
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
#include "qxrdintegrateddata.h"

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(parent),
    m_DataProcessor(NULL),
    m_Integrator(NULL),
    m_PlotIndex(0),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.2 2010/09/13 20:00:40 jennings Exp $")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");

  insertLegend(m_Legend, QwtPlot::BottomLegend);

  connect(this, SIGNAL(legendClicked(QwtPlotItem*)), this, SLOT(onLegendClicked(QwtPlotItem*)));
  connect(this, SIGNAL(legendChecked(QwtPlotItem*,bool)), this, SLOT(onLegendChecked(QwtPlotItem*,bool)));
}

void QxrdIntegratorPlot::setDataProcessor(QxrdDataProcessorPtr proc)
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

    const QString title = data -> get_Image() -> get_Title();

    QwtPlotCurvePtr pc = QwtPlotCurvePtr(new QwtPlotCurve(title/*tr("Plot %1").arg(m_PlotIndex)*/));
    pc -> setData(data->x(), data->y(), data->size());
    setPlotCurveStyle(m_PlotIndex, pc);
    pc -> attach(this);

    if (m_Zoomer && m_Zoomer -> zoomRectIndex() == 0) {
      m_Zoomer -> setZoomBase();
    }

    replot();

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

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.16.2.7  2010/07/20 20:30:25  jennings
*  Added memory usage display to status bar
*  Improved calculation of processing timings
*
*  Revision 1.16.2.6  2010/06/14 20:18:00  jennings
*  Implemented plotting and saving of integrated data - still need some kind of 'serializer' to maintain order of saving and plotting
*
*  Revision 1.16.2.5  2010/05/02 16:56:50  jennings
*  Removed embedded C comments from commit log
*
*  Revision 1.16.2.4  2010/05/02 08:12:06  jennings
*  Replaced 'returnImageToPool' and 'replaceImageFromPool' by
*  equivalent smart pointer assignments
*
*  Revision 1.16.2.3  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.16.2.2  2010/04/26 20:53:26  jennings
*  More attempts to get QSharedPointers to work...
*
*  Revision 1.16.2.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.16  2009/11/12 20:17:43  jennings
*  Version 0.3.5, fix problems with first-time zooming of graphs
*
*  Revision 1.15  2009/10/02 20:18:35  jennings
*  Added support for (optionally) saving and/or displaying integrated data
*
*  Revision 1.14  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
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

