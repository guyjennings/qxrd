#ifndef QCEPDATACOLUMN_H
#define QCEPDATACOLUMN_H

#include <QObject>
#include <QVector>
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatacolumn-ptr.h"

/*

  A named 1-Dimensional data vector

 */

typedef QString (*QcepDataColumnFormatter)(double);

class QcepDataColumn : public QcepDataObject
{
  Q_OBJECT

public:
  QcepDataColumn(QcepSettingsSaverWPtr saver,
                 QString name,
                 int npts,
                 QcepObject *parent);
  virtual ~QcepDataColumn();

public slots:
  virtual QString description() const;
  virtual int columnCount() const;
  virtual int rowCount() const;
  virtual double value(int i) const;
  virtual void setValue(int i, const double value);
  virtual void resize(int n);
  virtual int count() const;
  virtual double *data();
  virtual QcepDataColumnFormatter formatter();
  void setFormatter(QcepDataColumnFormatter f);

public:
  static QcepDataColumnPtr newDataColumn(QcepSettingsSaverWPtr saver,
                                         QString name,
                                         int npts,
                                         QcepObject *parent);

  static QScriptValue toColumnScriptValue(QScriptEngine *engine, const QcepDataColumnPtr &data);
  static void fromColumnScriptValue(const QScriptValue &obj, QcepDataColumnPtr &data);

private:
  int             m_NPoints;
  QVector<double> m_Vector;
  QcepDataColumnFormatter       m_Formatter;
};

#endif // QCEPDATACOLUMN_H
