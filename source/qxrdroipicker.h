#ifndef QXRDROIPICKER_H
#define QXRDROIPICKER_H

#include "qxrdimageplotmeasurer.h"
#include <QDateTime>

class QxrdROIPicker : public QxrdImagePlotMeasurer
{
  Q_OBJECT

public:
  QxrdROIPicker(QWidget *canvas, QxrdImagePlot *plot,
                bool useSelectedOnly,
                bool canSelectEdges);

  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;

protected:
  void begin();
  void move(const QPoint &pt);
  void append(const QPoint &pt);
  bool end(bool ok = true);

protected:
  enum {
    UseAllROIs = 0,
    UseSelectedROIs = 1
  };

  enum {
    SelectPointsOnly = 0,
    CanSelectEdges   = 1
  };

  int m_UseSelectedROIsOnly;
  int m_CanSelectEdges;
  int m_SelectedROI;
  int m_SelectedShape;
  int m_SelectedPoint;
  int m_SelectedEdge;

  QPolygon m_SelectedPoints;
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
