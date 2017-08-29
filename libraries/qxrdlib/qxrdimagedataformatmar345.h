#ifndef QXRDIMAGEDATAFORMATMAR345_H
#define QXRDIMAGEDATAFORMATMAR345_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"
#include "qcepimagedataformatmar345.h"

class QXRD_EXPORT QxrdImageDataFormatMar345 : public QcepImageDataFormatMar345<double>
{
public:
  QxrdImageDataFormatMar345(QString name="mar345");
};

#endif
