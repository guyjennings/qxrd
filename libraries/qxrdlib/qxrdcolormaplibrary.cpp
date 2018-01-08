#include "qxrdcolormaplibrary.h"
#include "qxrdcolormap.h"
#include "qxrdmaskcolormap.h"

static QxrdColorMap* newGrayscaleImageColorMap();
static QxrdColorMap* newInverseGrayscaleImageColorMap();
static QxrdColorMap* newEarthTonesImageColorMap();
static QxrdColorMap* newSpectrumImageColorMap();
static QxrdColorMap* newFireImageColorMap();
static QxrdColorMap* newIceImageColorMap();
static QxrdColorMap* newDefaultImageColorMap();

static QxrdMaskColorMap* newGrayscaleOverflowColorMap();
static QxrdMaskColorMap* newInverseGrayscaleOverflowColorMap();
static QxrdMaskColorMap* newEarthTonesOverflowColorMap();
static QxrdMaskColorMap* newSpectrumOverflowColorMap();
static QxrdMaskColorMap* newFireImageOverflowMap();
static QxrdMaskColorMap* newIceImageOverflowMap();
static QxrdMaskColorMap* newDefaultImageOverflowMap();

static QxrdMaskColorMap* newGrayscaleMaskColorMap();
static QxrdMaskColorMap* newInverseGrayscaleMaskColorMap();
static QxrdMaskColorMap* newEarthTonesMaskColorMap();
static QxrdMaskColorMap* newSpectrumMaskColorMap();
static QxrdMaskColorMap* newFireMaskColorMap();
static QxrdMaskColorMap* newIceMaskColorMap();
static QxrdMaskColorMap* newDefaultMaskColorMap();

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

QxrdColorMap* QxrdColorMapLibrary::newImageColorMap(int n)
{
  QxrdColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleImageColorMap();
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleImageColorMap();
    break;

  case EarthTonesMap:
    res = newEarthTonesImageColorMap();
    break;

  case SpectrumMap:
    res = newSpectrumImageColorMap();
    break;

  case FireMap:
    res = newFireImageColorMap();
    break;

  case IceMap:
    res = newIceImageColorMap();
    break;

  default:
    res = newDefaultImageColorMap();
    break;
  }

  return res;
}

QxrdMaskColorMap* QxrdColorMapLibrary::newOverflowColorMap(int n)
{
  QxrdMaskColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleOverflowColorMap();
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleOverflowColorMap();
    break;

  case EarthTonesMap:
    res = newEarthTonesOverflowColorMap();
    break;

  case SpectrumMap:
    res = newSpectrumOverflowColorMap();
    break;

  case FireMap:
    res = newFireImageOverflowMap();
    break;

  case IceMap:
    res = newIceImageOverflowMap();
    break;

  default:
    res = newDefaultImageOverflowMap();
    break;
  }

  return res;
}

QxrdMaskColorMap* QxrdColorMapLibrary::newMaskColorMap(int n)
{
  QxrdMaskColorMap* res = NULL;

  switch (n) {
  case GrayscaleMap:
    res = newGrayscaleMaskColorMap();
    break;

  case InverseGrayscaleMap:
    res = newInverseGrayscaleMaskColorMap();
    break;

  case EarthTonesMap:
    res = newEarthTonesMaskColorMap();
    break;

  case SpectrumMap:
    res = newSpectrumMaskColorMap();
    break;

  case FireMap:
    res = newFireMaskColorMap();
    break;

  case IceMap:
    res = newIceMaskColorMap();
    break;

  default:
    res = newDefaultMaskColorMap();
    break;
  }

  return res;
}

static QxrdColorMap* newGrayscaleImageColorMap()
{
  QxrdColorMap* res = new QxrdColorMap();

  res -> setColorInterval(Qt::black, Qt::white);
}

static QxrdColorMap* newInverseGrayscaleImageColorMap()
{
}

static QxrdColorMap* newEarthTonesImageColorMap()
{
}

static QxrdColorMap* newSpectrumImageColorMap()
{
}

static QxrdColorMap* newFireImageColorMap()
{
}

static QxrdColorMap* newIceImageColorMap()
{
}

static QxrdColorMap* newDefaultImageColorMap()
{
}

static QxrdMaskColorMap* newGrayscaleOverflowColorMap()
{
}

static QxrdMaskColorMap* newInverseGrayscaleOverflowColorMap()
{
}

static QxrdMaskColorMap* newEarthTonesOverflowColorMap()
{
}

static QxrdMaskColorMap* newSpectrumOverflowColorMap()
{
}

static QxrdMaskColorMap* newFireImageOverflowMap()
{
}

static QxrdMaskColorMap* newIceImageOverflowMap()
{
}

static QxrdMaskColorMap* newDefaultImageOverflowMap()
{
}

static QxrdMaskColorMap* newGrayscaleMaskColorMap()
{
}

static QxrdMaskColorMap* newInverseGrayscaleMaskColorMap()
{
}

static QxrdMaskColorMap* newEarthTonesMaskColorMap()
{
}

static QxrdMaskColorMap* newSpectrumMaskColorMap()
{
}

static QxrdMaskColorMap* newFireMaskColorMap()
{
}

static QxrdMaskColorMap* newIceMaskColorMap()
{
}

static QxrdMaskColorMap* newDefaultMaskColorMap()
{
}

