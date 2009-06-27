/******************************************************************
*
*  $Id: qxrdplot.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPLOT_H
#define QXRDPLOT_H

#include "qcepmacros.h"

#include <qwt_plot.h>

class QxrdPlot : public QwtPlot
{
  Q_OBJECT;
public:
  QxrdPlot(QWidget *parent = 0);

public slots:
  void autoScalePlot();

private:
  HEADER_IDENT("$Id: qxrdplot.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDPLOT_H

/******************************************************************
*
*  $Log: qxrdplot.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

