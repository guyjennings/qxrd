#ifndef QXRDTESTSCANGENERATOR_H
#define QXRDTESTSCANGENERATOR_H

#include "qxrdtestgenerator.h"
#include "qcepdatacolumnscan-ptr.h"
#include "qcepdatacolumnscan.h"

class QxrdTestScanGenerator : public QxrdTestGenerator
{
  Q_OBJECT

public:
  explicit QxrdTestScanGenerator(QString name);

  void start();

protected slots:
  void generateTest();

signals:
  void newScanAvailable(QcepDataColumnScanPtr data);

public slots:
};

Q_DECLARE_METATYPE(QcepDataColumnScanPtr)

#endif // QXRDTESTSCANGENERATOR_H
