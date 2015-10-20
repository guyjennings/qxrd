#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"
#include <QtMath>

QxrdROICoordinates::QxrdROICoordinates(QcepSettingsSaverWPtr saver,
                                       QxrdExperimentWPtr    exp,
                                       int                   roiType,
                                       double                left,
                                       double                top,
                                       double                right,
                                       double                bottom)
  : QcepObject("coords", exp.data()),
    m_RoiType(saver, this, "roiType", roiType, "ROI Type"),
    m_RoiTypeName(QcepSettingsSaverWPtr(), this, "roiTypeName", roiTypeName(roiType), "ROI Type Name"),
    m_Coords(saver, this, "coords", QRectF(left, top, right-left, bottom-top), "ROI Coords"),
    m_Value(QcepSettingsSaverWPtr(), this, "value", 0, "ROI Value")
{
}

QxrdROICoordinates::~QxrdROICoordinates()
{
}

QScriptValue QxrdROICoordinates::toScriptValue(QScriptEngine *engine, const QxrdROICoordinatesPtr &coords)
{
  return engine->newQObject(coords.data());
}

void QxrdROICoordinates::fromScriptValue(const QScriptValue &obj, QxrdROICoordinatesPtr &coords)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QxrdROICoordinates *qcoords = qobject_cast<QxrdROICoordinates*>(qobj);

    if (qcoords) {
      coords = QxrdROICoordinatesPtr(qcoords);
    }
  }
}

QString QxrdROICoordinates::roiTypeName(int roiType)
{
  if (roiType == SumInRectangle) {
    return "Rectangle Sum";
  } else if (roiType == AverageInRectangle) {
    return "Rectangle Avg";
  } else if (roiType == MinInRectangle) {
    return "Rectangle Min";
  } else if (roiType == MaxInRectangle) {
    return "Rectangle Max";
  } else if (roiType == SumInEllipse) {
    return "Ellipse Sum";
  } else if (roiType == AverageInEllipse) {
    return "Ellipse Avg";
  } else if (roiType == MinInEllipse) {
    return "Ellipse Min";
  } else if (roiType == MaxInEllipse) {
    return "Ellipse Max";
  } else if (roiType == SumInPeak) {
    return "Peak Sum";
  }

  return "";
}

void QxrdROICoordinates::selectNamedROIType(QString nm)
{
  for (int i=0; i<roiTypeCount(); i++) {
    if (roiTypeName(i) == nm) {
      set_RoiType(i);
      set_RoiTypeName(nm);
      emit roiChanged();
      return;
    }
  }
}

int QxrdROICoordinates::roiTypeCount()
{
  return ROITypeCount;
}

double QxrdROICoordinates::left() const
{
  return get_Coords().left();
}

double QxrdROICoordinates::top() const
{
  return get_Coords().top();
}

double QxrdROICoordinates::right() const
{
  return get_Coords().right();
}

double QxrdROICoordinates::bottom() const
{
  return get_Coords().bottom();
}

double QxrdROICoordinates::width() const
{
  return get_Coords().width();
}

double QxrdROICoordinates::height() const
{
  return get_Coords().height();
}

QPointF QxrdROICoordinates::center() const
{
  return get_Coords().center();
}

QSizeF QxrdROICoordinates::size() const
{
  return get_Coords().size();
}

void QxrdROICoordinates::setLeft(double l)
{
  QRectF c = get_Coords();

  c.setLeft(l);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setTop(double t)
{
  QRectF c = get_Coords();

  c.setTop(t);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setRight(double r)
{
  QRectF c = get_Coords();

  c.setRight(r);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setBottom(double b)
{
  QRectF c = get_Coords();

  c.setBottom(b);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setCenter(QPointF c)
{
  QRectF crd = get_Coords();

  crd.moveCenter(c);

  set_Coords(crd);

  emit roiChanged();
}

void QxrdROICoordinates::setCenter(double cx, double cy)
{
  setCenter(QPointF(cx, cy));
}

void QxrdROICoordinates::setSize(QSizeF s)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setSize(s);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setSize(double w, double h)
{
  setSize(QSizeF(w,h));
}

void QxrdROICoordinates::setCenterX(double cx)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setX(cx);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setCenterY(double cy)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setY(cy);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setWidth(double w)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setWidth(w);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

void QxrdROICoordinates::setHeight(double h)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setHeight(h);
  c.moveCenter(cen);

  set_Coords(c);

  emit roiChanged();
}

QVector<QPointF> QxrdROICoordinates::markerCoords()
{
  QVector<QPointF> res;
  QRectF c = get_Coords();

  switch (get_RoiType()) {
  case SumInRectangle:
  case AverageInRectangle:
  case MinInRectangle:
  case MaxInRectangle:
  default:
    {
      res.append(c.topLeft());
      res.append(c.bottomLeft());
      res.append(c.bottomRight());
      res.append(c.topRight());
      res.append(c.topLeft());
    }
    break;

  case SumInEllipse:
  case AverageInEllipse:
  case MinInEllipse:
  case MaxInEllipse:
    {
      double a=c.width()/2.0;
      double b=c.height()/2.0;
      QPointF cen = c.center();

      for (int i=0; i<=16; i++) {
        double theta = M_PI*i/8.0;
        double x = cen.x() + a*cos(theta);
        double y = cen.y() + b*sin(theta);

        res.append(QPointF(x,y));
      }
    }
    break;

  case SumInPeak:
    {
      QRectF pkr;
      pkr.setSize(c.size()/2.0);
      pkr.moveCenter(c.center());

      res.append(c.topLeft());
      res.append(c.bottomLeft());
      res.append(c.bottomRight());
      res.append(c.topRight());
      res.append(c.topLeft());

      res.append(pkr.topLeft());

      res.append(pkr.bottomLeft());
      res.append(c.bottomLeft());
      res.append(pkr.bottomLeft());

      res.append(pkr.bottomRight());
      res.append(c.bottomRight());
      res.append(pkr.bottomRight());

      res.append(pkr.topRight());
      res.append(c.topRight());
      res.append(pkr.topRight());

      res.append(pkr.topLeft());
    }
    break;
  }

  return res;
}
