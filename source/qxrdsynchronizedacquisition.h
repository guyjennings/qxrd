#ifndef QXRDSYNCHRONIZEDACQUISITION_H
#define QXRDSYNCHRONIZEDACQUISITION_H

#include <QObject>

class QxrdAcquisition;
class QxrdNIDAQPluginInterface;

class QxrdSynchronizedAcquisition : public QObject
{
  Q_OBJECT
public:
  explicit QxrdSynchronizedAcquisition(QxrdAcquisition *acq);

signals:

public slots:

public:
  void acquiredFrameAvailable(int currentExposure, int currentFile);
  void setNIDAQPlugin(QxrdNIDAQPluginInterface *nidaqPlugin);

private:
  QxrdAcquisition *m_Acquisition;
  QxrdNIDAQPluginInterface *m_NIDAQPlugin;
};

#endif // QXRDSYNCHRONIZEDACQUISITION_H
