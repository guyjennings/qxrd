#include "qxrdimagedataformatcbf.h"

static QxrdImageDataFormatCBF cbf;

QxrdImageDataFormatCBF::QxrdImageDataFormatCBF(QString name)
  : QcepImageDataFormatCBF<double>(name)
{
}
