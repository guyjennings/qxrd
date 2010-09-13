/******************************************************************
*
*  $Id: qxrdserver.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSERVER_H
#define QXRDSERVER_H

#include "qcepmacros.h"

#include "qxrdforwardtypes.h"
#include "qspecserver.h"
#include <QVariant>

class QxrdServer : public QSpecServer
{
  Q_OBJECT;

public:
  QxrdServer(QxrdAcquisitionThreadPtr acqth, QString name, int port, QObject *parent=0);

signals:
  void printMessage(QString msg);
  void statusMessage(QString msg);
  void criticalMessage(QString msg);

public slots:
  QVariant executeCommand(QString cmd);
  QVariant readProperty(QString name);
  void shutdown();

private:
  QxrdAcquisitionThreadPtr m_AcquisitionThread;
  HEADER_IDENT("$Id: qxrdserver.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdserver.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.2  2010/08/04 20:22:58  jennings
*  Added simple socket server, and prefs to control which servers are run, and on which ports
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.9.4.2  2010/04/26 23:46:13  jennings
*  *** empty log message ***
*
*  Revision 1.9.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.9  2009/09/20 21:18:53  jennings
*  Removed 'printf' messages
*  Added printMessage, statusMessage and criticalMessage functiosn for major classes.
*
*  Revision 1.8  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

