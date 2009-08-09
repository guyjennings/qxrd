/******************************************************************
*
*  $Id: qxrdimageplotmeasurer.h,v 1.1 2009/08/09 15:38:29 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qcepmacros.h"

#include "qxrdplotmeasurer.h"

#include <qwt_plot_picker.h>
class QxrdImagePlot;

class QxrdImagePlotMeasurer : public QxrdPlotMeasurer
{
public:
  QxrdImagePlotMeasurer(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdimageplotmeasurer.h,v 1.1 2009/08/09 15:38:29 jennings Exp $");
};

#endif // QXRDIMAGEPLOTMEASURER_H

/******************************************************************
*
*  $Log: qxrdimageplotmeasurer.h,v $
*  Revision 1.1  2009/08/09 15:38:29  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*
*******************************************************************/

