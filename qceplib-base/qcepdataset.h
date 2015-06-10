#ifndef QCEPDATASET_H
#define QCEPDATASET_H

#include <QObject>
#include "qcepdatagroup.h"

class QcepDataset : public QcepDataGroup
{
  Q_OBJECT

public:
  QcepDataset(QcepSettingsSaverWPtr saver, QString name, QcepDataObjectWPtr parent = QcepDataObjectWPtr());
};

#endif // QCEPDATASET_H
