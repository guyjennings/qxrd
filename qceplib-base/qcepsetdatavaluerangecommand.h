#ifndef QCEPSETDATAVALUERANGECOMMAND_H
#define QCEPSETDATAVALUERANGECOMMAND_H

#include "qceplib_global.h"
#include "qcepserializableobject.h"
#include "qcepimagedata-ptr.h"

class QCEP_EXPORT QcepSetDataValueRangeCommand : public QcepSerializableObject
{
  Q_OBJECT

public:
  QcepSetDataValueRangeCommand();

public slots:
  void onImageAvailable(QcepDoubleImageDataPtr img);
  QcepDoubleImageDataPtr exec(QcepDoubleImageDataPtr img);

signals:
  void processedImageAvailable(QcepDoubleImageDataPtr res);
  void progressMade(int pct);

public:
  Q_PROPERTY(int regionType READ get_RegionType WRITE set_RegionType)
  QCEP_INTEGER_PROPERTY(RegionType)

  Q_PROPERTY(int regionCoordinateStyle READ get_RegionCoordinateStyle WRITE set_RegionCoordinateStyle)
  QCEP_INTEGER_PROPERTY(RegionCoordinateStyle)

  Q_PROPERTY(double regionX1 READ get_RegionX1 WRITE set_RegionX1)
  QCEP_DOUBLE_PROPERTY(RegionX1)

  Q_PROPERTY(double regionY1 READ get_RegionY1 WRITE set_RegionY1)
  QCEP_DOUBLE_PROPERTY(RegionY1)

  Q_PROPERTY(double regionX2 READ get_RegionX2 WRITE set_RegionX2)
  QCEP_DOUBLE_PROPERTY(RegionX2)

  Q_PROPERTY(double regionY2 READ get_RegionY2 WRITE set_RegionY2)
  QCEP_DOUBLE_PROPERTY(RegionY2)

  Q_PROPERTY(int outputChoice READ get_OutputChoice WRITE set_OutputChoice)
  QCEP_INTEGER_PROPERTY(OutputChoice)

  Q_PROPERTY(double newValue READ get_NewValue WRITE set_NewValue)
  QCEP_DOUBLE_PROPERTY(NewValue)
};

#endif // QCEPSETDATAVALUERANGECOMMAND_H
