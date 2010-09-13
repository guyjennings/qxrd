/******************************************************************
*
*  $Id: qxrdimageplotmeasurer.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDIMAGEPLOTMEASURER_H
#define QXRDIMAGEPLOTMEASURER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qxrdplotmeasurer.h"

#include <qwt_plot_picker.h>

class QxrdImagePlotMeasurer : public QxrdPlotMeasurer
{
public:
  QxrdImagePlotMeasurer(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdimageplotmeasurer.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDIMAGEPLOTMEASURER_H

/******************************************************************
*
*  $Log: qxrdimageplotmeasurer.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:39  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.1.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.1  2009/08/09 15:38:29  jennings
*  Added a separate QxrdImagePlotMeasurer class
*
*
*******************************************************************/

