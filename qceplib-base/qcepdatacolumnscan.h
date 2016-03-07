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
  QcepDataColumnPtr column(int n);
  int rowCount() const;
  int columnCount() const;
  void resizeRows(int nRows);
  void appendColumn(QString title);
  double value(int col, int row);
  void setValue(int col, int row, double val);

public:
  QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QcepObject *parent);

  typedef QcepDataGroup inherited;

  static QcepDataColumnScanPtr newDataColumnScan(QcepSettingsSaverWPtr sav,
                                                 QString name,
                                                 QStringList cols,
                                                 int npts,
                                                 QcepObject *parent);


  static QScriptValue toColumnScanScriptValue(QScriptEngine *engine, const QcepDataColumnScanPtr &data);
  static void fromColumnScanScriptValue(const QScriptValue &obj, QcepDataColumnScanPtr &data);

public:
  Q_PROPERTY(int numPoints READ get_NumPoints WRITE set_NumPoints)
  QCEP_INTEGER_PROPERTY(NumPoints)

  Q_PROPERTY(QString title READ get_Title WRITE set_Title)
  QCEP_STRING_PROPERTY(Title)
};

#endif // QCEPDATACOLUMNSCAN_H
