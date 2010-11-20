/* -*- mode: C++ ; c-file-style: "stroustrup" -*- *****************************
 * Qwt Widget Library
 * Copyright (C) 1997   Josef Wilgen
 * Copyright (C) 2002   Uwe Rathmann
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the Qwt License, Version 1.0
 *****************************************************************************/

#include "qwt_plot_piecewise_curve.h"
#include "qxrdplot.h"

QwtPlotPiecewiseCurve::QwtPlotPiecewiseCurve(): QwtPlotCurve(),
    m_Plot(NULL)
{
}

QwtPlotPiecewiseCurve::QwtPlotPiecewiseCurve(QxrdPlot *plot, const QwtText &title):
    QwtPlotCurve(title),
    m_Plot(plot)
{
}

QwtPlotPiecewiseCurve::QwtPlotPiecewiseCurve(QxrdPlot *plot, const QString &title):
    QwtPlotCurve(title),
    m_Plot(plot)
{
}

bool QwtPlotPiecewiseCurve::isNaN(double x)
{
  return x != x;
}

bool QwtPlotPiecewiseCurve::ignorePoint(double x, double y) const
{
  if (isNaN(x)) return true;

  if (isNaN(y)) return true;

  if (m_Plot) {
    if (m_Plot->logAxis(xAxis()) && (x <= 0)) return true;
    if (m_Plot->logAxis(yAxis()) && (y <= 0)) return true;
  }

  return false;
}

// This is a slow implementation: it might be worth to cache valid data ranges.
void QwtPlotPiecewiseCurve::draw(QPainter *painter,
                                 const QwtScaleMap &xMap, const QwtScaleMap &yMap,
                                 int from, int to) const
{
  if (to < 0) {
    to = dataSize() - 1;
  }

  int first, last = from;
  while (last <= to) {
    first = last;
    while (first <= to && ignorePoint(x(first),y(first))) {
      ++first;
    }

    last = first;
    while (last <= to && !ignorePoint(x(last),y(last))) {
      ++last;
    }

    if (first <= to) {
      QwtPlotCurve::draw(painter, xMap, yMap, first, last - 1);
    }
  }
}

// This overload is needed when using autoscale. It is a slow implementation:
// it might be worth to cache valid data ranges.
QwtDoubleRect QwtPlotPiecewiseCurve::boundingRect() const
{
  if (dataSize() <= 0) {
    return QwtDoubleRect(1.0, 1.0, 2.0, 2.0); // Empty data.
  }

  int first = 0;
  while (first < dataSize() && ignorePoint(x(first),y(first))) {
    ++first;
  }

  if (first == dataSize()) {
    return QwtDoubleRect(1.0, 1.0, 2.0, 2.0); // Empty data.
  }

  double minX, maxX, minY, maxY;
  minX = maxX = x(first);
  minY = maxY = y(first);
  for (int i = first + 1; i < dataSize(); ++i) {
    const double xv = x(i);
    const double yv = y(i);

    if (!ignorePoint(xv,yv)) {
      if (xv < minX)
        minX = xv;
      if (xv > maxX)
        maxX = xv;
      if (yv < minY)
        minY = yv;
      if (yv > maxY)
        maxY = yv;
    }
  }

  return QwtDoubleRect(minX, minY, maxX - minX, maxY - minY);
}
