#ifndef QXRDIMAGEDATAFORMATTIFF_H
#define QXRDIMAGEDATAFORMATTIFF_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepimagedataformattiff.h"

class QXRD_EXPORT QxrdImageDataFormatTiff : public QcepImageDataFormatTiff<double>
{
public:
  QxrdImageDataFormatTiff(QString name="tiff");
};

#endif
