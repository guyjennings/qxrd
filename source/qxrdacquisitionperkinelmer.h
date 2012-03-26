#ifndef QXRDACQUISITIONPERKINELMER_H
#define QXRDACQUISITIONPERKINELMER_H

//#ifdef HAVE_PERKIN_ELMER

#include "qxrdacquisition.h"
#include "qxrdperkinelmerplugininterface-ptr.h"
#include <Windows.h>
#include "Acq.h"
#include <QMutex>
#include <QVector>

class QxrdAcquisitionPerkinElmer : public QxrdAcquisition
{
  Q_OBJECT
public:
  QxrdAcquisitionPerkinElmer(QxrdSettingsSaverPtr saver,
                             QxrdExperimentWPtr doc,
                             QxrdDataProcessorWPtr proc,
                             QxrdAllocatorWPtr allocator);
  ~QxrdAcquisitionPerkinElmer();

};

//#endif // HAVE_PERKIN_ELMER

#endif // QXRDACQUISITIONPERKINELMER_H
