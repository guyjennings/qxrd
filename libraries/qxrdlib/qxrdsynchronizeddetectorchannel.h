#ifndef QXRDSYNCHRONIZEDDETECTORCHANNEL_H
#define QXRDSYNCHRONIZEDDETECTORCHANNEL_H

#include "qxrdlib_global.h"
#include "qcepobject.h"

class QXRD_EXPORT QxrdSynchronizedDetectorChannel : public QcepObject
{
  Q_OBJECT

private:
  typedef QcepObject inherited;

public:
  Q_INVOKABLE QxrdSynchronizedDetectorChannel(QString name);

public:
  Q_PROPERTY(int channelNumber READ get_ChannelNumber WRITE set_ChannelNumber)
  QCEP_INTEGER_PROPERTY(ChannelNumber)

  Q_PROPERTY(int detectorNumber READ get_DetectorNumber WRITE set_DetectorNumber)
  QCEP_INTEGER_PROPERTY(DetectorNumber)

  Q_PROPERTY(int detectorType READ get_DetectorType WRITE set_DetectorType)
  QCEP_INTEGER_PROPERTY(DetectorType)
};

Q_DECLARE_METATYPE(QxrdSynchronizedDetectorChannel*)

#endif // QXRDSYNCHRONIZEDDETECTORCHANNEL_H
