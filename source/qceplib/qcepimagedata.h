#ifndef QCEPIMAGEDATA_H
#define QCEPIMAGEDATA_H

#include "qcepmacros.h"
#include <QMutex>
#include <QString>
#include <QVector>
#include <QSharedPointer>
#include <QFileInfo>
#include <QDateTime>
#include <math.h>
#include <stdio.h>

#include "qcepproperty.h"
#include "qxrdsettingssaver-ptr.h"

typedef struct tiff TIFF;

class QcepImageDataBase : public QObject
{
  Q_OBJECT

public:
  QcepImageDataBase(QxrdSettingsSaverWPtr saver, int width, int height);
  virtual ~QcepImageDataBase();

  Q_PROPERTY(int width READ get_Width)
  //  QCEP_INTEGER_PROPERTY(Width)

  Q_PROPERTY(int height READ get_Height)
  //  QCEP_INTEGER_PROPERTY(Height)

  Q_PROPERTY(QString qxrdVersion READ get_QxrdVersion WRITE set_QxrdVersion)
  QCEP_STRING_PROPERTY(QxrdVersion)

  Q_PROPERTY(QString qtVersion READ get_QtVersion WRITE set_QtVersion)
  QCEP_STRING_PROPERTY(QtVersion)

  Q_PROPERTY(int dataType READ get_DataType WRITE set_DataType)
  QCEP_INTEGER_PROPERTY(DataType)

  Q_PROPERTY(QString dataTypeName READ get_DataTypeName)

  Q_PROPERTY(QString fileBase READ get_FileBase WRITE set_FileBase)
  QCEP_STRING_PROPERTY(FileBase)

  Q_PROPERTY(QString fileName READ get_FileName WRITE set_FileName)
  QCEP_STRING_PROPERTY(FileName)

  Q_PROPERTY(QString title READ get_Title WRITE set_Title)
  QCEP_STRING_PROPERTY(Title)

  Q_PROPERTY(int readoutMode READ get_ReadoutMode WRITE set_ReadoutMode)
  QCEP_INTEGER_PROPERTY(ReadoutMode)

  Q_PROPERTY(double exposureTime READ get_ExposureTime WRITE set_ExposureTime)
  QCEP_DOUBLE_PROPERTY(ExposureTime)

  Q_PROPERTY(int summedExposures READ get_SummedExposures WRITE set_SummedExposures)
  QCEP_INTEGER_PROPERTY(SummedExposures)

  Q_PROPERTY(int imageNumber READ get_ImageNumber WRITE set_ImageNumber)
  QCEP_INTEGER_PROPERTY(ImageNumber)

  Q_PROPERTY(int phaseNumber READ get_PhaseNumber WRITE set_PhaseNumber)
  QCEP_INTEGER_PROPERTY(PhaseNumber)

  Q_PROPERTY(int nPhases READ get_NPhases WRITE set_NPhases)
  QCEP_INTEGER_PROPERTY(NPhases)

  Q_PROPERTY(QDateTime dateTime READ get_DateTime WRITE set_DateTime)
  QCEP_DATETIME_PROPERTY(DateTime)

  Q_PROPERTY(QString dateString READ get_DateString)
  //  QCEP_STRING_PROPERTY(DateString)

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

  Q_PROPERTY(int imageSaved READ get_ImageSaved WRITE set_ImageSaved)
  QCEP_INTEGER_PROPERTY(ImageSaved)

  Q_PROPERTY(QcepDoubleList normalization READ get_Normalization WRITE set_Normalization)
  QCEP_DOUBLE_LIST_PROPERTY(Normalization)

  Q_PROPERTY(QcepDoubleList extraInputs READ get_ExtraInputs WRITE set_ExtraInputs)
  QCEP_DOUBLE_LIST_PROPERTY(ExtraInputs)

  Q_PROPERTY(bool used READ get_Used WRITE set_Used)
  QCEP_BOOLEAN_PROPERTY(Used)

  public slots:
    virtual double getImageData(int x, int y) const = 0;
    virtual QVector<double> getImageData(int x0, int y0, int x1, int y1) const = 0;

public:
  int get_Width() const
  {
    return m_Width;
  }

  int get_Height() const
  {
    return m_Height;
  }

  void set_Width(int width)
  {
    m_Width = width;
  }

  void set_Height(int height)
  {
    m_Height = height;
  }

  QString get_DateString() const
  {
    return get_DateTime().toString(tr("yyyy.MM.dd : hh:mm:ss.zzz"));
  }

  QString get_DataTypeName() const;

  void copyProperties(QcepImageDataBase *dest);
  void copyPropertiesFrom(QSharedPointer<QcepImageDataBase> src);

  void setTiffMetaData(TIFF *tif);
  void getTiffMetaData(TIFF *tif);

  void loadMetaData();
  void saveMetaData();
  void saveMetaData(QString name);

  void setDefaultFileName(QString path);

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
  int m_Width;
  int m_Height;

private:
  mutable QMutex m_Mutex;
};

template <typename T>
    class QcepImageData : public QcepImageDataBase
{
public:
  QcepImageData(QxrdSettingsSaverWPtr saver, int width, int height, T def=0);
  virtual ~QcepImageData();

public:
  bool readImage(QString filename);

  void resize(int width, int height);
  void clear();

  double getImageData(int x, int y) const;
  QVector<double> getImageData(int x0, int y0, int x1, int y1) const;

//  template <typename T2>
//  void copyImage(QSharedPointer< QcepImageData<T2> > dest);

  void calculateRange();
  void calculateRangeInCircle();

  void dumpPixels(int x0, int y0, int x1, int y1);

  T value(int x, int y) const;
  T value(double x, double y) const;

  T minValue() const;
  T maxValue() const;

  T* data();

  void setValue(int x, int y, T val);
  void addValue(int x, int y, T val);
  void subtractValue(int x, int y, T val);
  void multiplyValue(int x, int y, T val);
  void divideValue(int x, int y, T val);
  void fill(T val);

  T defaultValue() const;
  void setDefaultValue(T def);

protected:
  QVector<T> m_Image;
  T m_MinValue;
  T m_MaxValue;
  T m_Default;
};

#endif
