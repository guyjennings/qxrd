/******************************************************************
*
*  $Id: qxrdintegratorplot.h,v 1.4 2009/06/27 22:50:32 jennings Exp $
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
  void doZoomIn();
  void doZoomOut();
  void doZoomAll();

private:
  QwtPlotZoomer   *m_Zoomer;
  HEADER_IDENT("$Id: qxrdintegratorplot.h,v 1.4 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDINTEGRATORPLOT_H

/******************************************************************
*
*  $Log: qxrdintegratorplot.h,v $
*  Revision 1.4  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

