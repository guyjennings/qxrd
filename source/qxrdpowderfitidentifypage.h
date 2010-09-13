/******************************************************************
*
*  $Id: qxrdpowderfitidentifypage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITIDENTIFYPAGE_H
#define QXRDPOWDERFITIDENTIFYPAGE_H

#include "qxrdpowderfitwidget.h"
#include "qwt_plot_picker.h"

class QxrdPowderFitIdentifyPage : public QxrdPowderFitWidget
{
  Q_OBJECT;
public:
  QxrdPowderFitIdentifyPage(QxrdDataProcessorPtr proc, QWidget *parent);

  Q_PROPERTY(int    idCurrentRing     READ get_IdCurrentRing WRITE set_IdCurrentRing STORED false);
  QCEP_INTEGER_PROPERTY(IdCurrentRing);

public slots:
  void selectIdentifyPage();
  void pointClicked(QwtDoublePoint pt);

  void appendRing();
  void removeRing();
  void selectRing(int n);
  void selectNextRing();
  void selectPrevRing();
  void appendPoint(QwtDoublePoint pt);
  void idPerformFit();

public:
  void evaluateFit(double *p, double *x, int np, int nx);

private:
  QwtPlotPicker            *m_Picker;
  QxrdRingSetSampledDataPtr m_SampledData;
  QList<QxrdFitParameter*>  m_FittedParameters;
  QList<QxrdFitParameter*>  m_FixedParameters;
  QVector<double>           m_FitResult;

  HEADER_IDENT("$Id: qxrdpowderfitidentifypage.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITIDENTIFYPAGE_H

/******************************************************************
*
*  $Log: qxrdpowderfitidentifypage.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.5  2010/09/11 21:44:03  jennings
*  Work on powder ring fitting
*
*  Revision 1.1.2.4  2010/08/19 16:16:32  jennings
*  Support for fit parameter 'isFitted' flag
*
*  Revision 1.1.2.3  2010/08/15 20:53:34  jennings
*  Added evaluateFit function for initial ring fitting
*
*  Revision 1.1.2.2  2010/08/13 21:57:36  jennings
*  A little more powder fit implementation
*
*  Revision 1.1.2.1  2010/08/10 20:41:55  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*
*******************************************************************/
