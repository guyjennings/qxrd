#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include <QReadWriteLock>
#include "qcepimagedata.h"

class QxrdImageData : public QcepImageData<double>
{
  public:
    QxrdImageData(int width=0, int height=0);

    int readoutMode();
    void setReadoutMode(int n);

    double exposureTime() const;
    void setExposureTime(double t);

    int summedExposures();
    void setSummedExposures(int n);

    QReadWriteLock *rwLock();

    int imageNumber();
    void setImageNumber(int n);

    QString rawFileName();

  private:
    mutable QReadWriteLock m_Lock;
    int m_ReadoutMode;
    double m_ExposureTime;
    int m_SummedExposures;
    int m_ImageNumber;
};

#endif
