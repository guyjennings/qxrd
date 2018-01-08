#include "qxrdcolormaplibrary.h"
#include "qxrdcolormap.h"
#include "qxrdmaskcolormap.h"
#include "qxrdimageplotwidgetsettings.h"
#include <QtMath>

QxrdColorMapLibrary::QxrdColorMapLibrary(QString name)
  : QcepObject(name)
{
}

enum {
  GrayscaleMap,
  InverseGrayscaleMap,
  EarthTonesMap,
  SpectrumMap,
  FireMap,
  IceMap,
  ColorMapCount
};

int QxrdColorMapLibrary::colorMapCount()
{
  return ColorMapCount;
}

QString QxrdColorMapLibrary::colorMapName(int n)
{
  QString res = "Unknown Color Map";

  switch (n) {
  case GrayscaleMap:
    res = "Grayscale";
    break;

  case InverseGrayscaleMap:
    res = "Inverse Grayscale";
    break;

  case EarthTonesMap:
    res = "Earth Tones";
    break;

  case SpectrumMap:
    res = "Spectrum";
    break;

  case FireMap:
    res = "Fire";
    break;

  case IceMap:
    res = "Ice";
    break;
  }

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newImageColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleImageColorMap(set);
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleImageColorMap(set);
    break;

  case EarthTonesMap:
    res = newEarthTonesImageColorMap(set);
    break;

  case SpectrumMap:
    res = newSpectrumImageColorMap(set);
    break;

  case FireMap:
    res = newFireImageColorMap(set);
    break;

  case IceMap:
    res = newIceImageColorMap(set);
    break;

  default:
    res = newDefaultImageColorMap(set);
    break;
  }

  return res;
}

QxrdMaskColorMap* QxrdColorMapLibrary::newOverflowColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdMaskColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleOverflowColorMap(set);
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleOverflowColorMap(set);
    break;

  case EarthTonesMap:
    res = newEarthTonesOverflowColorMap(set);
    break;

  case SpectrumMap:
    res = newSpectrumOverflowColorMap(set);
    break;

  case FireMap:
    res = newFireImageOverflowMap(set);
    break;

  case IceMap:
    res = newIceImageOverflowMap(set);
    break;

  default:
    res = newDefaultImageOverflowMap(set);
    break;
  }

  return res;
}

QxrdMaskColorMap* QxrdColorMapLibrary::newMaskColorMap(int n, QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdMaskColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleMaskColorMap(set);
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleMaskColorMap(set);
    break;

  case EarthTonesMap:
    res = newEarthTonesMaskColorMap(set);
    break;

  case SpectrumMap:
    res = newSpectrumMaskColorMap(set);
    break;

  case FireMap:
    res = newFireMaskColorMap(set);
    break;

  case IceMap:
    res = newIceMaskColorMap(set);
    break;

  default:
    res = newDefaultMaskColorMap(set);
    break;
  }

  return res;
}

void QxrdColorMapLibrary::colorMapStart(QxrdColorMap* map,
                                        QColor startColor,
                                        QColor endColor)
{
  if (map) {
    map->setColorInterval(startColor, endColor);
  }
}

void QxrdColorMapLibrary::colorMapRange(QxrdColorMap* map,
                                        double value1,
                                        QColor color1,
                                        double value2,
                                        QColor color2)
{
  if (map) {
    QxrdImagePlotWidgetSettingsPtr set(map->settings());

    if (map && set && set->get_DisplayLog()) {
      int n1 = int(value1*100);
      int n2 = int(value2*100);
      double r1 = color1.redF();
      double r2 = color2.redF();
      double g1 = color1.greenF();
      double g2 = color2.greenF();
      double b1 = color1.blueF();
      double b2 = color2.blueF();

      for (int n=n1; n<n2; n++) {
        double pos = double(n)/100.0;
        double val = (pow(10.0, pos) - 1.0)/9.0;
        double interp = (pos-value1)/(value2-value1);

        QColor col = QColor::fromRgbF(r1 + (r2-r1)*interp, g1 + (g2 - g1)*interp, b1 + (b2 - b1)*interp);

        map->addColorStop(val, col);
      }
    } else {
      map->addColorStop(value1, color1);
    }
  }
}

QxrdColorMap* QxrdColorMapLibrary::newGrayscaleImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::black, Qt::white);
  colorMapRange(res, 0.0, Qt::black, 1.0, Qt::white);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newInverseGrayscaleImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::white, Qt::black);
  colorMapRange(res, 0.0, Qt::white, 1.0, Qt::black);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newEarthTonesImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::black, Qt::white);

  colorMapRange(res, 0.0, Qt::black, 0.15, Qt::blue);
  colorMapRange(res, 0.15, Qt::blue, 0.25, Qt::gray);
  colorMapRange(res, 0.25, Qt::gray, 0.35, Qt::green);
  colorMapRange(res, 0.35, Qt::green, 0.5, Qt::darkYellow);
  colorMapRange(res, 0.5, Qt::darkYellow, 0.85, Qt::darkMagenta);
  colorMapRange(res, 0.85, Qt::darkMagenta, 1.0, Qt::white);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newSpectrumImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::magenta, Qt::red);

  colorMapRange(res, 0.0, Qt::magenta,0.2, Qt::blue);
  colorMapRange(res, 0.2, Qt::blue,   0.4, Qt::cyan);
  colorMapRange(res, 0.4, Qt::cyan,   0.6, Qt::green);
  colorMapRange(res, 0.6, Qt::green,  0.8, Qt::yellow);
  colorMapRange(res, 0.8, Qt::yellow, 1.0, Qt::red);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newFireImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::black, Qt::white);

  colorMapRange(res, 0.0,  Qt::black,  0.25, Qt::red);
  colorMapRange(res, 0.25, Qt::red,    0.75, Qt::yellow);
  colorMapRange(res, 0.75, Qt::yellow, 1.0,  Qt::white);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newIceImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  QxrdColorMap* res = new QxrdColorMap(set);

  colorMapStart(res, Qt::black, Qt::white);

  colorMapRange(res, 0.0,  Qt::black, 0.25, Qt::blue);
  colorMapRange(res, 0.25, Qt::blue,  0.75, Qt::cyan);
  colorMapRange(res, 0.75, Qt::cyan,  1.0,  Qt::white);

  return res;
}

QxrdColorMap* QxrdColorMapLibrary::newDefaultImageColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newGrayscaleImageColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newGrayscaleOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newInverseGrayscaleOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newEarthTonesOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newSpectrumOverflowColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newFireImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newIceImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultImageOverflowMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newDefaultImageOverflowMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return new QxrdMaskColorMap(set, QColor(0,0,0,0), Qt::green);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newGrayscaleMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newInverseGrayscaleMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newEarthTonesMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newSpectrumMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newFireMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newIceMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return newDefaultMaskColorMap(set);
}

QxrdMaskColorMap* QxrdColorMapLibrary::newDefaultMaskColorMap(QxrdImagePlotWidgetSettingsWPtr set)
{
  return new QxrdMaskColorMap(set, Qt::red, QColor(0,0,0,0));
}

