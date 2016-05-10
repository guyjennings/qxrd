#ifndef QTESTIMAGEDATA_H
#define QTESTIMAGEDATA_H

#include "qcepimagedata.h"

class QTestImageData : public QcepDoubleImageData
{
  Q_OBJECT
public:
  explicit QTestImageData(QcepObjectWPtr parent, QString name, int width, int height, double def=0);

  bool writeImage(QString fileName);

  Q_PROPERTY(QString inherited READ get_Inherited WRITE set_Inherited)
  QCEP_STRING_PROPERTY(Inherited)
};

#endif // QTESTIMAGEDATA_H
