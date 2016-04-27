#include "qxrdroicoordinates.h"
#include "qxrdexperiment.h"
#include <QtMath>
#include <QMatrix4x4>

QxrdROICoordinates::QxrdROICoordinates(QxrdExperimentWPtr    exp,
                                       int                   roiType,
                                       double                left,
                                       double                top,
                                       double                right,
                                       double                bottom)
  : QcepObject("coords", exp),
    m_RoiType(this, "roiType", roiType, "ROI Type"),
    m_RoiTypeName(this, "roiTypeName", roiTypeName(roiType), "ROI Type Name"),
    m_Coords(this, "coords", QRectF(left, top, right-left, bottom-top), "ROI Coords"),
    m_Width2(this, "width2", 0, "Width of inner region"),
    m_Height2(this, "height2", 0, "Height of inner region"),
    m_Sum(this, "sum", 0, "ROI Pixel Sum"),
    m_Average(this, "average", 0, "ROI Pixel Average"),
    m_Minimum(this, "minimum", 0, "ROI Pixel Minimum"),
    m_Maximum(this, "maximum", 0, "ROI Pixel Maximum"),
    m_NPixels(this, "nPixels", 0, "ROI N Pixels"),
    m_Background(this, "background", 0, "ROI Background"),
    m_XGradient(this, "xGradient", 0, "ROI X Gradient"),
    m_YGradient(this, "yGradient", 0, "ROI Y Gradient")
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

double QxrdROICoordinates::left2() const
{
  return center().x() - width2()/2.0;
}

double QxrdROICoordinates::right2() const
{
  return center().x() + width2()/2.0;
}

double QxrdROICoordinates::top2() const
{
  return center().y() - height2()/2.0;
}

double QxrdROICoordinates::bottom2() const
{
  return center().y() + height2()/2.0;
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
      pkr.setSize(size2());
      pkr.moveCenter(c.center());

      res.append(c.topLeft());
      res.append(c.bottomLeft());
      res.append(c.bottomRight());
      res.append(c.topRight());
      res.append(c.topLeft());

      res.append(QPointF(qQNaN(), qQNaN()));

      res.append(pkr.topLeft());
      res.append(pkr.bottomLeft());
      res.append(pkr.bottomRight());
      res.append(pkr.topRight());
      res.append(pkr.topLeft());
    }
    break;

  case RectangleInEllipse:
    {
      QRectF pkr;
      pkr.setSize(size2());
      pkr.moveCenter(c.center());

      double a=c.width()/2.0;
      double b=c.height()/2.0;
      QPointF cen = c.center();

      for (int i=0; i<=16; i++) {
        double theta = M_PI*i/8.0;
        double x = cen.x() + a*cos(theta);
        double y = cen.y() + b*sin(theta);

        res.append(QPointF(x,y));
      }

      res.append(QPointF(qQNaN(), qQNaN()));

      res.append(pkr.topLeft());
      res.append(pkr.bottomLeft());
      res.append(pkr.bottomRight());
      res.append(pkr.topRight());
      res.append(pkr.topLeft());
    }
    break;

  case EllipseInRectangle:
    {
      res.append(c.topLeft());
      res.append(c.bottomLeft());
      res.append(c.bottomRight());
      res.append(c.topRight());
      res.append(c.topLeft());

      res.append(QPointF(qQNaN(), qQNaN()));

      double a2=width2()/2.0;
      double b2=height2()/2.0;
      QPointF cen = center();

      for (int i=0; i<=16; i++) {
        double theta = M_PI*i/8.0;
        double x = cen.x() + a2*cos(theta);
        double y = cen.y() + b2*sin(theta);

        res.append(QPointF(x,y));
      }
    }
    break;

  case EllipseInEllipse:
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

      res.append(QPointF(qQNaN(), qQNaN()));

      double a2=width2()/2.0;
      double b2=height2()/2.0;

      for (int i=0; i<=16; i++) {
        double theta = M_PI*i/8.0;
        double x = cen.x() + a2*cos(theta);
        double y = cen.y() + b2*sin(theta);

        res.append(QPointF(x,y));
      }
    }
    break;
  }

  return res;
}

void QxrdROICoordinates::recalculate(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeNone);
}

void QxrdROICoordinates::visualizeBackground(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizeBackground);
}

void QxrdROICoordinates::visualizePeak(QcepImageDataBasePtr img, QcepMaskDataPtr mask)
{
  recalculatePrivate(img, mask, VisualizePeak);
}

void QxrdROICoordinates::recalculatePrivate(QcepImageDataBasePtr img, QcepMaskDataPtr mask, int vis)
{
  int outerBounds = NoBounds;
  int innerBounds = NoBounds;

  switch (get_RoiType()) {
  case Rectangle:
    outerBounds = RectangleBounds;
    break;
  case Ellipse:
    outerBounds = EllipseBounds;
    break;
  case RectangleInRectangle:
    outerBounds = RectangleBounds;
    innerBounds = RectangleBounds;
    break;
  case RectangleInEllipse:
    outerBounds = EllipseBounds;
    innerBounds = RectangleBounds;
    break;
  case EllipseInRectangle:
    outerBounds = RectangleBounds;
    innerBounds = EllipseBounds;
    break;
  case EllipseInEllipse:
    outerBounds = EllipseBounds;
    innerBounds = EllipseBounds;
    break;
  }

  int first = true;
  double min = 0;
  double max = 0;
  double sum = 0;
  double npx = 0;

  double sumvn = 0;
  double sumvx = 0;
  double sumvy = 0;

  double sumct = 0;
  double sumnn = 0;
  double sumnx = 0;
  double sumny = 0;
  double sumxy = 0;
  double sumxx = 0;
  double sumyy = 0;

  double bkgd = 0;
  double gradx = 0;
  double grady = 0;

  if (img) {
    int tp = qRound(top());
    int bt = qRound(bottom());
    int lf = qRound(left());
    int rt = qRound(right());

    int tp2 = qRound(top2());
    int bt2 = qRound(bottom2());
    int lf2 = qRound(left2());
    int rt2 = qRound(right2());

    double cx = center().x();
    double cy = center().y();
    double a  = width()/2.0;
    double b  = height()/2.0;
    double a2 = width2()/2.0;
    double b2 = height2()/2.0;

    for (int row=tp; row<=bt; row++) {
      double dy = row - cy;

      if (outerBounds == EllipseBounds) {
        double xx = a*sqrt(1 - pow(dy/b,2));
        lf = qRound(cx - xx);
        rt = qRound(cx + xx);
      }

      for (int col=lf; col<=rt; col++) {
        double dx = col - cx;

        if (innerBounds == EllipseBounds) {
          double xx2 = a2*sqrt(1 - pow(dy/b2,2));
          if (xx2==xx2) {
            lf2 = qRound(cx - xx2);
            rt2 = qRound(cx + xx2);
          } else {
            lf2 = rt;
            rt2 = rt;
          }
        }

        if (innerBounds == NoBounds
            || (row <= tp2)
            || (row >= bt2)
            || (col <= lf2)
            || (col >= rt2)) {
          if (mask == NULL || mask->value(col, row)) {
            double val = img->getImageData(col, row);

            if (val == val) {
              sumct += 1;
              sumnn += 1;
              sumnx += dx;
              sumny += dy;
              sumxy += dx*dy;
              sumxx += dx*dx;
              sumyy += dy*dy;

              sumvn += val;
              sumvx += val*dx;
              sumvy += val*dy;

              if (innerBounds == NoBounds) {
                if (first) {
                  min = val;
                  max = val;
                  first = false;
                } else if (val > max) {
                  max = val;
                } else if (val < min) {
                  min = val;
                }
              }

              sum += val;
              npx += 1;

              if (vis == VisualizeBackground) {
                img->setImageData(col, row, 1000 - val);
              }
            }
          }
        }
      }
    }

    if (sumct > 5) {
      QMatrix4x4 m;

      m(0,0) = sumnn;
      m(1,0) = sumnx;
      m(0,1) = sumnx;
      m(2,0) = sumny;
      m(0,2) = sumny;
      m(1,1) = sumxx;
      m(2,1) = sumxy;
      m(1,2) = sumxy;
      m(2,2) = sumyy;

      bool invertible;

      QMatrix4x4 inv = m.inverted(&invertible);

      if (invertible) {
        bkgd   = inv(0,0)*sumvn + inv(0,1)*sumvx + inv(0,2)*sumvy;
        gradx  = inv(1,0)*sumvn + inv(1,1)*sumvx + inv(1,2)*sumvy;
        grady  = inv(2,0)*sumvn + inv(2,1)*sumvx + inv(2,2)*sumvy;
      } else {
        bkgd = sumvn/sumct;
      }
    } else if (sumct > 0) {
      bkgd = sumvn/sumct;
    }
    if (innerBounds != NoBounds) {
      first = true;
      min = 0;
      max = 0;
      sum = 0;
      npx = 0;

      for (int row=tp2; row<=bt2; row++) {
        double dy = row - cy;

        if (innerBounds == EllipseBounds) {
          double xx2 = a2*sqrt(1 - pow(dy/b2,2));
          if (xx2==xx2) {
            lf2 = qRound(cx - xx2);
            rt2 = qRound(cx + xx2);
          } else {
            lf2 = rt;
            rt2 = rt-1;
          }
        }

        for (int col=lf2; col<=rt2; col++) {
          if (mask == NULL || mask->value(col, row)) {
            double val = img->getImageData(col, row);

            if (val == val) {
              double dx = col - cx;
              double bk = bkgd + dx*gradx + dy*grady;
              double v  = val - bk;

              if (first) {
                min = v;
                max = v;
                first = false;
              } else if (v > max) {
                max = v;
              } else if (v < min) {
                min = v;
              }

              sum += v;
              npx += 1;

              if (vis == VisualizePeak) {
                img->setImageData(col, row, 1000 - val);
              }
            }
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

  set_Background(bkgd);
  set_XGradient(gradx);
  set_YGradient(grady);
}

QVector<double> QxrdROICoordinates::values() const
{
  QVector<double> res;

  res.append(get_Sum());
  res.append(get_Average());
  res.append(get_Minimum());
  res.append(get_Maximum());
  res.append(get_NPixels());
  res.append(get_Background());
  res.append(get_XGradient());
  res.append(get_YGradient());

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
  case BackgroundOutput:
    res = "Background";
    break;
  case XGradientOutput:
    res = "X Gradient";
    break;
  case YGradientOutput:
    res = "Y Gradient";
    break;
  }

  return res;
}
