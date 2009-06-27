/******************************************************************
*
*  $Id: qxrdserver.h,v 1.8 2009/06/27 22:50:33 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"

#include "qspecserver.h"

class QxrdApplication;
class QxrdAcquisitionThread;

#include <QScriptEngine>

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

public:
  QxrdServer(QxrdAcquisitionThread *acqth, QString name, QObject *parent=0);

signals:
  void printMessage(QString msg);

public slots:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();

private:
  QxrdAcquisitionThread    *m_AcquisitionThread;
  HEADER_IDENT("$Id: qxrdserver.h,v 1.8 2009/06/27 22:50:33 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdserver.h,v $
*  Revision 1.8  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

