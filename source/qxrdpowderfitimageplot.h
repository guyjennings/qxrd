/******************************************************************
*
*  $Id: qxrdpowderfitimageplot.h,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#ifndef QXRDPOWDERFITIMAGEPLOT_H
#define QXRDPOWDERFITIMAGEPLOT_H

#include "qxrdimageplot.h"

class QxrdPowderFitImagePlot : public QxrdImagePlot
{
  Q_OBJECT;
public:
  explicit QxrdPowderFitImagePlot(QWidget *parent = 0);

  void setRingSetSampleMarkers(QxrdRingSetSampledDataPtr p);
  void setRingSampleMarkers(int n);
  void showRingSetSampleMarkers(bool show);

signals:

public slots:

private:
  QxrdRingSetSampledDataPtr m_RingSamples;
  int                       m_Ring;
  bool                      m_RingsSamplesShown;

  HEADER_IDENT("$Id: qxrdpowderfitimageplot.h,v 1.2 2010/09/13 20:00:41 jennings Exp $");
};

#endif // QXRDPOWDERFITIMAGEPLOT_H

/******************************************************************
*
*  $Log: qxrdpowderfitimageplot.h,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/10 20:41:55  jennings
*  Split powder fit dialog into separate sub classes to handle each 'wizard' step
*
*  Revision 1.1.2.1  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*
*******************************************************************/

