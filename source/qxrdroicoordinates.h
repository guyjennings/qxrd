#ifndef QXRDROICOORDINATES_H
#define QXRDROICOORDINATES_H

#include "qcepmacros.h"
#include "qcepobject.h"
#include "qcepsettingssaver-ptr.h"
#include "qxrdexperiment-ptr.h"
#include "qcepproperty.h"
#include "qxrdroicoordinates-ptr.h"
#include <QScriptEngine>

class QxrdROICoordinates : public QcepObject
{
  Q_OBJECT

public:
  QxrdROICoordinates(QcepSettingsSaverWPtr saver,
                     QxrdExperimentWPtr    exp,
                     int                   roiType,
                     double                left=0,
                     double                top=0,
                     double                right=0,
                     double                bottom=0);
  virtual ~QxrdROICoordinates();

  static int     roiTypeCount();
  static QString roiTypeName(int roiType);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesPtr &coords);
  static void fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesPtr &coords);

  enum {
    SumInRectangle,
    AverageInRectangle,
    SumInEllipse,
    AverageInEllipse,
    SumInPeak
  };

signals:

public slots:
  double left() const;
  double top() const;
  double right() const;
  double bottom() const;
  double width() const;
  double height() const;

  QPointF center() const;
  QSizeF size() const;

  void setLeft(double l);
  void setTop(double t);
  void setRight(double r);
  void setBottom(double b);

  void setCenter(QPointF c);
  void setSize(QSizeF s);

  void setCenterX(double cx);
  void setCenterY(double cy);
  void setWidth(double w);
  void setHeight(double h);

public:
  Q_PROPERTY(int roiType READ get_RoiType WRITE set_RoiType)
  QCEP_INTEGER_PROPERTY(RoiType)

  Q_PROPERTY(QString roiTypeName READ get_RoiTypeName WRITE set_RoiTypeName STORED false)
  QCEP_STRING_PROPERTY(RoiTypeName)

  Q_PROPERTY(QRectF coords READ get_Coords WRITE set_Coords)
  QCEP_DOUBLE_RECT_PROPERTY(Coords)
};

#endif // QXRDROICOORDINATES_H
