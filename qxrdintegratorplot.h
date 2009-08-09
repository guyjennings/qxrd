/******************************************************************
*
*  $Id: qxrdintegratorplot.h,v 1.8 2009/08/09 15:39:10 jennings Exp $
*
*******************************************************************/

#ifndef QXRDINTEGRATORPLOT_H
#define QXRDINTEGRATORPLOT_H

#include "qcepmacros.h"

#include "qxrdplot.h"

class QwtPlotZoomer;
class QxrdDataProcessor;
class QxrdIntegrator;

class QxrdIntegratorPlot : public QxrdPlot
{
  Q_OBJECT;

public:
  QxrdIntegratorPlot(QWidget *parent=0);

  void setDataProcessor(QxrdDataProcessor *proc);

public slots:
  void onNewIntegrationAvailable(QVector<double> x, QVector<double> y);

private:
  QwtLegend           *m_Legend;
  QxrdDataProcessor   *m_DataProcessor;
  QxrdIntegrator      *m_Integrator;

  HEADER_IDENT("$Id: qxrdintegratorplot.h,v 1.8 2009/08/09 15:39:10 jennings Exp $");
};

#endif // QXRDINTEGRATORPLOT_H

/******************************************************************
*
*  $Log: qxrdintegratorplot.h,v $
*  Revision 1.8  2009/08/09 15:39:10  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*  Revision 1.7  2009/08/04 22:03:31  jennings
*  Moved integration code into QxrdIntegrator, added oversampling option
*  Add each integration result to the az-avg plot panel
*
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

