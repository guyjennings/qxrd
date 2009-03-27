#ifndef QIMGIMAGEDATAFORMATHIS_H
#define QIMGIMAGEDATAFORMATHIS_H

#include "qcepimagedata.h"
#include "qcepimagedataformat.h"

class QxrdImageDataFormatHis : public QcepImageDataFormat<double>
{
 public:
  QxrdImageDataFormatHis(QObject *parent=0);

  QxrdImageDataFormatHis* canLoadFile(QString path);
  QxrdImageDataFormatHis* loadFile(QString path, QcepImageData<double> *img);
  QxrdImageDataFormatHis* saveFile(QString path, QcepImageData<double> *img);
  Priority priority() const;
};

#endif
