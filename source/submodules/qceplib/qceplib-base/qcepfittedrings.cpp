#include "qcepfittedrings.h"
#include "qcepallocator.h"
#include "qcepmutexlocker.h"
#include "qcepdatacolumn.h"

QcepFittedRings::QcepFittedRings(QString name, int sz)
  : inherited(name, QStringList(), sz)
{
//  appendColumn("n");
//  appendColumn("x");
//  appendColumn("y");
//  appendColumn("a");
//  appendColumn("b");
//  appendColumn("rot");
}

void QcepFittedRings::initialize(QcepObjectWPtr parent)
{
  inherited::initialize(parent);

  appendColumn("n");
  appendColumn("x");
  appendColumn("y");
  appendColumn("a");
  appendColumn("b");
  appendColumn("rot");
}

//QcepFittedRingsPtr QcepFittedRings::newFittedRings(QString name)
//{
//  if (g_Allocator) {
//    QcepMutexLocker lock(__FILE__, __LINE__, g_Allocator->mutex());

//    if (g_Allocator->waitTillAvailable(
//          QcepAllocator::WaitTillAvailable,
//          QcepAllocator::columnScanSize(6, 0))) {
//      QcepFittedRingsPtr res(new QcepFittedRings(name, 0));

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

//  return QcepFittedRingsPtr();
//}

void QcepFittedRings::append(int n, double x, double y, double a, double b, double rot)
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

int QcepFittedRings::n(int i)
{
  return value(0, i);
}

double QcepFittedRings::x(int i)
{
  return value(1, i);
}

double QcepFittedRings::y(int i)
{
  return value(2, i);
}

double QcepFittedRings::a(int i)
{
  return value(3, i);
}

double QcepFittedRings::b(int i)
{
  return value(4, i);
}

double QcepFittedRings::rot(int i)
{
  return value(5, i);
}
