#ifndef QXRDSYNCHRONIZEDACQUISITION_H
#define QXRDSYNCHRONIZEDACQUISITION_H

#include <QObject>

class QxrdAcquisition;

class QxrdSynchronizedAcquisition : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisition(QxrdAcquisition *acq);

signals:

public slots:

private:
  QxrdAcquisition *m_Acquisition;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
