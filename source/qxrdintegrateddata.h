#ifndef QXRDINTEGRATEDDATA_H
#define QXRDINTEGRATEDDATA_H

#include <QObject>
#include "qxrdimagedata.h"

class QxrdIntegratedData : public QObject
{
  Q_OBJECT

public:
  explicit QxrdIntegratedData(QxrdAllocatorInterface *alloc, QxrdDoubleImageDataPtr data, int maxSize, QObject *parent = 0);
  ~QxrdIntegratedData();

  void resize(int n);
  int size() const;
  void append(double x, double y);
  void set_Center(double cx, double cy);
  void set_Image(QxrdDoubleImageDataPtr image);
  QxrdDoubleImageDataPtr get_Image() const;

  QString get_XUnitsLabel() const;
  void set_XUnitsLabel(QString units);

  const double* x() const;
  const double* y() const;

  double cx() const;
  double cy() const;

  int allocatedMemoryMB();

private:
  QxrdImageDataObjectCounter m_ObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdDoubleImageDataPtr     m_Image;
  int                        m_MaxSize;
  int                        m_Size;
  const int                  m_AllocStep;

  QVector<double>            m_X, m_Y;
  double                     m_cx, m_cy;
  QString                    m_XUnitsLabel;
};

typedef QSharedPointer<QxrdIntegratedData>       QxrdIntegratedDataPtr;

#endif // QXRDINTEGRATEDDATA_H
