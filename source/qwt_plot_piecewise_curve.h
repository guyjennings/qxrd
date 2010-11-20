/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#ifndef QWT_PLOT_PIECEWISE_CURVE_H
#define QWT_PLOT_PIECEWISE_CURVE_H

#include "qwt_plot_curve.h"

class QxrdPlot;

/*!
  \brief A class which draws piecewise curves

  This class can be used to display data with missing (NaN) values as a
  piecewise curve in the  x-y plane.
*/
class QWT_EXPORT QwtPlotPiecewiseCurve: public QwtPlotCurve
{
public:
  explicit QwtPlotPiecewiseCurve();
  explicit QwtPlotPiecewiseCurve(QxrdPlot *plot, const QwtText &title);
  explicit QwtPlotPiecewiseCurve(QxrdPlot *plot, const QString &title);

  virtual QwtDoubleRect boundingRect() const;

  virtual void draw(QPainter *p,
                    const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                    int from, int to) const;

private:
  static bool isNaN(double x);
  bool ignorePoint(double x, double y) const;
  QxrdPlot *m_Plot;
};

#endif
