/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.h,v 1.2 2009/06/27 22:50:32 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include "qcepmacros.h"

#include <qwt_plot_picker.h>

class QxrdImagePlot;
class QxrdCenterFinderDialog;

class QxrdCenterFinderPicker : public QwtPlotPicker
{
public:
  QxrdCenterFinderPicker
      (QwtPlotCanvas *canvas, QxrdImagePlot *plot, QxrdCenterFinderDialog *dlog);
private:
  QxrdImagePlot          *m_Plot;
  QxrdCenterFinderDialog *m_Dialog;
  HEADER_IDENT("$Id: qxrdcenterfinderpicker.h,v 1.2 2009/06/27 22:50:32 jennings Exp $");
};

#endif // QXRDCENTERFINDERPICKER_H

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.h,v $
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

