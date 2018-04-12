#ifndef QXRDROIPICKER_H
#define QXRDROIPICKER_H

#include "qxrdlib_global.h"
#include "qxrdimageplotmeasurer.h"
#include <QDateTime>
#include "qxrdroi-ptr.h"

class QXRD_EXPORT QxrdROIPicker : public QxrdImagePlotMeasurer
{
  Q_OBJECT

public:
  QxrdROIPicker(QcepImagePlot *plot,
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

class QxrdROICreator : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROICreator(QcepImagePlot *plot);

  void begin();
  void move(const QPoint &pt);
  void append(const QPoint &pt);
  bool end(bool ok = true);

  void setCreatedType(int newType);

private:
  int                   m_CreatedType;
  int                   m_CreatedTypeID;
  QxrdROIPtr m_NewROI;
};

class QxrdROISelector : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROISelector(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIAddNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIAddNode(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIRemoveNode : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRemoveNode(QcepImagePlot *plot);

  bool end(bool ok = true);
};

class QxrdROIRotator : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIRotator(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QxrdROIResizer : public QxrdROIPicker
{
  Q_OBJECT

public:
  QxrdROIResizer(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);

private:
  double m_ScaledX;
  double m_ScaledY;
};

#endif // QXRDROIPICKER_H
