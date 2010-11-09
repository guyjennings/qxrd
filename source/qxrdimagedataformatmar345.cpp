#include "qxrdimagedataformatmar345.h"

static QxrdImageDataFormatMar345 fmt;
static QcepImageDataFormatMar345<quint16> rawfmt("raw");
static QcepImageDataFormatMar345<quint32> raw2fmt("raw2");
static QcepImageDataFormatMar345<short> maskfmt("mask");

QxrdImageDataFormatMar345::QxrdImageDataFormatMar345(QString name)
  : QcepImageDataFormatMar345<double>(name)
{
}
