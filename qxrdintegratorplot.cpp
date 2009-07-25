/******************************************************************
*
*  $Id: qxrdintegratorplot.cpp,v 1.6 2009/07/25 15:18:19 jennings Exp $
*
*******************************************************************/

#include "qxrdintegratorplot.h"

#include <qwt_plot_zoomer.h>
#include <QMetaMethod>

QxrdIntegratorPlot::QxrdIntegratorPlot(QWidget *parent)
  : QxrdPlot(false, false, parent),
    SOURCE_IDENT("$Id: qxrdintegratorplot.cpp,v 1.6 2009/07/25 15:18:19 jennings Exp $")
{
}

/******************************************************************
*
*  $Log: qxrdintegratorplot.cpp,v $
*  Revision 1.6  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.5  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

