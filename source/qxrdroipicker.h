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
                bool canSelectEdges,
                bool usePolygonsOnly,
                int triggeredAppend,
                double startingDistance,
                double secondaryDistance);

  virtual void printMessage(QString msg, QDateTime dt=QDateTime::currentDateTime()) const;
  virtual void drawRubberBand(QPainter *painter) const;
  virtual QRegion rubberBandMask() const;

protected:
  void begin();
  void move(const QPoint &pt);
  void append(const QPoint &pt);
  bool end(bool ok = true);

protected:
  enum {
    UseAllROIs       = 0,
    UseSelectedROIs  = 1
  };

  enum {
    SelectPointsOnly = 0,
    CanSelectEdges   = 1
  };

  enum {
    UseAnyShape      = 0,
    UsePolygonsOnly  = 1
  };

  enum {
    FirstAppend      = 0,
    SecondAppend     = 1
  };

  int m_UseSelectedROIsOnly;
  int m_CanSelectEdges;
  int m_UsePolygonsOnly;
  int m_TriggeredAppend;
  double m_StartingDistance;
  double m_SecondaryDistance;

  int m_SelectedROI;
  int m_SelectedShape;
  int m_SelectedPoint;
  int m_SelectedEdge;

  QPolygon m_SelectedPoints;
  QPolygon m_RubberBand;
};

class QxrdROISelector : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROISelector(QWidget *canvas, QxrdImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIAddNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIAddNode(QWidget *canvas, QxrdImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIRemoveNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRemoveNode(QWidget *canvas, QxrdImagePlot *plot);

  bool end(bool ok = true);
};

class QxrdROIRotator : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRotator(QWidget *canvas, QxrdImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIResizer : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIResizer(QWidget *canvas, QxrdImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);

private:
  double m_ScaledX;
  double m_ScaledY;
};

#endif // QXRDROIPICKER_H
