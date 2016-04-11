#ifndef QCEPIMAGEDATA_H
#define QCEPIMAGEDATA_H

#include "qcepmacros.h"
#include <QMutex>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QFileInfo>
#include <QDateTime>
#include <qmath.h>
#include <stdio.h>

#include "qcepdataobject.h"
#include "qcepproperty.h"
#include "qcepsettingssaver-ptr.h"
#include "qcepexperiment-ptr.h"
#include "qcepmaskdata-ptr.h"
#include "qcepimagedata-ptr.h"

typedef struct tiff TIFF;

class QcepImageDataBase : public QcepDataObject
{
  Q_OBJECT

public:
  QcepImageDataBase(QcepSettingsSaverWPtr saver, int width, int height, int size);
  virtual ~QcepImageDataBase();

  Q_PROPERTY(int width READ get_Width WRITE set_Width)
  QCEP_INTEGER_PROPERTY(Width)

  Q_PROPERTY(int height READ get_Height WRITE set_Height)
  QCEP_INTEGER_PROPERTY(Height)

  Q_PROPERTY(double hStart READ get_HStart WRITE set_HStart)
  QCEP_DOUBLE_PROPERTY(HStart)

  Q_PROPERTY(double hStep READ get_HStep WRITE set_HStep)
  QCEP_DOUBLE_PROPERTY(HStep)

  Q_PROPERTY(double vStart READ get_VStart WRITE set_VStart)
  QCEP_DOUBLE_PROPERTY(VStart)

  Q_PROPERTY(double vStep READ get_VStep WRITE set_VStep)
  QCEP_DOUBLE_PROPERTY(VStep)

  Q_PROPERTY(QString hLabel READ get_HLabel WRITE set_HLabel)
  QCEP_STRING_PROPERTY(HLabel)

  Q_PROPERTY(QString hUnits READ get_HUnits WRITE set_HUnits)
  QCEP_STRING_PROPERTY(HUnits)

  Q_PROPERTY(QString vLabel READ get_VLabel WRITE set_VLabel)
  QCEP_STRING_PROPERTY(VLabel)

  Q_PROPERTY(QString vUnits READ get_VUnits WRITE set_VUnits)
  QCEP_STRING_PROPERTY(VUnits)

  Q_PROPERTY(int dataType READ get_DataType WRITE set_DataType)
  QCEP_INTEGER_PROPERTY(DataType)

  Q_PROPERTY(QString dataTypeName READ get_DataTypeName WRITE set_DataTypeName)

  Q_PROPERTY(QString fileBase READ get_FileBase WRITE set_FileBase)
  QCEP_STRING_PROPERTY(FileBase)

//  Q_PROPERTY(QString title READ get_Title WRITE set_Title)
//  QCEP_STRING_PROPERTY(Title)

  Q_PROPERTY(int readoutMode READ get_ReadoutMode WRITE set_ReadoutMode)
  QCEP_INTEGER_PROPERTY(ReadoutMode)

  Q_PROPERTY(double exposureTime READ get_ExposureTime WRITE set_ExposureTime)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(int summedExposures READ get_SummedExposures WRITE set_SummedExposures)
  QCEP_INTEGER_PROPERTY(SummedExposures)

  Q_PROPERTY(int imageSequenceNumber READ get_ImageSequenceNumber WRITE set_ImageSequenceNumber)
  QCEP_INTEGER_PROPERTY(ImageSequenceNumber)

  Q_PROPERTY(int imageNumber READ get_ImageNumber WRITE set_ImageNumber)
  QCEP_INTEGER_PROPERTY(ImageNumber)

  Q_PROPERTY(int phaseNumber READ get_PhaseNumber WRITE set_PhaseNumber)
  QCEP_INTEGER_PROPERTY(PhaseNumber)

  Q_PROPERTY(int nPhases READ get_NPhases WRITE set_NPhases)
  QCEP_INTEGER_PROPERTY(NPhases)

  Q_PROPERTY(QDateTime dateTime READ get_DateTime WRITE set_DateTime)
  QCEP_DATETIME_PROPERTY(DateTime)

  Q_PROPERTY(QString dateString READ get_DateString WRITE set_DateString)
  //  QCEP_STRING_PROPERTY(DateString)

  Q_PROPERTY(double timeStamp READ get_TimeStamp WRITE set_TimeStamp)
  QCEP_DOUBLE_PROPERTY(TimeStamp)

  Q_PROPERTY(int hBinning READ get_HBinning WRITE set_HBinning)
  QCEP_INTEGER_PROPERTY(HBinning)

  Q_PROPERTY(int vBinning READ get_VBinning WRITE set_VBinning)
  QCEP_INTEGER_PROPERTY(VBinning)

  Q_PROPERTY(double cameraGain READ get_CameraGain WRITE set_CameraGain)
  QCEP_DOUBLE_PROPERTY(CameraGain)

  Q_PROPERTY(int triggered READ get_Triggered WRITE set_Triggered)
  QCEP_INTEGER_PROPERTY(Triggered)

  Q_PROPERTY(QString userComment1 READ get_UserComment1 WRITE set_UserComment1)
  QCEP_STRING_PROPERTY(UserComment1)

  Q_PROPERTY(QString userComment2 READ get_UserComment2 WRITE set_UserComment2)
  QCEP_STRING_PROPERTY(UserComment2)

  Q_PROPERTY(QString userComment3 READ get_UserComment3 WRITE set_UserComment3)
  QCEP_STRING_PROPERTY(UserComment3)

  Q_PROPERTY(QString userComment4 READ get_UserComment4 WRITE set_UserComment4)
  QCEP_STRING_PROPERTY(UserComment4)

  Q_PROPERTY(QcepDoubleList normalization READ get_Normalization WRITE set_Normalization)
  QCEP_DOUBLE_LIST_PROPERTY(Normalization)

  Q_PROPERTY(QcepDoubleList extraInputs READ get_ExtraInputs WRITE set_ExtraInputs)
  QCEP_DOUBLE_LIST_PROPERTY(ExtraInputs)

  Q_PROPERTY(bool used READ get_Used WRITE set_Used)
  QCEP_BOOLEAN_PROPERTY(Used)

public slots:
  virtual QString description() const;
  void printMessage(QString msg, QDateTime ts=QDateTime::currentDateTime()) const;

  virtual double getImageData(int x, int y) const = 0;
  virtual QVector<double> getImageData(int x0, int y0, int x1, int y1) const = 0;
  virtual void setImageData(int x, int y, double v) = 0;

  virtual void clear() = 0;
  virtual void resize(int width, int height) = 0;
  virtual void fill(double val) = 0;

  virtual double minValue() const = 0;
  virtual double maxValue() const = 0;

  virtual QPointF percentileRange(double lowpct, double highpct) = 0;

  virtual double sumInRectangle(QRectF rect) = 0;
  virtual double averageInRectangle(QRectF rect) = 0;
  virtual double maxInRectangle(QRectF rect) = 0;
  virtual double minInRectangle(QRectF rect) = 0;
  virtual double sumInEllipse(QRectF rect) = 0;
  virtual double averageInEllipse(QRectF rect) = 0;
  virtual double minInEllipse(QRectF rect) = 0;
  virtual double maxInEllipse(QRectF rect) = 0;
  virtual double sumInPeak(QRectF rect) = 0;

  double hValue(int n) const;
  double vValue(int n) const;

public:
//  int get_Width() const
//  {
//    return m_Width;
//  }

//  int get_Height() const
//  {
//    return m_Height;
//  }

//  void set_Width(int width)
//  {
//    m_Width = width;
//  }

//  void set_Height(int height)
//  {
//    m_Height = height;
//  }

  QString get_DateString() const
  {
    return get_DateTime().toString(tr("yyyy.MM.dd : hh:mm:ss.zzz"));
  }

  void set_DateString(QString /*val*/) {}

  QString get_DataTypeName() const;
  void set_DataTypeName(QString /*name*/) {}

  void copyProperties(QcepImageDataBase *dest);
  void copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src);

  void loadMetaData();
  void saveMetaData();
  void saveMetaData(QString name);
  void saveTextData(QString name, QString sep, bool transp=false);

  void setDefaultFileName(QString path);

  virtual QString fileFormatFilterString();

  static double secondsSinceEpoch();

  enum {
    UndefinedData,
    Raw16Data,
    Raw32Data,
    DarkData,
    MaskData,
    SubtractedData,
    GainData,
    BadPixelsData
  };

  QMutex *mutex();

protected:
  int m_ImageCounter;
//  int m_Width;
//  int m_Height;

private:
  mutable QMutex m_Mutex;

protected:
  QcepSettingsSaverWPtr m_Saver;
};

template <typename T>
    class QcepImageData : public QcepImageDataBase
{
public:
  QcepImageData(QcepSettingsSaverWPtr saver, int width, int height, T def);
  virtual ~QcepImageData();

public:
  bool readImage(QString filename);

  void resize(int width, int height);
  void clear();
  void fill(double val);

  double getImageData(int x, int y) const;
  QVector<double> getImageData(int x0, int y0, int x1, int y1) const;
  void setImageData(int x, int y, double v);

  virtual void saveData(QString &name, QString filter, Overwrite canOverwrite=NoOverwrite);
  void saveTIFFData(QString name);

  double minValue() const;
  double maxValue() const;
  QPointF percentileRange(double lowpct, double highpct);

  double sumInRectangle(QRectF rect);
  double averageInRectangle(QRectF rect);
  double minInRectangle(QRectF rect);
  double maxInRectangle(QRectF rect);
  double sumInEllipse(QRectF rect);
  double averageInEllipse(QRectF rect);
  double minInEllipse(QRectF rect);
  double maxInEllipse(QRectF rect);
  double sumInPeak(QRectF rect);

public:
  template <typename T2>
  void subtractDark(const QSharedPointer< QcepImageData<T2> > dark);

  void calculateRange();
  void calculateRangeInCircle();

  void dumpPixels(int x0, int y0, int x1, int y1);

  T* data();

public:
  T value(int x, int y) const;
  T value(double x, double y) const;

  void setValue(int x, int y, T val);
  void addValue(int x, int y, T val);
  void subtractValue(int x, int y, T val);
  void multiplyValue(int x, int y, T val);
  void divideValue(int x, int y, T val);
//  void fill(T val);

  T defaultValue() const;
  void setDefaultValue(T def);

  QString rawFileName();

  template <typename T2>
  void copyImage(QSharedPointer< QcepImageData<T2> > dest);

  template <typename T2>
      void copyFrom(const QSharedPointer< QcepImageData<T2> > img);

  template <typename T2>
  void accumulateImage(QSharedPointer< QcepImageData<T2> > image);

  template <typename T2>
  void add(QSharedPointer< QcepImageData<T2> > image);
  void add(double val);

  template <typename T2>
  void subtract(QSharedPointer< QcepImageData<T2> > image);
  void subtract(double val);

  template <typename T2>
  void multiply(QSharedPointer< QcepImageData<T2> > image);
  void multiply(double val);

  template <typename T2>
  void divide(QSharedPointer< QcepImageData<T2> > image);
  void divide(double val);

  void setMask(QcepMaskDataPtr mask, QcepMaskDataPtr overflow);
  QcepMaskDataPtr mask() const;
  QcepMaskDataPtr overflow() const;

  double correlate(QSharedPointer< QcepImageData<T> > image, int dx, int dy, int mx, int my);

  void shiftImage(QSharedPointer< QcepImageData<T> > image, double dx, double dy);

  T findMin() const;
  T findMax() const;
  double findAverage() const;

  void correctBadBackgroundSubtraction(QcepDoubleImageDataPtr dark, int nImgExposures, int nDarkExposures);

  static QScriptValue toScriptValue(QScriptEngine *engine, const QSharedPointer< QcepImageData<T> > &data);
  static void fromScriptValue(const QScriptValue &obj, QSharedPointer< QcepImageData<T> > &data);


protected:
  QVector<T> m_Image;
  T m_MinValue;
  T m_MaxValue;
  T m_Default;

  QcepMaskDataPtr            m_Mask;
  QcepMaskDataPtr            m_Overflow;
};

#endif
