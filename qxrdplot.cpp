/******************************************************************
*
*  $Id: qxrdplot.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#include "qxrdplot.h"

QxrdPlot::QxrdPlot(QWidget *parent)
  : QwtPlot(parent),
    SOURCE_IDENT("$Id: qxrdplot.cpp,v 1.2 2009/06/27 22:50:32 jennings Exp $")
{
}

void QxrdPlot::autoScalePlot()
{
  setAxisAutoScale(QwtPlot::yLeft);
  setAxisAutoScale(QwtPlot::xBottom);

  replot();
}


/******************************************************************
*
*  $Log: qxrdplot.cpp,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

