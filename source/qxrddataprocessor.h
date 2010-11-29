#ifndef QXRDDATAPROCESSOR_H
#define QXRDDATAPROCESSOR_H

#include "qcepmacros.h"

#include <QObject>
#include <QReadWriteLock>
#include <QAtomicInt>
#include <QWaitCondition>

#include "qxrdforwardtypes.h"
#include "qcepproperty.h"
#include "qxrdsettings.h"
#include "qxrdimagequeue.h"
#include "qxrdimagedata.h"
#include "qxrdmaskdata.h"
#include "qxrddoubleimagedata.h"
#include "qxrdacquisition.h"
#include "qxrddataprocessorthreaded.h"

#include <qwt_double_rect.h>

class QxrdDataProcessor : public QxrdDataProcessorThreaded
{
  Q_OBJECT;

public:
  QxrdDataProcessor(QxrdAcquisitionPtr acq, QxrdAllocatorPtr allocator, QxrdFileSaverThreadPtr saver, QObject *parent=0);
  ~QxrdDataProcessor();

//  virtual void loadData(QString name) = 0;
//  virtual void saveData(QString name, int canOverwrite=NoOverwrite) = 0;
//  virtual void loadDark(QString name) = 0;
//  virtual void saveDark(QString name, int canOverwrite=NoOverwrite) = 0;
//  virtual void loadMask(QString name) = 0;
//  virtual void saveMask(QString name, int canOverwrite=NoOverwrite) = 0;
//////  void loadDarkImage(QString name);
//  virtual void loadBadPixels(QString name) = 0;
//  virtual void saveBadPixels(QString name, int canOverwrite=NoOverwrite) = 0;
//  virtual void loadGainMap(QString name) = 0;
//  virtual void saveGainMap(QString name, int canOverwrite=NoOverwrite) = 0;

//  virtual void newMask() = 0;

//  virtual int  maskStackSize() = 0;
//  virtual int  maskStackPosition(int pos) = 0;
//  virtual void pushMaskStack(QxrdMaskDataPtr mask = QxrdMaskDataPtr()) = 0;
//  virtual void popMaskStack(int amount=1) = 0;
//  virtual void clearMaskStack() = 0;
//  virtual void rollMaskStack(int amount) = 0;
//  virtual void exchangeMaskStack(int pos) = 0;
//  virtual void andMaskStack(int pos) = 0;
//  virtual void orMaskStack(int pos) = 0;
//  virtual void xorMaskStack(int pos) = 0;
//  virtual void andNotMaskStack(int pos) = 0;
//  virtual void orNotMaskStack(int pos) = 0;
//  virtual void xorNotMaskStack(int pos) = 0;
//  virtual void invertMaskStack(int pos) = 0;
//  virtual void hideMaskAllStack(int pos) = 0;
//  virtual void showMaskAllStack(int pos) = 0;
//  virtual void hideMaskRangeStack(int pos) = 0;
//  virtual void showMaskRangeStack(int pos) = 0;

//  virtual void newData(QxrdDoubleImageDataPtr image, QxrdMaskDataPtr overflow) = 0;
//  virtual void newDarkImage(QxrdDoubleImageDataPtr image) = 0;
//  virtual void newBadPixelsImage(QxrdDoubleImageDataPtr image) = 0;
//  virtual void newGainMapImage(QxrdDoubleImageDataPtr image) = 0;

//  virtual void newLogFile(QString path) = 0;
//  virtual void logMessage(QString msg) = 0;
//  virtual void closeLogFile() = 0;

//  virtual int status(double delay) = 0;
//  virtual void loadDefaultImages() = 0;
//  virtual void readSettings(QxrdSettings &settings, QString section) = 0;
//  virtual void writeSettings(QxrdSettings &settings, QString section) = 0;

//  virtual void setAcquisition(QxrdAcquisitionPtr acq) = 0;
//  virtual void setWindow(QxrdWindowPtr win) = 0;

//  virtual void writeOutputScan(QxrdIntegratedDataPtr data) = 0;
//  virtual void displayIntegratedData(QxrdIntegratedDataPtr data) = 0;

//  virtual double estimatedProcessingTime(double estSerTime, double estParallelTime) = 0;

//  virtual QxrdDoubleImageDataPtr data() const = 0;
//  virtual QxrdDoubleImageDataPtr darkImage() const = 0;
//  virtual QxrdMaskDataPtr        mask() const = 0;
//  virtual QxrdMaskDataPtr        overflow() const = 0;

//  virtual QxrdCenterFinderPtr    centerFinder() const = 0;
//  virtual QxrdIntegratorPtr      integrator() const = 0;
//  virtual QxrdGenerateTestImagePtr generateTestImage() const = 0;

//  virtual QxrdRingSetFitParametersPtr initialRingSetFitParameters() const = 0;
//  virtual QxrdRingSetSampledDataPtr   initialRingSetData() const = 0;
//  virtual QxrdRingSetFitParametersPtr refinedRingSetFitParameters() const = 0;
//  virtual QxrdRingSetSampledDataPtr   refinedRingSetData() const = 0;

};

#endif
