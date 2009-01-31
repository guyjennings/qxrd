#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qspecserver.h"

class QxrdApplication;
class QxrdAcquisitionThread;

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

 public:
  QxrdServer(QxrdApplication *app, QxrdAcquisitionThread *acqth, QString name, QObject *parent=0);

 private:
  QxrdApplication          *m_Application;
  QxrdAcquisitionThread    *m_AcquisitionThread;
};

#endif
