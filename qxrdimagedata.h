#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include "qcepimagedata.h"

class QxrdImageData : public QcepImageData<double>
{
  public:
    QxrdImageData(int width=0, int height=0);
};

#endif
