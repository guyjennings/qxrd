#ifndef QXRDALLOCATOR_H
#define QXRDALLOCATOR_H

//#include "qcepmacros.h"
//#include "qcepobject.h"

//#include <QWeakPointer>
//#include <QSharedPointer>

//#include "qcepallocator.h"
//#include "qcepallocator-ptr.h"
//#include "qxrdimagedata-ptr.h"
//#include "qxrdintegrateddata-ptr.h"
//#include "QcepMaskData-ptr.h"

//class QxrdAllocator : public QcepAllocator
//{
//  Q_OBJECT

//public:
//  QxrdAllocator(QcepSettingsSaverPtr saver);
//  virtual ~QxrdAllocator();

////  void readSettings(QSettings *settings, QString section);
////  void writeSettings(QSettings *settings, QString section);

//public slots:
//public:
//  enum {
//    AllocateInt16      = 0,
//    AllocateInt32      = 1,
//    AllocateDouble     = 2,
//    AllocateMask       = 3,
//    AllocateIntegrated = 4
//  };

//  enum AllocationStrategy {
//    WaitTillAvailable,
//    NullIfNotAvailable,
//    AllocateFromReserve,
//    AlwaysAllocate
//  };

//  static QcepInt16ImageDataPtr newInt16Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
//  static QcepInt32ImageDataPtr newInt32Image(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
//  static QcepDoubleImageDataPtr newDoubleImage(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height);
//  static QcepMaskDataPtr newMask(QxrdAllocatorWPtr allocw, AllocationStrategy strat, int width, int height, int def=1);
//  static QxrdIntegratedDataPtr newIntegratedData(QxrdAllocatorWPtr allocw, AllocationStrategy strat, QcepDoubleImageDataPtr image);

//  static QcepDoubleImageDataPtr newDoubleImage(QString name, int width, int height);
//  static QxrdIntegratedDataPtr  newIntegratedData(QString name, int size);

//  static void newDoubleImageAndIntegratedData(QxrdAllocatorWPtr allocw,
//                                              AllocationStrategy strat,
//                                       int width, int height,
//                                       QcepDoubleImageDataPtr &img,
//                                       QxrdIntegratedDataPtr &integ);


//  static void maskDeleter(QcepMaskData *mask);
//  static void int16Deleter(QcepInt16ImageData *img);
//  static void int32Deleter(QcepInt32ImageData *img);
//  static void doubleDeleter(QcepDoubleImageData *img);
//  static void integratedDeleter(QxrdIntegratedData *integ);



};

#endif
