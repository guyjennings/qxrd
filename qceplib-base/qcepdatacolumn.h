#ifndef QCEPDATACOLUMN_H
#define QCEPDATACOLUMN_H

#include "qceplib_global.h"
#include "qcepmacros.h"
#include <QObject>
#include <QVector>
#include "qcepproperty.h"
#include "qcepdataobject.h"
#include "qcepdataobject-ptr.h"
#include "qcepdatacolumn-ptr.h"

/*

  A named 1-Dimensional data vector

 */

typedef QString (*QcepDataColumnFormatter)(double);

class QCEP_EXPORT QcepDataColumn : public QcepDataObject
{
  Q_OBJECT

public:
  enum ColumnType {
    NormalColumn,
    XValueColumn,
    CountsColumn,
    WeightColumn,
    RatioColumn
  };

public:
  Q_INVOKABLE QcepDataColumn(QString name,
                 int npts = 0,
                 ColumnType colType = NormalColumn,
                 int col1 = -1,
                 int col2 = -1);

  virtual ~QcepDataColumn();

  void readSettings(QSettings *settings);
  void writeSettings(QSettings *settings);

public:
  Q_PROPERTY(int columnType READ get_ColumnType WRITE set_ColumnType)
  QCEP_INTEGER_PROPERTY(ColumnType)

  Q_PROPERTY(int column1 READ get_Column1 WRITE set_Column1)
  QCEP_INTEGER_PROPERTY(Column1)

  Q_PROPERTY(int column2 READ get_Column2 WRITE set_Column2)
  QCEP_INTEGER_PROPERTY(Column2)

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

  void append(double v);

  virtual void add(QcepDataColumnPtr col);
  virtual void subtract(QcepDataColumnPtr col);
  virtual void copy(QcepDataColumnPtr col);
  virtual void multiply(QcepDataColumnPtr col);
  virtual void divide(QcepDataColumnPtr col);
  virtual void concat(QcepDataColumnPtr col);

  virtual void add(double val);
  virtual void subtract(double val);
  virtual void copy(double val);
  virtual void multiply(double val);
  virtual void divide(double val);
  virtual void concat(double val);

  QcepDataColumnPtr duplicate();

public:
  static QScriptValue toColumnScriptValue(QScriptEngine *engine, const QcepDataColumnPtr &data);
  static void fromColumnScriptValue(const QScriptValue &obj, QcepDataColumnPtr &data);

private:
  int             m_NPoints;
  QVector<double> m_Vector;
  QcepDataColumnFormatter       m_Formatter;
};

Q_DECLARE_METATYPE(QcepDataColumn*)

#endif // QCEPDATACOLUMN_H
