/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.9 2009/08/05 16:44:08 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <QMetaMethod>
#include <qwt_legend.h>
#include <stdio.h>
//#include <QTime>

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(false, false, parent),
    m_Legend(NULL),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.9 2009/08/05 16:44:08 jennings Exp $")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");

  m_Legend = new QwtLegend();
  m_Legend -> setItemMode(QwtLegend::CheckableItem);

  insertLegend(m_Legend, QwtPlot::BottomLegend);
}

static int plotIndex=0;

void QxrdIntegratorPlot::onNewIntegrationAvailable(QVector<double> x, QVector<double> y)
{
//  QTime tic;
//  tic.start();

//  printf("New integration available, %d, %d points\n", x.size(), y.size());

  QwtPlotCurve *pc = new QwtPlotCurve(tr("Plot %1").arg(plotIndex++));
  pc -> setData(x.data(), y.data(), x.size());
  pc -> setPen(QPen(QColor::fromHsv(plotIndex*67, 255, 255)));
  pc -> attach(this);
  replot();
//
//  printf("Plotting took %d msec\n", tic.restart());
}

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
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

