#include "qxrdimagedataformattiff.h"

static QxrdImageDataFormatTiff fmt;
static QcepImageDataFormatTiff<quint16> rawfmt("raw");
static QcepImageDataFormatTiff<quint32> raw2fmt("raw2");
static QcepImageDataFormatTiff<short> maskfmt("mask");

QxrdImageDataFormatTiff::QxrdImageDataFormatTiff(QString name)
  : QcepImageDataFormatTiff<double>(name)
{
}
