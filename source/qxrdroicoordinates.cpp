#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"

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
    m_Coords(saver, this, "coords", QRectF(left, top, right-left, bottom-top), "ROI Coords")
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
  } else if (roiType == SumInEllipse) {
    return "Ellipse Sum";
  } else if (roiType == AverageInEllipse) {
    return "Ellipse Avg";
  } else if (roiType == SumInPeak) {
    return "Peak Sum";
  }

  return "";
}

int QxrdROICoordinates::roiTypeCount()
{
  return 5;
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
}

void QxrdROICoordinates::setTop(double t)
{
  QRectF c = get_Coords();

  c.setTop(t);

  set_Coords(c);
}

void QxrdROICoordinates::setRight(double r)
{
  QRectF c = get_Coords();

  c.setRight(r);

  set_Coords(c);
}

void QxrdROICoordinates::setBottom(double b)
{
  QRectF c = get_Coords();

  c.setBottom(b);

  set_Coords(c);
}

void QxrdROICoordinates::setCenter(QPointF c)
{
  QRectF crd = get_Coords();

  crd.moveCenter(c);

  set_Coords(crd);
}

void QxrdROICoordinates::setSize(QSizeF s)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setSize(s);
  c.moveCenter(cen);

  set_Coords(c);
}

void QxrdROICoordinates::setCenterX(double cx)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setX(cx);
  c.moveCenter(cen);

  set_Coords(c);
}

void QxrdROICoordinates::setCenterY(double cy)
{
  QRectF  c = get_Coords();
  QPointF cen = c.center();

  cen.setY(cy);
  c.moveCenter(cen);

  set_Coords(c);
}

void QxrdROICoordinates::setWidth(double w)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setWidth(w);
  c.moveCenter(cen);

  set_Coords(c);
}

void QxrdROICoordinates::setHeight(double h)
{
  QRectF c = get_Coords();
  QPointF cen = c.center();

  c.setHeight(h);
  c.moveCenter(cen);

  set_Coords(c);
}
