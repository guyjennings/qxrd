#ifndef QXRDIMAGEDATAFORMATMAR345_H
#define QXRDIMAGEDATAFORMATMAR345_H

#include "qcepimagedataformatmar345.h"

class QxrdImageDataFormatMar345 : public QcepImageDataFormatMar345<double>
{
  public:
    QxrdImageDataFormatMar345(QString name="mar345");
};

#endif
