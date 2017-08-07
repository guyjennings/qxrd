#ifndef QCEPSETRANGEOFIMAGECOMMAND_H
#define QCEPSETRANGEOFIMAGECOMMAND_H

#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QcepSetRangeOfImageCommand : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepSetRangeOfImageCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
  void progressMade(int pct);

public:
  Q_PROPERTY(int rangeChoice READ get_RangeChoice WRITE set_RangeChoice)
  QCEP_INTEGER_PROPERTY(RangeChoice)

  Q_PROPERTY(int outputChoice READ get_OutputChoice WRITE set_OutputChoice)
  QCEP_INTEGER_PROPERTY(OutputChoice)

  Q_PROPERTY(double minimum READ get_Minimum WRITE set_Minimum)
  QCEP_DOUBLE_PROPERTY(Minimum)

  Q_PROPERTY(double maximum READ get_Maximum WRITE set_Maximum)
  QCEP_DOUBLE_PROPERTY(Maximum)

  Q_PROPERTY(double newValue READ get_NewValue WRITE set_NewValue)
  QCEP_DOUBLE_PROPERTY(NewValue)
};

#endif // QCEPSETRANGEOFIMAGECOMMAND_H
