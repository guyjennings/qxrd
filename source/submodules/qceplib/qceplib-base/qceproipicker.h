#ifndef QCEPROIPICKER_H
#define QCEPROIPICKER_H

#include "qceplib_global.h"
#include "qcepimageplotmeasurer.h"
#include <QDateTime>
#include "qceproi-ptr.h"

class QCEP_EXPORT QcepROIPicker : public QcepImagePlotMeasurer
{
  Q_OBJECT

public:
  QcepROIPicker(QcepImagePlot *plot,
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

class QCEP_EXPORT QcepROICreator : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROICreator(QcepImagePlot *plot);

  void begin();
  void move(const QPoint &pt);
  void append(const QPoint &pt);
  bool end(bool ok = true);

  void setCreatedType(int newType);

private:
  int        m_CreatedType;
  int        m_CreatedTypeID;
  QcepROIPtr m_NewROI;
};

class QCEP_EXPORT QcepROISelector : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROISelector(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QCEP_EXPORT QcepROIAddNode : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROIAddNode(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QCEP_EXPORT QcepROIRemoveNode : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROIRemoveNode(QcepImagePlot *plot);

  bool end(bool ok = true);
};

class QCEP_EXPORT QcepROIRotator : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROIRotator(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);
};

class QCEP_EXPORT QcepROIResizer : public QcepROIPicker
{
  Q_OBJECT

public:
  QcepROIResizer(QcepImagePlot *plot);

  void move(const QPoint &pt);
  bool end(bool ok = true);

private:
  double m_ScaledX;
  double m_ScaledY;
};

#endif // QCEPROIPICKER_H
