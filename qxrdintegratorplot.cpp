/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.7 2009/08/04 20:42:53 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <qwt_plot_curve.h>
#include <QMetaMethod>

#include <stdio.h>

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(false, false, parent),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.7 2009/08/04 20:42:53 jennings Exp $")
{
  qRegisterMetaType< QVector<double> >("QVector<double>");
}

static int plotIndex=0;

void QxrdIntegratorPlot::onNewIntegrationAvailable(QVector<double> x, QVector<double> y)
{
  printf("New integration available, %d, %d points\n", x.size(), y.size());

  QwtPlotCurve *pc = new QwtPlotCurve(tr("Plot %1").arg(plotIndex++));
  pc -> setData(x.data(), y.data(), x.size());

  pc -> attach(this);
  replot();
}

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
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

