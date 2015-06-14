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
#include "qxrdimagedata-ptr.h"

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

  double correlate(QSharedPointer< QxrdImageData<T> > image, int dx, int dy, int mx, int my);

  void shiftImage(QSharedPointer< QxrdImageData<T> > image, double dx, double dy);

  T findMin() const;
  T findMax() const;
  double findAverage() const;

  int allocatedMemoryMB();

  void correctBadBackgroundSubtraction(QxrdDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures);

  void saveMetaData(QxrdExperimentWPtr expt);
  void saveMetaData(QString name, QxrdExperimentWPtr expt);

  void loadMetaData(QxrdExperimentWPtr expt = QxrdExperimentWPtr());
  void loadMetaData(QString name, QxrdExperimentWPtr expt);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QSharedPointer< QxrdImageData<T> > &data);
  static void fromScriptValue(const QScriptValue &obj, QSharedPointer< QxrdImageData<T> > &data);

protected:
  QxrdImageDataObjectCounter m_ObjectCounter; /* global counter to track allocation of QxrdImageData objects */
  QxrdMaskDataPtr            m_Mask;
  QxrdMaskDataPtr            m_Overflow;
};

//QScriptValue toInt16ImageScriptValue(QScriptEngine *engine, const QxrdInt16ImageDataPtr &data);
//void fromInt16ImageScriptValue(const QScriptValue &obj, QxrdInt16ImageDataPtr &data);

//QScriptValue toInt32ImageScriptValue(QScriptEngine *engine, const QxrdInt32ImageDataPtr &data);
//void fromInt32ImageScriptValue(const QScriptValue &obj, QxrdInt32ImageDataPtr &data);

//QScriptValue toDoubleImageScriptValue(QScriptEngine *engine, const QxrdDoubleImageDataPtr &data);
//void fromDoubleImageScriptValue(const QScriptValue &obj, QxrdDoubleImageDataPtr &data);

Q_DECLARE_METATYPE(QxrdInt16ImageDataPtr)
Q_DECLARE_METATYPE(QxrdInt32ImageDataPtr)
Q_DECLARE_METATYPE(QxrdDoubleImageDataPtr)

#endif
