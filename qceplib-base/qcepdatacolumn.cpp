#include "qcepdatacolumn.h"
#include <QScriptEngine>

QcepDataColumn::QcepDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts) :
  QcepDataObject(saver, name),
  m_NPoints(npts)
{
  set_Type("Data Column");

  resize(m_NPoints);

  set_Description(tr("%1 rows").arg(m_NPoints));
}

QcepDataColumnPtr QcepDataColumn::newDataColumn(QcepSettingsSaverWPtr saver, QString name, int npts)
{
  QcepDataColumnPtr res(new QcepDataColumn(saver, name, npts));

  return res;
}

QScriptValue QcepDataColumn::toColumnScriptValue(QScriptEngine *engine, const QcepDataColumnPtr &data)
{
  return engine->newQObject(data.data());
}

void QcepDataColumn::fromColumnScriptValue(const QScriptValue &obj, QcepDataColumnPtr &data)
{
  QObject *qobj = obj.toQObject();

  if (qobj) {
    QcepDataColumn *qdobj = qobject_cast<QcepDataColumn*>(qobj);

    if (qdobj) {
      QcepDataObjectPtr p = qdobj->sharedFromThis();

      if (p) {
        QcepDataColumnPtr cs = qSharedPointerCast<QcepDataColumn>(p);

        if (cs) {
          data = cs;
        }
      }
    }
  }
}
