#ifndef QCEPINTEGRATEDDATA_H
#define QCEPINTEGRATEDDATA_H

#include "qcepproperty.h"
#include "qcepdataobject.h"
#include "qcepimagedata-ptr.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepintegrateddata-ptr.h"

class QcepIntegratedData : public QcepDataObject
{
  Q_OBJECT

public:
  explicit QcepIntegratedData(QcepSettingsSaverWPtr saver,
                              QcepDoubleImageDataPtr data,
                              int maxSize, QcepObject *parent);
  ~QcepIntegratedData();

public slots:
  virtual QString description() const;

  double x(int i) const;
  double y(int i) const;
  void resize(int n);
  int size() const;

public:
  void append(double x, double y);
  void set_Center(double cx, double cy);
  void set_Image(QcepDoubleImageDataPtr image);
  QcepDoubleImageDataPtr get_Image() const;

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

  Q_PROPERTY(QString title READ get_Title WRITE set_Title STORED false)
  QCEP_STRING_PROPERTY(Title)

private:
  QcepDoubleImageDataPtr     m_Image;
  int                        m_MaxSize;
  int                        m_Size;
  const int                  m_AllocStep;

  QVector<double>            m_X, m_Y;
  double                     m_cx, m_cy;
  QString                    m_XUnitsLabel;
  int                        m_Oversample;
};

Q_DECLARE_METATYPE(QcepIntegratedDataPtr)

#endif // QCEPINTEGRATEDDATA_H
