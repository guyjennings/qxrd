#ifndef QCEPDATAARRAY_H
#define QCEPDATAARRAY_H

#include <QObject>
#include "qcepdataobject.h"
#include "qcepdataarray-ptr.h"
#include <QVector>

class QcepDataArray : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataArray(QcepSettingsSaverWPtr saver,
                QString name,
                QVector<int> dims,
                QcepObject *parent);
  virtual ~QcepDataArray();

  static QcepDataArrayPtr newDataArray(QcepSettingsSaverWPtr saver,
                                       QString name,
                                       QVector<int> dims,
                                       QcepObject *parent);

  static QScriptValue toArrayScriptValue(QScriptEngine *engine, const QcepDataArrayPtr &data);
  static void fromArrayScriptValue(const QScriptValue &obj, QcepDataArrayPtr &data);

public slots:
  virtual QString description() const;

  QVector<int>    dimensions();
  QVector<double> vectorData();

private:
  QVector<int>    m_Dimensions;
  QVector<double> m_Data;
};

#endif // QCEPDATAARRAY_H
