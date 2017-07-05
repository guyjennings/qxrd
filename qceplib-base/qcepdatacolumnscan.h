#ifndef QCEPDATACOLUMNSCAN_H
#define QCEPDATACOLUMNSCAN_H

#include <QObject>
#include "qcepdatagroup.h"
#include "qcepdatacolumnscan-ptr.h"

class QcepDataColumnScan : public QcepDataGroup
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepDataColumnScan(QString name,
      QStringList cols = QStringList(), int sz=0);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public slots:
  virtual QString description() const;
  QcepDataColumnPtr column(int n);
  int rowCount() const;
  int columnCount() const;
  void resizeRows(int nRows);
  QcepDataColumnPtr appendColumn(QString title);
  double value(int col, int row);
  void setValue(int col, int row, double val);

  void add(QcepDataColumnScanPtr scan);
  void subtract(QcepDataColumnScanPtr scan);
  void concat(QcepDataColumnScanPtr scan);

public:

  static QScriptValue toColumnScanScriptValue(QScriptEngine *engine, const QcepDataColumnScanPtr &data);
  static void fromColumnScanScriptValue(const QScriptValue &obj, QcepDataColumnScanPtr &data);

private:
  bool checkCompatibility(QcepDataColumnScanPtr scan);
  bool checkAppendability(QcepDataColumnScanPtr scan);

public:
  Q_PROPERTY(int numPoints READ get_NumPoints WRITE set_NumPoints)
  QCEP_INTEGER_PROPERTY(NumPoints)
};

#endif // QCEPDATACOLUMNSCAN_H
