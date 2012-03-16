#ifndef QXRDINTEGRATEDDATA_H
#define QXRDINTEGRATEDDATA_H

#include "qcepproperty.h"
#include <QObject>
#include <QSharedPointer>
#include "qxrdimagedataobjectcounter.h"
#include "qxrdimagedata.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdallocator-ptr.h"
#include "qxrdsettingssaver-ptr.h"

class QxrdIntegratedData : public QObject
{
  Q_OBJECT

public:
  explicit QxrdIntegratedData(QxrdSettingsSaverWPtr saver,
                              QxrdAllocatorWPtr alloc,
                              QxrdDoubleImageDataPtr data,
                              int typ, int maxSize, QObject *parent = 0);
  ~QxrdIntegratedData();

  void resize(int n);
  int size() const;
  void append(double x, double y);
  void set_Center(double cx, double cy);
  void set_Image(QxrdDoubleImageDataPtr image);
  QxrdDoubleImageDataPtr get_Image() const;

  QString get_XUnitsLabel() const;
  void set_XUnitsLabel(QString units);

  int get_Oversample() const;
  void set_Oversample(int ovs);

  const double* x() const;
  const double* y() const;

  double cx() const;
  double cy() const;

  int allocatedMemoryMB();

  Q_PROPERTY(QString title READ get_Title WRITE set_Title STORED false)
  QCEP_STRING_PROPERTY(Title)

private:
  QxrdImageDataObjectCounter m_ObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdDoubleImageDataPtr     m_Image;
  int                        m_MaxSize;
  int                        m_Size;
  const int                  m_AllocStep;

  QVector<double>            m_X, m_Y;
  double                     m_cx, m_cy;
  QString                    m_XUnitsLabel;
  int                        m_Oversample;
};

#endif // QXRDINTEGRATEDDATA_H
