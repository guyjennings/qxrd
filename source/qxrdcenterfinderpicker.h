/******************************************************************
*
*  $Id: qxrdcenterfinderpicker.h,v 1.3 2010/09/17 16:21:51 jennings Exp $
*
*******************************************************************/

#ifndef QXRDCENTERFINDERPICKER_H
#define QXRDCENTERFINDERPICKER_H

#include "qcepmacros.h"
#include "qxrdforwardtypes.h"
#include "qxrdimageplotmeasurer.h"

class QxrdCenterFinderPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdCenterFinderPicker(QxrdImagePlotPtr plot);

public:
//  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;

  HEADER_IDENT("$Id: qxrdcenterfinderpicker.h,v 1.3 2010/09/17 16:21:51 jennings Exp $");
};

#endif // QXRDCENTERFINDERPICKER_H

/******************************************************************
*
*  $Log: qxrdcenterfinderpicker.h,v $
*  Revision 1.3  2010/09/17 16:21:51  jennings
*  Rationalised the trackerText implementations
*
*  Revision 1.2  2010/09/13 20:00:39  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:38  jennings
*  Moving files into source subdirectory
*
*  Revision 1.4.4.2  2010/04/26 23:46:12  jennings
*  *** empty log message ***
*
*  Revision 1.4.4.1  2010/04/26 00:37:10  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.4  2009/07/22 11:55:34  jennings
*  Center finder modifications
*
*  Revision 1.3  2009/07/21 22:55:48  jennings
*  Rearranged center finder and integrator code so that the center finder and integrator objects go into the data processor thread, and the GUI stuff goes in the GUI thread
*
*  Revision 1.2  2009/06/27 22:50:32  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

