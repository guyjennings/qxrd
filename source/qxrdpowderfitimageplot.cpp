/******************************************************************
*
*  $Id: qxrdpowderfitimageplot.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $
*
*******************************************************************/

#include "qxrdpowderfitimageplot.h"
#include "qwt_plot_zoomer.h"

QxrdPowderFitImagePlot::QxrdPowderFitImagePlot(QWidget *parent) :
    QxrdImagePlot(parent),
    SOURCE_IDENT("$Id: qxrdpowderfitimageplot.cpp,v 1.2 2010/09/13 20:00:41 jennings Exp $")
{
  m_Zoomer->setEnabled(false);

  set_DisplayMinimumPctle(2.0);
  set_DisplayMaximumPctle(98.0);

  set_DisplayScalingMode(PercentileMode);
}

/******************************************************************
*
*  $Log: qxrdpowderfitimageplot.cpp,v $
*  Revision 1.2  2010/09/13 20:00:41  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/09/10 21:09:52  jennings
*  Tilt and powder fitting modifications
*
*  Revision 1.1.2.1  2010/08/09 21:53:58  jennings
*  Made center and fit refiner into a modal dialog, partial implementation
*
*
*******************************************************************/

