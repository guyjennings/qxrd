/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.h,v 1.3 2009/07/21 22:55:48 jennings Exp $
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
      (QwtPlotCanvas *canvas);
private:
  HEADER_IDENT("$Id: qxrdcenterfinderpicker.h,v 1.3 2009/07/21 22:55:48 jennings Exp $");
};

#endif // QXRDCENTERFINDERPICKER_H

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.h,v $
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

