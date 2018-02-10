#ifndef QCEPDATASET_H
#define QCEPDATASET_H

#include "qceplib_global.h"
#include <QObject>
#include "qcepdatagroup.h"

class QCEP_EXPORT QcepDataset : public QcepDataGroup
{
  Q_OBJECT

private:
  typedef QcepDataGroup inherited;

public:
  Q_INVOKABLE QcepDataset(QString name);

  void initialize(QcepObjectWPtr parent);

//  void readSettings(QSettings *settings, QString section);
//  void writeSettings(QSettings *settings, QString section);
};

Q_DECLARE_METATYPE(QcepDataset*)

#endif // QCEPDATASET_H
