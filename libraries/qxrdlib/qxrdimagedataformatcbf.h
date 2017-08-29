#ifndef QXRDIMAGEDATAFORMATCBF_H
#define QXRDIMAGEDATAFORMATCBF_H

#include "qcepmacros.h"

#include "qcepimagedataformatcbf.h"

class QxrdImageDataFormatCBF : public QcepImageDataFormatCBF<double>
{
public:
  QxrdImageDataFormatCBF(QString name="cbf");
};

#endif // QXRDIMAGEDATAFORMATCBF_H
