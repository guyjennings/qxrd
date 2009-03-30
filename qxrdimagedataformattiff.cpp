#include "qxrdimagedataformattiff.h"

static QxrdImageDataFormatTiff fmt;

QxrdImageDataFormatTiff::QxrdImageDataFormatTiff(QString name)
  : QcepImageDataFormatTiff<double>(name)
{
}
