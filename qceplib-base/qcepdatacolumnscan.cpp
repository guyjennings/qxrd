#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include <QScriptEngine>

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QcepObject *parent) :
  QcepDataGroup(sav, name, parent),
  m_NumPoints(sav, this, "numPoints", 0, "Number of points in scan"),
  m_Title(sav, this, "title", "", "Scan Title")
{
  set_Type("Data Column Scan");
}

QcepDataColumnScanPtr QcepDataColumnScan::newDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts, QcepObject *parent)
{
  QcepDataColumnScanPtr res(new QcepDataColumnScan(sav, name, parent));

  foreach (QString col, cols) {
    res -> append(QcepDataColumn::newDataColumn(sav, col, npts, parent));
  }

  res -> set_NumPoints(npts);

  return res;
}

QString QcepDataColumnScan::description() const
{
  return tr("%1 rows x %2 cols").arg(rowCount()).arg(columnCount());
}

QScriptValue QcepDataColumnScan::toColumnScanScriptValue(QScriptEngine *engine, const QcepDataColumnScanPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataColumnScan::fromColumnScanScriptValue(const QScriptValue &obj, QcepDataColumnScanPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataColumnScan *qdobj = qobject_cast<QcepDataColumnScan*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataColumnScanPtr cs = qSharedPointerDynamicCast<QcepDataColumnScan>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

QcepDataColumnPtr QcepDataColumnScan::column(int n)
{
  QcepDataObjectPtr obj = item(n);

  return qSharedPointerDynamicCast<QcepDataColumn>(obj);
}

int QcepDataColumnScan::columnCount() const
{
  return childCount();
}

int QcepDataColumnScan::rowCount() const
{
  return get_NumPoints();
}

void QcepDataColumnScan::appendColumn(QString title)
{
  QcepDataColumnPtr col = QcepDataColumn::newDataColumn(saver(), title, 0, this);

  append(col);
}

void QcepDataColumnScan::resizeRows(int nRows)
{
  int nc = columnCount();

  for (int i=0; i<nc; i++) {
    QcepDataColumnPtr c = column(i);

    if (c) {
      c -> resize(nRows);
    }
  }

  set_NumPoints(nRows);
}

void QcepDataColumnScan::setValue(int col, int row, double val)
{
  QcepDataColumnPtr c = column(col);

  if (c) {
    c -> setValue(row, val);
  }
}

double QcepDataColumnScan::value(int col, int row)
{
  QcepDataColumnPtr c = column(col);

  if (c) {
    return c->value(row);
  } else {
    return qQNaN();
  }
}
