#ifndef QCEPDATACOLUMNSCAN_H
#define QCEPDATACOLUMNSCAN_H

#include <QObject>
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan-ptr.h"

class QcepDataColumnScan : public QcepDataGroup
{
  Q_OBJECT

public slots:
  virtual QString description() const;

public:
  QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name);

  typedef QcepDataGroup inherited;

  static QcepDataColumnScanPtr newDataColumnScan(QcepSettingsSaverWPtr sav,
                                                 QString name,
                                                 QStringList cols,
                                                 int npts);

  static QScriptValue toColumnScanScriptValue(QScriptEngine *engine, const QcepDataColumnScanPtr &data);
  static void fromColumnScanScriptValue(const QScriptValue &obj, QcepDataColumnScanPtr &data);

public:
  Q_PROPERTY(int numPoints READ get_NumPoints WRITE set_NumPoints)
  QCEP_INTEGER_PROPERTY(NumPoints)
};

#endif // QCEPDATACOLUMNSCAN_H
