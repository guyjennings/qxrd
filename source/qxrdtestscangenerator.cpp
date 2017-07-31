#include "qxrdtestscangenerator.h"
#include "qcepallocator.h"
#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"

QxrdTestScanGenerator::QxrdTestScanGenerator(QString name)
  : QxrdTestGenerator(name)
{
}

void QxrdTestScanGenerator::start()
{
  printMessage("Test Scan Generator Started");
}

void QxrdTestScanGenerator::generateTest()
{
  int ind = get_TestIndex();

  prop_TestIndex()->incValue(1);

  QStringList cols;
  cols.append("X");
  cols.append("Y");

  QcepDataColumnScanPtr scan =
      QcepAllocator::newColumnScan(tr("newScan-%1").arg(ind),
                                   cols, 1024, QcepAllocator::NullIfNotAvailable);

  if (scan) {
    QcepDataColumnPtr xCol = scan->column(0);
    QcepDataColumnPtr yCol = scan->column(1);

    if (xCol && yCol) {
      int nXPts = xCol->count();
      int nYPts = yCol->count();

      if (nXPts && (nXPts == nYPts)) {
        for (int i=0; i<nXPts; i++) {
          xCol->setValue(i, i);
          yCol->setValue(i, sin((i+ind*5)/18.0));
        }
      }
    }

    emit newScanAvailable(scan);
  }
}
