#ifndef QXRDIMAGEDATA_H
#define QXRDIMAGEDATA_H

#include <QReadWriteLock.h>
#include "qcepimagedata.h"

class QxrdImageData : public QcepImageData<double>
{
  public:
    QxrdImageData(int width=0, int height=0);

    int integrationMode();
    void setIntegrationMode(int n);

    int nSummed();
    void setNSummed(int n);

    QReadWriteLock *rwLock();

  private:
    mutable QReadWriteLock m_Lock;
    int m_IntegrationMode;
    int m_NSummed;
};

#endif
