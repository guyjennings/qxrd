#ifndef QCEPROI_H
#define QCEPROI_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepproperty.h"
#include "qceproi-ptr.h"
#include "qcepimagedata-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qceproishape-ptr.h"
#include "qceproicache-ptr.h"

#include <QScriptEngine>
#include <QMutex>

class QCEP_EXPORT QcepROI : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  QcepROI(int roiOuterType, int roiInnerType);
  virtual ~QcepROI();
  void initialize(QcepObjectWPtr parent);

  static QcepROIPtr readROI(QcepObjectWPtr parent, QSettings *settings);

  static int     roiTypeID(int outerType, int innerType);
  static QString roiTypeName(int outerType, int innerType);

  static int     roiOuterType(int roiTypeID);
  static int     roiInnerType(int roiTypeID);

  static int     outputCount();
  static QString outputName(int opt);

  static QcepROIPtr newROICoordinates(int roiTypeID);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QcepROIPtr &coords);
  static void fromScriptValue(const QScriptValue &obj, QcepROIPtr &coords);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

  bool pointInInner(QPointF pt);
  bool pointInOuter(QPointF pt);

  double nearestDistance(QPointF pt);

  typedef enum {
    NewRectROI,
    NewEllipseROI,
    NewRectDonutROI,
    NewEllipseDonutROI,
    LastNewROIType
  } NewROIType;

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
    InnerSum,
    OuterSum,
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

  QcepROIShapePtr inner() const;
  QcepROIShapePtr outer() const;

  int innerType();
  int outerType();

  QString innerTypeName();
  QString outerTypeName();

  void changeInnerType(int t);
  void changeOuterType(int t);

  void changed();

  void setCenter(QPointF c);
  void setCenterX(double cx);
  void setCenterY(double cy);
  void setRotation(double r);

  void scaleROI(int innerOuter, double kx, double ky);
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
  bool innerPoint(QPointF pt) const;
  bool outerPoint(QPointF pt) const;

  void recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis);

public:
  Q_PROPERTY(int roiOuterType READ get_RoiOuterType WRITE set_RoiOuterType)
  QCEP_INTEGER_PROPERTY(RoiOuterType)

  Q_PROPERTY(int roiInnerType READ get_RoiInnerType WRITE set_RoiInnerType)
  QCEP_INTEGER_PROPERTY(RoiInnerType)

//  Q_PROPERTY(QString roiOuterTypeName READ get_RoiOuterTypeName WRITE set_RoiOuterTypeName STORED false)
//  QCEP_STRING_PROPERTY(RoiOuterTypeName)

//  Q_PROPERTY(QString roiInnerTypeName READ get_RoiInnerTypeName WRITE set_RoiInnerTypeName STORED false)
//  QCEP_STRING_PROPERTY(RoiInnerTypeName)

  Q_PROPERTY(QPointF center READ get_Center WRITE set_Center)
  QCEP_DOUBLE_POINT_PROPERTY(Center)

  Q_PROPERTY(double  rotation READ get_Rotation WRITE set_Rotation)
  QCEP_DOUBLE_PROPERTY(Rotation)

  Q_PROPERTY(int changed READ get_Changed WRITE set_Changed STORED false)
  QCEP_INTEGER_PROPERTY(Changed)

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

  Q_PROPERTY(double innerSum READ get_InnerSum WRITE set_InnerSum STORED false)
  QCEP_DOUBLE_PROPERTY(InnerSum)

  Q_PROPERTY(double outerSum READ get_OuterSum WRITE set_OuterSum STORED false)
  QCEP_DOUBLE_PROPERTY(OuterSum)

private:
  QcepROIShapePtr m_OuterShape;
  QcepROIShapePtr m_InnerShape;

  QcepROICachePtr m_Cache;
  QRect           m_Bounds;
  QRect           m_InnerBounds;
  QRect           m_OuterBounds;
};

#endif // QCEPROI_H
