#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include <QSharedPointer>

#include "qcepmacros.h"

#include "qcepimagedata.h"
#include "qxrdimagedataobjectcounter.h"
#include "qxrdallocator-ptr.h"
#include "qxrdmaskdata.h"
#include "qxrdmaskdata-ptr.h"
#include "qxrdimagedata-ptr.h"
#include "qxrdexperiment-ptr.h"

template <typename T>
class QxrdImageData : public QcepImageData<T>
{
//  Q_OBJECT

public:
  QxrdImageData(QxrdSettingsSaverWPtr saver, QxrdAllocatorWPtr allocator, int typ, int width, int height, T def=0);
  ~QxrdImageData();

  QString rawFileName();

  template <typename T2>
  void copyImage(QSharedPointer< QxrdImageData<T2> > dest);

  template <typename T2>
      void copyFrom(const QSharedPointer< QxrdImageData<T2> > img);

  template <typename T2>
  void accumulateImage(QSharedPointer< QxrdImageData<T2> > image);

  template <typename T2>
  void add(QSharedPointer< QxrdImageData<T2> > image);

  template <typename T2>
  void subtract(QSharedPointer< QxrdImageData<T2> > image);

  template <typename T2>
  void multiply(QSharedPointer< QxrdImageData<T2> > image);

  template <typename T2>
  void divide(QSharedPointer< QxrdImageData<T2> > image);

  void setMask(QxrdMaskDataPtr mask, QxrdMaskDataPtr overflow);
  QxrdMaskDataPtr mask() const;
  QxrdMaskDataPtr overflow() const;

  int pixelsInRange(double min, double max);
  int overflowCount(double level) const;

  void markOverflows(QxrdMaskDataPtr overflow, double level);

  double correlate(QSharedPointer< QxrdImageData<T> > image, int dx, int dy, int mx, int my);

  void shiftImage(QSharedPointer< QxrdImageData<T> > image, double dx, double dy);

  double findMin() const;
  double findMax() const;
  double findAverage() const;

  void calculateRange();

public slots:
  double minValue();
  double maxValue();

public:
  int allocatedMemoryMB();

  void correctBadBackgroundSubtraction(QxrdDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures);

  void saveMetaData(QxrdExperimentWPtr expt);
  void saveMetaData(QString name, QxrdExperimentWPtr expt);

  void loadMetaData(QxrdExperimentWPtr expt = QxrdExperimentWPtr());
  void loadMetaData(QString name, QxrdExperimentWPtr expt);

protected:
  QxrdImageDataObjectCounter m_ObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdMaskDataPtr            m_Mask;
  QxrdMaskDataPtr            m_Overflow;
};

#endif
