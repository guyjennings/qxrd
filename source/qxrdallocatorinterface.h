#ifndef QXRDALLOCATORINTERFACE_H
#define QXRDALLOCATORINTERFACE_H

#include <QObject>

class QxrdAllocatorInterface : public QObject
{
  Q_OBJECT
public:
  QxrdAllocatorInterface(QObject *parent=0);

  virtual void allocate(int typ, int sz, int width, int height) = 0;
  virtual void allocate(int typ, quint64 amt) = 0;
  virtual void deallocate(int typ, int sz, int width, int height) = 0;
  virtual void deallocate(int typ, quint64 amt) = 0;

  virtual double  allocatedMemoryMB() = 0;

  enum {
    AllocateInt16      = 0,
    AllocateInt32      = 1,
    AllocateDouble     = 2,
    AllocateMask       = 3,
    AllocateIntegrated = 4
  };
};

#endif // QXRDALLOCATORINTERFACE_H
