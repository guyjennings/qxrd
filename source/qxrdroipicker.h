#ifndef QXRDROIPICKER_H
#define QXRDROIPICKER_H

#include "qxrdimageplotmeasurer.h"
#include <QDateTime>

class QxrdROIPicker : public QxrdImagePlotMeasurer
{
  Q_OBJECT

public:
  QxrdROIPicker(QWidget *canvas, QxrdImagePlot *plot);

  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
//public slots:
//  void pickerActivated(bool active);
//  void selectedPoint(const QPointF &pt);
//  void selectedRect(const QRectF &r);
//  void selectedPoly(const QVector<QPointF> &r);
//  void appendedPoint(const QPointF &pt);
//  void movedPoint(const QPointF &pt);
protected:
};

class QxrdROISelector : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIAddNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot);

  QPolygon adjustedPoints(const QPolygon &poly) const;

  void begin();
  void move(const QPoint &pt);
  void append(const QPoint &pt);
  bool end(bool ok = true);

private:
  QPolygon m_SelectedPoints;
  int m_SelectedROI;
  int m_SelectedShape;
  int m_SelectedPoint;
  int m_SelectedEdge;
};

class QxrdROIRemoveNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRemoveNode(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIRotator : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRotator(QWidget *canvas, QxrdImagePlot *plot);
};

class QxrdROIResizer : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot);
};

#endif // QXRDROIPICKER_H
