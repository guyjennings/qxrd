/******************************************************************
*
*  $Id: qxrdfitrefiner.h,v 1.2 2010/09/13 20:00:40 jennings Exp $
*
*******************************************************************/

#ifndef QXRDFITREFINER_H
#define QXRDFITREFINER_H

#include "qcepmacros.h"
#include <QObject>

class QxrdFitRefiner : public QObject
{
  Q_OBJECT
public:
  explicit QxrdFitRefiner(QObject *parent = 0);

signals:

public slots:

private:
  HEADER_IDENT("$Id: qxrdfitrefiner.h,v 1.2 2010/09/13 20:00:40 jennings Exp $");
};

#endif // QXRDFITREFINER_H

/******************************************************************
*
*  $Log: qxrdfitrefiner.h,v $
*  Revision 1.2  2010/09/13 20:00:40  jennings
*  Merged
*
*  Revision 1.1.2.1  2010/08/03 20:14:37  jennings
*  Added classes for resampling ring centroids and fitting ring parameters
*
*
*******************************************************************/

