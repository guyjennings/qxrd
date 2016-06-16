#ifndef QXRDROICOORDINATES_H
#define QXRDROICOORDINATES_H

#include "qcepmacros.h"
#include "qcepserializableobject.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepproperty.h"
#include "qxrdroicoordinates-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qxrdroishape-ptr.h"

#include <QScriptEngine>
#include <QMutex>

class QxrdROICoordinates : public QcepSerializableObject
{
  Q_OBJECT

public:
  QxrdROICoordinates(int roiOuterType, int roiInnerType);
  virtual ~QxrdROICoordinates();

  static int     roiTypeID(int outerType, int innerType);
  static QString roiTypeName(int outerType, int innerType);

  static int     roiOuterType(int roiTypeID);
  static int     roiInnerType(int roiTypeID);

  static int     outputCount();
  static QString outputName(int opt);

  static QxrdROICoordinatesPtr newROICoordinates(int roiTypeID);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesPtr &coords);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesPtr &coords);

  void readSettings(QSettings *settings, QString section);
  void writeSettings(QSettings *settings, QString section);

  bool pointInInner(QPointF pt);
  bool pointInOuter(QPointF pt);

  enum {
    SumOutput,
    AverageOutput,
    MinimumOutput,
    MaximumOutput,
    NPixelsOutput,
    NBackgroundOutput,
    BackgroundOutput,
    XGradientOutput,
    YGradientOutput,
    OutputCount
  };

  enum {
    NoBounds,
    RectangleBounds,
    EllipseBounds
  };

  enum {
    VisualizeNone,
    VisualizeBackground,
    VisualizePeak
  };

  enum {
    InnerShape,
    OuterShape
  };

signals:
  void roiChanged();

public slots:
  void recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
  void visualizeBackground(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
  void visualizePeak(QcepImageDataBasePtr img, QcepMaskDataPtr mask);

  QVector<double> values() const;

  void selectNamedROIOuterType(QString nm);
  void selectNamedROIInnerType(QString nm);

  QxrdROIShapePtr inner() const;
  QxrdROIShapePtr outer() const;

  void outerChanged();
  void innerChanged();

  void setCenter(QPointF c);
  void setCenterX(double cx);
  void setCenterY(double cy);
  void setRotation(double r);

  void scaleROI(double kx, double ky);
  void deleteROIPoint(int innerOuter, int n);
  void changeROIPoint(int innerOuter, int n, QPointF pt);
  void insertROIPoint(int innerOuter, int n, QPointF pt);

  QVector<QPointF> markerCoords();
  QPolygonF dragOutline();
  QPolygonF rotatedDragOutline(double r);
  QPolygonF scaledDragOutline(double kx, double ky);

  QPolygonF innerOutline();
  QPolygonF outerOutline();
  QPolygonF scaledInnerOutline(double kx, double ky);
  QPolygonF scaledOuterOutline(double kx, double ky);

  QPointF transform( const QPointF & pt) const;
  QPointF invTransform( const QPointF & pt) const;

private:
  void updateBounds();
  void recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis);

public:
  Q_PROPERTY(int roiOuterType READ get_RoiOuterType WRITE set_RoiOuterType)
  QCEP_INTEGER_PROPERTY(RoiOuterType)

  Q_PROPERTY(int roiInnerType READ get_RoiInnerType WRITE set_RoiInnerType)
  QCEP_INTEGER_PROPERTY(RoiInnerType)

  Q_PROPERTY(QString roiOuterTypeName READ get_RoiOuterTypeName WRITE set_RoiOuterTypeName STORED false)
  QCEP_STRING_PROPERTY(RoiOuterTypeName)

  Q_PROPERTY(QString roiInnerTypeName READ get_RoiInnerTypeName WRITE set_RoiInnerTypeName STORED false)
  QCEP_STRING_PROPERTY(RoiInnerTypeName)

  Q_PROPERTY(QPointF center READ get_Center WRITE set_Center)
  QCEP_DOUBLE_POINT_PROPERTY(Center)

  Q_PROPERTY(double  rotation READ get_Rotation WRITE set_Rotation)
  QCEP_DOUBLE_PROPERTY(Rotation)

  Q_PROPERTY(double sum READ get_Sum WRITE set_Sum STORED false)
  QCEP_DOUBLE_PROPERTY(Sum)

  Q_PROPERTY(double average READ get_Average WRITE set_Average STORED false)
  QCEP_DOUBLE_PROPERTY(Average)

  Q_PROPERTY(double minimum READ get_Minimum WRITE set_Minimum STORED false)
  QCEP_DOUBLE_PROPERTY(Minimum)

  Q_PROPERTY(double maximum READ get_Maximum WRITE set_Maximum STORED false)
  QCEP_DOUBLE_PROPERTY(Maximum)

  Q_PROPERTY(double nPixels READ get_NPixels WRITE set_NPixels STORED false)
  QCEP_DOUBLE_PROPERTY(NPixels)

  Q_PROPERTY(double nBackground READ get_NBackground WRITE set_NBackground STORED false)
  QCEP_DOUBLE_PROPERTY(NBackground)

  Q_PROPERTY(double background READ get_Background WRITE set_Background STORED false)
  QCEP_DOUBLE_PROPERTY(Background)

  Q_PROPERTY(double xGradient READ get_XGradient WRITE set_XGradient STORED false)
  QCEP_DOUBLE_PROPERTY(XGradient)

  Q_PROPERTY(double yGradient READ get_YGradient WRITE set_YGradient STORED false)
  QCEP_DOUBLE_PROPERTY(YGradient)

private:
  QMutex          m_Mutex;
  QxrdROIShapePtr m_OuterShape;
  QxrdROIShapePtr m_InnerShape;

  QImage          m_OuterImage;
  QImage          m_InnerImage;
};

#endif // QXRDROICOORDINATES_H
