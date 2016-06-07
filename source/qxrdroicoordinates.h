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
  QxrdROICoordinates(int                   roiOuterType,
                     int                   roiInnerType,
                     double                left=0,
                     double                top=0,
                     double                right=0,
                     double                bottom=0);
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

  enum {
    SumOutput,
    AverageOutput,
    MinimumOutput,
    MaximumOutput,
    NPixelsOutput,
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

//  double left() const;
//  double top() const;
//  double right() const;
//  double bottom() const;
//  double width() const;
//  double height() const;
//  double width2() const;
//  double height2() const;
//  double left2() const;
//  double right2() const;
//  double top2() const;
//  double bottom2() const;

//  QPointF center() const;
//  QSizeF size() const;
//  QSizeF size2() const;

//  void setLeft(double l);
//  void setTop(double t);
//  void setRight(double r);
//  void setBottom(double b);

  void setCenter(QPointF c);
//  void setCenter(double cx, double cy);
//  void setSize(QSizeF s);
//  void setSize(double w, double h);
//  void setSize2(QSizeF s);
//  void setSize2(double w, double h);

  void setCenterX(double cx);
  void setCenterY(double cy);
  void setWidth(double w);
  void setHeight(double h);
//  void setWidth2(double w);
//  void setHeight2(double h);

  QVector<QPointF> markerCoords();

private:
  void recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis);

//  void recalculateRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
//  void recalculateEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
//  void recalculateRectangleInRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
//  void recalculateEllipseInRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
//  void recalculateRectangleInEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask);
//  void recalculateEllipseInEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask);

public:
  Q_PROPERTY(int roiOuterType READ get_RoiOuterType WRITE set_RoiOuterType)
  QCEP_INTEGER_PROPERTY(RoiOuterType)

  Q_PROPERTY(int roiInnerType READ get_RoiInnerType WRITE set_RoiInnerType)
  QCEP_INTEGER_PROPERTY(RoiInnerType)

  Q_PROPERTY(QString roiOuterTypeName READ get_RoiOuterTypeName WRITE set_RoiOuterTypeName STORED false)
  QCEP_STRING_PROPERTY(RoiOuterTypeName)

  Q_PROPERTY(QString roiInnerTypeName READ get_RoiInnerTypeName WRITE set_RoiInnerTypeName STORED false)
  QCEP_STRING_PROPERTY(RoiInnerTypeName)

  Q_PROPERTY(QRectF coords READ get_Coords WRITE set_Coords STORED false)
  QCEP_DOUBLE_RECT_PROPERTY(Coords)

//  Q_PROPERTY(double width2 READ get_Width2 WRITE set_Width2)
//  QCEP_DOUBLE_PROPERTY(Width2)

//  Q_PROPERTY(double height2 READ get_Height2 WRITE set_Height2)
//  QCEP_DOUBLE_PROPERTY(Height2)

//  Q_PROPERTY(double value READ get_Value WRITE set_Value STORED false)
//  QCEP_DOUBLE_PROPERTY(Value)

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
};

#endif // QXRDROICOORDINATES_H
