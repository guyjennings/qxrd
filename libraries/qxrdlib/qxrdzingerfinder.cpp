#include "qxrdzingerfinder.h"
#include "qcepmaskdata.h"
#include "qxrdmaskstack-ptr.h"
#include "qxrdmaskstack.h"
#include "qcepallocator.h"

QxrdZingerFinder::QxrdZingerFinder(QString name) :
  QcepObject(name),
  m_ZingerAlgorithm(this, "zingerAlgorithm", 0, "Zinger Detection Algorithm"),
  m_ZingerMask(this, "zingerMask", 0, "Zinger Mask Source"),
  m_ZingerDestination(this, "zingerDestination", -1, "Zinger Destination"),
  m_ZingerSize1(this, "zingerSize1", 1, "Inner Zinger Search Box Size"),
  m_ZingerSize2(this, "zingerSize2", 5, "Outer Zinger Search Box Size"),
  m_ZingerThreshold(this, "zingerThreshold", 2.5, "Zinger Detection Threshold")
{
#ifndef QT_NO_DEBUG
  printf("Constructing zinger finder\n");
#endif
}

QxrdZingerFinder::~QxrdZingerFinder()
{
#ifndef QT_NO_DEBUG
  printf("Deleting zinger finder\n");
#endif
}

void QxrdZingerFinder::findZingers()
{
  //TODO: reimplement
//  QcepMaskDataPtr  mask;
//  QxrdMaskStackPtr masks = maskStack();

//  if (get_ZingerMask() == ZingerNoMask) {
//    mask = QcepMaskDataPtr();
////TODO: rework
////  } else if (get_ZingerMask() == ZingerDataMask && m_Data) {
////    mask = m_Data->mask();
//  } else if (masks) {
//    mask = masks->mask(get_ZingerMask());
//  }

//  int wid = m_Data->get_Width();
//  int ht  = m_Data->get_Height();
//  int sz1 = get_ZingerSize1();
//  int sz2 = get_ZingerSize2();
//  double thr = get_ZingerThreshold();

//  QcepMaskDataPtr dest
//      = QcepAllocator::newMask("zingers", wid, ht, 1, QcepAllocator::NullIfNotAvailable);

//  if (dest) {
//    for (int y=0; y<ht; y++) {
//      for (int x=0; x<wid; x++) {
//        if (mask==NULL || mask->value(x,y)) {
//          double sum = 0, n = 0;
//          double val = 0;

//          for (int dy=-sz2; dy<=sz2; dy++) {
//            for (int dx=-sz2; dx<=sz2; dx++) {
//              if (mask==NULL || mask->value(x+dx, y+dy)) {
//                if (dx == 0 && dy == 0) {
//                  val = m_Data->getImageData(x+dx, y+dy);
//                } else {
//                  sum += m_Data->getImageData(x+dx, y+dy);
//                  n   += 1;
//                }
//              }
//            }
//          }

//          if (n > 0 && fabs(val-sum/n) >= thr) {
//            dest->setValue(x,y,0);
//          }
//        }
//      }
//    }

//    if (sz1 > 0) {
//      for (int i=0; i<sz1; i++) {
//        dest->growMask();
//      }
//    }

//    if (mask) {
//      for (int y=0; y<ht; y++) {
//        for (int x=0; x<wid; x++) {
//          if (mask->value(x,y) == 0) {
//            dest->setValue(x,y,0);
//          }
//        }
//      }
//    }

//    m_MaskStack -> push(dest);
//  }
}
