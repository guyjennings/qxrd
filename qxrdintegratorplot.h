/******************************************************************
*
*  $Id: qxrdintegratorplot.h,v 1.12 2009/09/22 18:19:00 jennings Exp $
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
  void onNewIntegrationAvailable(QString title, QVector<double> x, QVector<double> y);
  void clearGraph();

private:
  QxrdDataProcessor   *m_DataProcessor;
  QxrdIntegrator      *m_Integrator;
  int                  m_PlotIndex;

  HEADER_IDENT("$Id: qxrdintegratorplot.h,v 1.12 2009/09/22 18:19:00 jennings Exp $");
};

#endif // QXRDINTEGRATORPLOT_H

/******************************************************************
*
*  $Log: qxrdintegratorplot.h,v $
*  Revision 1.12  2009/09/22 18:19:00  jennings
*  Added slicing routines
*  Set title for traces in avg data graph
*
*  Revision 1.11  2009/08/12 19:44:59  jennings
*  Reorganized plot zoomers into a single class, initialized in QxrdPlot, which
*  takes its tracker text from a QxrdPlot virtual member function
*
*  Revision 1.10  2009/08/11 20:53:42  jennings
*  Added automatic plot style options to plot curves
*
*  Revision 1.9  2009/08/09 18:00:00  jennings
*  Added graph clearing button to integrator dialog
*
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

