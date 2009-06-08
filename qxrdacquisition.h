#ifndef QXRDACQUISITION_H
#define QXRDACQUISITION_H

#include <QObject>

#include "qxrdrasterdata.h"
#include "qxrdimagequeue.h"
#include "qxrdacquisitionperkinelmer.h"

class QxrdAcquisition : public QxrdAcquisitionPerkinElmer
{
  Q_OBJECT;

public:
  QxrdAcquisition(QxrdAcquisitionThread *thread);
  typedef QxrdAcquisitionPerkinElmer inherited;
};

#endif
