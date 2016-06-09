#ifndef QXRDROIPICKER_H
#define QXRDROIPICKER_H

#include "qxrdimageplotmeasurer.h"

class QxrdROIPicker : public QxrdImagePlotMeasurer
{
public:
  QxrdROIPicker(QWidget *canvas, QxrdImagePlot *plot);

//public slots:
//  void pickerActivated(bool active);
//  void selectedPoint(const QPointF &pt);
//  void selectedRect(const QRectF &r);
//  void selectedPoly(const QVector<QPointF> &r);
//  void appendedPoint(const QPointF &pt);
//  void movedPoint(const QPointF &pt);
};

class QxrdROISelector : public QxrdROIPicker
{
public:
  QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIAddNode : public QxrdROIPicker
{
public:
  QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIRemoveNode : public QxrdROIPicker
{
public:
  QxrdROIRemoveNode(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIRotator : public QxrdROIPicker
{
public:
  QxrdROIRotator(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIResizer : public QxrdROIPicker
{
public:
  QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot);
};

#endif // QXRDROIPICKER_H
