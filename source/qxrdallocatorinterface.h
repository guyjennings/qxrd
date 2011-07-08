#ifndef QXRDALLOCATORINTERFACE_H
#define QXRDALLOCATORINTERFACE_H

#include <QObject>

class QxrdAllocatorInterface : public QObject
{
  Q_OBJECT
public:
  QxrdAllocatorInterface(QObject *parent=0);

  virtual void allocate(int sz, int width, int height) = 0;
  virtual void allocate(quint64 amt) = 0;
  virtual void deallocate(int sz, int width, int height) = 0;
  virtual void deallocate(quint64 amt) = 0;

  virtual double  allocatedMemoryMB() = 0;
};

#endif // QXRDALLOCATORINTERFACE_H
