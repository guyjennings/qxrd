#include "qcepdatacolumnscan.h"
#include "qcepdatacolumn.h"
#include "qcepdatacolumn-ptr.h"
#include <QScriptEngine>

QcepDataColumnScan::QcepDataColumnScan(QcepSettingsSaverWPtr sav, QString name) :
  QcepDataGroup(sav, name),
  m_NumPoints(sav, this, "numPoints", 0, "Number of points in scan")
{
  set_Type("Data Column Scan");
}

QcepDataColumnScanPtr QcepDataColumnScan::newDataColumnScan(QcepSettingsSaverWPtr sav, QString name, QStringList cols, int npts)
{
  QcepDataColumnScanPtr res(new QcepDataColumnScan(sav, name));

  foreach (QString col, cols) {
    res -> append(QcepDataColumn::newDataColumn(sav, col, npts));
  }

  res -> set_NumPoints(npts);
  res -> set_Description(tr("%1 rows x %2 cols").arg(npts).arg(cols.count()));

  return res;
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
        QcepDataColumnScanPtr cs = qSharedPointerCast<QcepDataColumnScan>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}

