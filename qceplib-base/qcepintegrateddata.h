#ifndef QCEPINTEGRATEDDATA_H
#define QCEPINTEGRATEDDATA_H

#include "qcepproperty.h"
#include "qcepdataobject.h"
//#include "qcepimagedata-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QcepIntegratedData : public QcepDataObject
{
  Q_OBJECT

public:
  Q_INVOKABLE QcepIntegratedData(QString name,
      int maxSize=0);

  ~QcepIntegratedData();

public slots:
  virtual QString description() const;

  double x(int i) const;
  double y(int i) const;
  void resize(int n);
  int size() const;

public:
  void append(double x, double y);
  void setValue(int n, double x, double y);
  void selfNormalize(double minx, double maxx);

  void set_Center(double cx, double cy);

  QString get_XUnitsLabel() const;
  void set_XUnitsLabel(QString units);

  int get_Oversample() const;
  void set_Oversample(int ovs);

  const double* x() const;
  const double* y() const;

  double cx() const;
  double cy() const;

  static QScriptValue toIntegratedDataScriptValue(QScriptEngine *engine, const QcepIntegratedDataPtr &data);
  static void fromIntegratedDataScriptValue(const QScriptValue &obj, QcepIntegratedDataPtr &data);

private:
  int                        m_MaxSize;
  int                        m_Size;
  const int                  m_AllocStep;

  QVector<double>            m_X, m_Y;
  double                     m_cx, m_cy;
  QString                    m_XUnitsLabel;
  int                        m_Oversample;
};

Q_DECLARE_METATYPE(QcepIntegratedDataPtr)
Q_DECLARE_METATYPE(QcepIntegratedData*)

#endif // QCEPINTEGRATEDDATA_H
