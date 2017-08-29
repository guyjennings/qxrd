#ifndef QXRDIMAGEDATAFORMATCBF_H
#define QXRDIMAGEDATAFORMATCBF_H

#include "qxrdlib_global.h"
#include "qcepmacros.h"

#include "qcepimagedataformatcbf.h"

class QXRD_EXPORT QxrdImageDataFormatCBF : public QcepImageDataFormatCBF<double>
{
public:
  QxrdImageDataFormatCBF(QString name="cbf");
};

#endif // QXRDIMAGEDATAFORMATCBF_H
