#ifndef QCEPDATASET_H
#define QCEPDATASET_H

#include <QObject>
#include "qcepdatagroup.h"

class QcepDataset : public QcepDataGroup
{
  Q_OBJECT

public:
  QcepDataset(QcepObjectWPtr parent,
              QcepSettingsSaverWPtr saver, QString name);

//  void readSettings(QSettings *settings, QString section);
//  void writeSettings(QSettings *settings, QString section);
};

#endif // QCEPDATASET_H
