/******************************************************************
*
*  $Id: qxrdsettings.h,v 1.2 2010/09/13 20:00:42 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGS_H
#define QXRDSETTINGS_H

#include "qcepmacros.h"

#include <QSettings>

class QxrdSettings : public QSettings
{
  Q_OBJECT;

public:
  QxrdSettings(QObject *parent=0);
  ~QxrdSettings();

private:
  HEADER_IDENT("$Id: qxrdsettings.h,v 1.2 2010/09/13 20:00:42 jennings Exp $");
};

#endif

/******************************************************************
*
*  $Log: qxrdsettings.h,v $
*  Revision 1.2  2010/09/13 20:00:42  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/07/22 18:39:42  jennings
*  Moving files into source subdirectory
*
*  Revision 1.3.4.3  2010/05/19 21:15:35  jennings
*  PE acquisition debugging
*
*  Revision 1.3.4.2  2010/04/26 23:46:14  jennings
*  *** empty log message ***
*
*  Revision 1.3.4.1  2010/04/26 00:37:11  jennings
*  Attempting to convert to using QSharedPointers
*
*  Revision 1.3  2009/06/27 22:50:33  jennings
*  Added standard log entries and ident macros
*  Used standard property macros for acquisition parameters and image properties
*
*
*******************************************************************/

