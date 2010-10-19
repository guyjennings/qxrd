/******************************************************************
*
*  $Id: qxrdsettingssaver.h,v 1.1 2010/10/19 18:23:38 jennings Exp $
*
*******************************************************************/

#ifndef QXRDSETTINGSSAVER_H
#define QXRDSETTINGSSAVER_H

#include "qcepmacros.h"
#include <QObject>

class QxrdSettingsSaver : public QObject
{
  Q_OBJECT;
public:
  explicit QxrdSettingsSaver(QObject *parent = 0);

signals:

public slots:

  HEADER_IDENT("$Id: qxrdsettingssaver.h,v 1.1 2010/10/19 18:23:38 jennings Exp $");
};

#endif // QXRDSETTINGSSAVER_H

/******************************************************************
*
*  $Log: qxrdsettingssaver.h,v $
*  Revision 1.1  2010/10/19 18:23:38  jennings
*  *** empty log message ***
*
*
*******************************************************************/

