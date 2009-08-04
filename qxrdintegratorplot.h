/******************************************************************
*
*  $Id: qxrdintegratorplot.h,v 1.6 2009/08/04 20:42:53 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"

class QwtPlotZoomer;

class QxrdIntegratorPlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdIntegratorPlot(QWidget *parent=0);

public slots:
  void onNewIntegrationAvailable(QVector<double> x, QVector<double> y);

private:
  HEADER_IDENT("$Id: qxrdintegratorplot.h,v 1.6 2009/08/04 20:42:53 jennings Exp $");
};

#endif // QXRDINTEGRATORPLOT_H

/******************************************************************
*
*  $Log: qxrdintegratorplot.h,v $
*  Revision 1.6  2009/08/04 20:42:53  jennings
*  Simple, initial, implementation of integration
*
*  Revision 1.5  2009/07/25 15:18:19  jennings
*  Moved graph zooming code into QxrdPlot - a common base class
*
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

