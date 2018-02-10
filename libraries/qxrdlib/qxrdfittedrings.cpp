#include "qxrdfittedrings.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qcepdatacolumn.h"

QxrdFittedRings::QxrdFittedRings(QString name, int sz)
  : inherited(name, QStringList(), sz)
{
//  appendColumn("n");
//  appendColumn("x");
//  appendColumn("y");
//  appendColumn("a");
//  appendColumn("b");
//  appendColumn("rot");
}

void QxrdFittedRings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  appendColumn("n");
  appendColumn("x");
  appendColumn("y");
  appendColumn("a");
  appendColumn("b");
  appendColumn("rot");
}

//QxrdFittedRingsPtr QxrdFittedRings::newFittedRings(QString name)
//{
//  if (g_Allocator) {
//    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

//    if (g_Allocator->waitTillAvailable(
//          QcepAllocator::WaitTillAvailable,
//          QcepAllocator::columnScanSize(6, 0))) {
//      QxrdFittedRingsPtr res(new QxrdFittedRings(name, 0));

//      if (res) {
//        res -> moveToThread(NULL);
//        res -> appendColumn("n");
//        res -> appendColumn("x");
//        res -> appendColumn("y");
//        res -> appendColumn("a");
//        res -> appendColumn("b");
//        res -> appendColumn("rot");
//      }

//      return res;
//    }
//  }

//  return QxrdFittedRingsPtr();
//}

void QxrdFittedRings::append(int n, double x, double y, double a, double b, double rot)
{
  if (columnCount()>=6) {
    column(0)->append(n);
    column(1)->append(x);
    column(2)->append(y);
    column(3)->append(a);
    column(4)->append(b);
    column(5)->append(rot);

    prop_NumPoints()->incValue(1);
  }
}

int QxrdFittedRings::n(int i)
{
  return value(0, i);
}

double QxrdFittedRings::x(int i)
{
  return value(1, i);
}

double QxrdFittedRings::y(int i)
{
  return value(2, i);
}

double QxrdFittedRings::a(int i)
{
  return value(3, i);
}

double QxrdFittedRings::b(int i)
{
  return value(4, i);
}

double QxrdFittedRings::rot(int i)
{
  return value(5, i);
}
