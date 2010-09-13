/******************************************************************
*
*  $Id: qxrdmaskpicker.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include <qwt_plot_picker.h>

class QxrdMaskPicker : public QwtPlotPicker
{
public:
  QxrdMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlotPtr m_Plot;
  HEADER_IDENT("$Id: qxrdmaskpicker.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

class QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);
};

class QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QwtPlotCanvasPtr canvas, QxrdImagePlotPtr plot);
};

#endif // QXRDMASKPICKER_H

/******************************************************************
*
*  $Log: qxrdmaskpicker.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:40  jennings
*  Moving files into source subdirectory
*
*  Revision 1.1.4.3  2010/06/07 20:17:42  jennings
*  Added more cvs log and ident stuff - removed qxrdspecserver.cpp & qxrdspecserver.h
*
*
*
*******************************************************************/
