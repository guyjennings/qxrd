/******************************************************************
*
*  $Id: qxrdmaskpicker.h,v 1.1 2009/08/02 18:02:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDMASKPICKER_H
#define QXRDMASKPICKER_H

#include "qcepmacros.h"

#include <qwt_plot_picker.h>

class QxrdImagePlot;

class QxrdMaskPicker : public QwtPlotPicker
{
public:
  QxrdMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);

public:
  virtual QwtText trackerText(const QwtDoublePoint &pos) const;

private:
  QxrdImagePlot   *m_Plot;
  HEADER_IDENT("$Id: qxrdmaskpicker.h,v 1.1 2009/08/02 18:02:42 jennings Exp $");
};

class QxrdCircularMaskPicker : public QxrdMaskPicker
{
public:
  QxrdCircularMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
};

class QxrdPolygonalMaskPicker : public QxrdMaskPicker
{
public:
  QxrdPolygonalMaskPicker(QwtPlotCanvas *canvas, QxrdImagePlot *plot);
};

#endif // QXRDMASKPICKER_H
