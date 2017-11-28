#ifndef QCEPDATAARRAY_H
#define QCEPDATAARRAY_H

#include "qceplib_global.h"
#include <QObject>
#include "qcepdataobject.h"
#include "qcepdataarray-ptr.h"
#include <QVector>

class QCEP_EXPORT QcepDataArray : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataArray(QString name,
                QVector<int> dims = QVector<int>());
  virtual ~QcepDataArray();

  static QScriptValue toArrayScriptValue(QScriptEngine *engine, const QcepDataArrayPtr &data);
  static void fromArrayScriptValue(const QScriptValue &obj, QcepDataArrayPtr &data);

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public slots:
  virtual QString description() const;

  QVector<int>    dimensions();
  QVector<double> vectorData();

private:
  int arrayCount(QVector<int> dims);

private:
  QVector<int>    m_Dimensions;
  QVector<double> m_Data;
};

#endif // QCEPDATAARRAY_H
