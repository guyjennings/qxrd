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
    m_Width2(saver, this, "width2", 0, "Width of inner region"),
    m_Height2(saver, this, "height2", 0, "Height of inner region"),
    m_Sum(QcepSettingsSaverWPtr(), this, "sum", 0, "ROI Pixel Sum"),
    m_Average(QcepSettingsSaverWPtr(), this, "average", 0, "ROI Pixel Average"),
    m_Minimum(QcepSettingsSaverWPtr(), this, "minimum", 0, "ROI Pixel Minimum"),
    m_Maximum(QcepSettingsSaverWPtr(), this, "maximum", 0, "ROI Pixel Maximum"),
    m_NPixels(QcepSettingsSaverWPtr(), this, "nPixels", 0, "ROI N Pixels")
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
  if (roiType == Rectangle) {
    return "Rectangle";
  } else if (roiType == Ellipse) {
    return "Ellipse";
  } else if (roiType == RectangleInRectangle) {
    return "Rectangle inside Rectangle";
  } else if (roiType == EllipseInRectangle) {
    return "Ellipse inside Rectangle";
  } else if (roiType == RectangleInEllipse) {
    return "Rectangle inside Ellipse";
  } else if (roiType == EllipseInEllipse) {
    return "Ellipse inside Ellipse";
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

double QxrdROICoordinates::width2() const
{
  return get_Width2();
}

double QxrdROICoordinates::height2() const
{
  return get_Height2();
}

QSizeF QxrdROICoordinates::size2() const
{
  return QSizeF(get_Width2(), get_Height2());
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

void QxrdROICoordinates::setSize2(QSizeF s)
{
  set_Width2(s.width());
  set_Height2(s.height());

  emit roiChanged();
}

void QxrdROICoordinates::setSize2(double w, double h)
{
  setSize2(QSizeF(w,h));
}

void QxrdROICoordinates::setWidth2(double w)
{
  set_Width2(w);

  emit roiChanged();
}

void QxrdROICoordinates::setHeight2(double w)
{
  set_Height2(w);

  emit roiChanged();
}

QVector<QPointF> QxrdROICoordinates::markerCoords()
{
  QVector<QPointF> res;
  QRectF c = get_Coords();

  switch (get_RoiType()) {
  case Rectangle:
  default:
    {
      res.append(c.topLeft());
      res.append(c.bottomLeft());
      res.append(c.bottomRight());
      res.append(c.topRight());
      res.append(c.topLeft());
    }
    break;

  case Ellipse:
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

  case RectangleInRectangle:
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

void QxrdROICoordinates::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  switch (get_RoiType()) {
  case Rectangle:
    recalculateRectangle(img, mask);
    break;
  case Ellipse:
    recalculateEllipse(img, mask);
    break;
  case RectangleInRectangle:
    recalculateRectangleInRectangle(img, mask);
    break;
  case RectangleInEllipse:
    recalculateRectangleInEllipse(img, mask);
    break;
  case EllipseInRectangle:
    recalculateEllipseInRectangle(img, mask);
    break;
  case EllipseInEllipse:
    recalculateEllipseInEllipse(img, mask);
    break;
  }
}

void QxrdROICoordinates::recalculateRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  int first = true;
  double min = 0;
  double max = 0;
  double sum = 0;
  double npx = 0;

  if (img) {
    int l = qRound(left());
    int t = qRound(top());
    int r = qRound(right());
    int b = qRound(bottom());


    for (int row=t; row<=b; row++) {
      for (int col=l; col<=r; col++) {
        if (mask == NULL || mask->value(col,row)) {
          double val = img->getImageData(col, row);

          if (val==val) {
            if (first) {
              min = val;
              max = val;
              first = false;
            } else if (val > max) {
              max = val;
            } else if (val < min) {
              min = val;
            }

            sum += val;
            npx += 1;
          }
        }
      }
    }
  }

  set_Sum(sum);
  set_NPixels(npx);
  set_Minimum(min);
  set_Maximum(max);

  if (npx > 0) {
    set_Average(sum/npx);
  } else {
    set_Average(0);
  }
}

void QxrdROICoordinates::recalculateEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
}

void QxrdROICoordinates::recalculateRectangleInRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
}

void QxrdROICoordinates::recalculateEllipseInRectangle(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
}

void QxrdROICoordinates::recalculateRectangleInEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
}

void QxrdROICoordinates::recalculateEllipseInEllipse(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
}

QVector<double> QxrdROICoordinates::values() const
{
  QVector<double> res;

  res.append(get_Sum());
  res.append(get_Average());
  res.append(get_Minimum());
  res.append(get_Maximum());
  res.append(get_NPixels());

  return res;
}

int QxrdROICoordinates::outputCount()
{
  return OutputCount;
}

QString QxrdROICoordinates::outputName(int opt)
{
  QString res;

  switch (opt) {
  case SumOutput:
    res = "Sum";
    break;
  case AverageOutput:
    res = "Average";
    break;
  case MinimumOutput:
    res = "Minimum";
    break;
  case MaximumOutput:
    res = "Maximum";
    break;
  case NPixelsOutput:
    res = "# Pixels";
    break;
  }

  return res;
}
