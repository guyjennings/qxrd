#ifndef QXRDIMAGEDATAFORMATTIFF_H
#define QXRDIMAGEDATAFORMATTIFF_H

#include "qcepmacros.h"

#include "qcepimagedataformattiff.h"

class QxrdImageDataFormatTiff : public QcepImageDataFormatTiff<double>
{
public:
  QxrdImageDataFormatTiff(QString name="tiff");
};

#endif